/*
 * H.26L/H.264/AVC/JVT/14496-10/... parser
 * Copyright (c) 2003 Michael Niedermayer <michaelni@gmx.at>
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file
 * H.264 / AVC / MPEG4 part10 parser.
 * @author Michael Niedermayer <michaelni@gmx.at>
 */

#include "parser.h"
#include "h264_parser.h"
#include "h264data.h"
#include "golomb.h"

#include <assert.h>

// #define CFG_H264_PARSE_TO_SPS_AFTER_FIRST_INIT
#define CFG_H264_PARSER_COMBIND_FIELD_FRAMES

typedef struct H264ParserContext{
	H264Context context;
	int first_init;
	int first_keyframe;
#ifdef CFG_H264_PARSER_COMBIND_FIELD_FRAMES
	int likely_first_field; // 0 - not field based, 1 - top field first, 2 - bottom field first
	int field_end_pos;
	int first_field_keyframe;
	int first_field_picType;
#endif	
}H264ParserContext;

int ff_h264_find_frame_end(H264Context *h, const uint8_t *buf, int buf_size)
{
    int i;
    uint32_t state;
    ParseContext *pc = &(h->s.parse_context);
//printf("first %02X%02X%02X%02X\n", buf[0], buf[1],buf[2],buf[3]);
//    mb_addr= pc->mb_addr - 1;
    state= pc->state;
    if(state>13)
        state= 7;

    for(i=0; i<buf_size; i++){
        if(state==7){
#if HAVE_FAST_UNALIGNED
        /* we check i<buf_size instead of i+3/7 because its simpler
         * and there should be FF_INPUT_BUFFER_PADDING_SIZE bytes at the end
         */
#    if HAVE_FAST_64BIT
            while(i<buf_size && !((~*(const uint64_t*)(buf+i) & (*(const uint64_t*)(buf+i) - 0x0101010101010101ULL)) & 0x8080808080808080ULL))
                i+=8;
#    else
            while(i<buf_size && !((~*(const uint32_t*)(buf+i) & (*(const uint32_t*)(buf+i) - 0x01010101U)) & 0x80808080U))
                i+=4;
#    endif
#endif
            for(; i<buf_size; i++){
                if(!buf[i]){
                    state=2;
                    break;
                }
            }
        }else if(state<=2){
            if(buf[i]==1)   state^= 5; //2->7, 1->4, 0->5
            else if(buf[i]) state = 7;
            else            state>>=1; //2->1, 1->0, 0->0
        }else if(state<=5){
            int v= buf[i] & 0x1F;
            if(v==6 || v==7 || v==8 || v==9){
                if(pc->frame_start_found){
                    i++;
                    goto found;
                }
            }else if(v==1 || v==2 || v==5){
                if(pc->frame_start_found){
                    state+=8;
                    continue;
                }else
                    pc->frame_start_found = 1;
            }
            state= 7;
        }else{
            if(buf[i] & 0x80)
                goto found;
            state= 7;
        }
    }
    pc->state= state;
    return END_NOT_FOUND;

found:
    pc->state=7;
    pc->frame_start_found= 0;
    return i-(state&5);
}

/*!
 * Parse NAL units of found picture and decode some basic information.
 *
 * @param s parser context.
 * @param avctx codec context.
 * @param buf buffer with field/frame data.
 * @param buf_size size of the buffer.
 */
static inline int parse_nal_units(AVCodecParserContext *s,
                                  AVCodecContext *avctx,
                                  const uint8_t *buf, int buf_size)
{
	H264ParserContext *ctx = s->priv_data;
    H264Context *h = &ctx->context;
    const uint8_t *buf_end = buf + buf_size;
    unsigned int pps_id;
    unsigned int slice_type;
    int state = -1;
    const uint8_t *ptr;

    /* set some sane default values */
    s->pict_type = FF_I_TYPE;
    s->key_frame = 0;

    h->s.avctx= avctx;
    h->sei_recovery_frame_cnt = -1;
    h->sei_dpb_output_delay         =  0;
    h->sei_cpb_removal_delay        = -1;
    h->sei_buffering_period_present =  0;

    for(;;) {
        int src_length, dst_length, consumed;
        buf = ff_find_start_code(buf, buf_end, &state);
        if(buf >= buf_end)
            break;
        --buf;
        src_length = buf_end - buf;
        switch (state & 0x1f) {
        case NAL_SLICE:
        case NAL_IDR_SLICE:
            // Do not walk the whole buffer just to decode slice header
            if (src_length > 20)
                src_length = 20;
            break;
        }
        ptr= ff_h264_decode_nal(h, buf, &dst_length, &consumed, src_length);
        if (ptr==NULL || dst_length < 0)
            break;

        init_get_bits(&h->s.gb, ptr, 8*dst_length);
        switch(h->nal_unit_type) {
        case NAL_SPS:
            ff_h264_decode_seq_parameter_set(h);
            break;
        case NAL_PPS:
            ff_h264_decode_picture_parameter_set(h, h->s.gb.size_in_bits);
            break;
        case NAL_SEI:
            ff_h264_decode_sei(h);
            break;
        case NAL_IDR_SLICE:
            s->key_frame = 1;
#ifdef CFG_H264_PARSER_COMBIND_FIELD_FRAMES
			ctx->likely_first_field = 0;
#endif
            /* fall through */
        case NAL_SLICE:
            get_ue_golomb(&h->s.gb);  // skip first_mb_in_slice
            slice_type = get_ue_golomb_31(&h->s.gb);
            s->pict_type = golomb_to_pict_type[slice_type % 5];
            if (h->sei_recovery_frame_cnt >= 0) {
                /* key frame, since recovery_frame_cnt is set */
                s->key_frame = 1;
            }
            pps_id= get_ue_golomb(&h->s.gb);
            if(pps_id>=MAX_PPS_COUNT) {
                av_log(h->s.avctx, AV_LOG_ERROR, "pps_id out of range\n");
                return -1;
            }
            if(!h->pps_buffers[pps_id]) {
                av_log(h->s.avctx, AV_LOG_ERROR, "non-existing PPS referenced\n");
                return -1;
            }
            h->pps= *h->pps_buffers[pps_id];
            if(!h->sps_buffers[h->pps.sps_id]) {
                av_log(h->s.avctx, AV_LOG_ERROR, "non-existing SPS referenced\n");
                return -1;
            }
            h->sps = *h->sps_buffers[h->pps.sps_id];
            h->frame_num = get_bits(&h->s.gb, h->sps.log2_max_frame_num);

            avctx->profile = h->sps.profile_idc;
            avctx->level   = h->sps.level_idc;

            if(h->sps.frame_mbs_only_flag){
                h->s.picture_structure= PICT_FRAME;
            }else{
                if(get_bits1(&h->s.gb)) { //field_pic_flag
                    h->s.picture_structure= PICT_TOP_FIELD + get_bits1(&h->s.gb); //bottom_field_flag
#ifdef CFG_H264_PARSER_COMBIND_FIELD_FRAMES
					if(!ctx->likely_first_field)
						ctx->likely_first_field = h->s.picture_structure;
#endif					
                } else {
                    h->s.picture_structure= PICT_FRAME;
                }
            }
#ifdef CFG_H264_PARSER_COMBIND_FIELD_FRAMES
			if(h->s.picture_structure == PICT_FRAME)
				ctx->likely_first_field = 0;
#endif
            if(h->sps.pic_struct_present_flag) {
                switch (h->sei_pic_struct) {
                    case SEI_PIC_STRUCT_TOP_FIELD:
                    case SEI_PIC_STRUCT_BOTTOM_FIELD:
                        s->repeat_pict = 0;
                        break;
                    case SEI_PIC_STRUCT_FRAME:
                    case SEI_PIC_STRUCT_TOP_BOTTOM:
                    case SEI_PIC_STRUCT_BOTTOM_TOP:
                        s->repeat_pict = 1;
                        break;
                    case SEI_PIC_STRUCT_TOP_BOTTOM_TOP:
                    case SEI_PIC_STRUCT_BOTTOM_TOP_BOTTOM:
                        s->repeat_pict = 2;
                        break;
                    case SEI_PIC_STRUCT_FRAME_DOUBLING:
                        s->repeat_pict = 3;
                        break;
                    case SEI_PIC_STRUCT_FRAME_TRIPLING:
                        s->repeat_pict = 5;
                        break;
                    default:
                        s->repeat_pict = h->s.picture_structure == PICT_FRAME ? 1 : 0;
                        break;
                }
            } else {
                s->repeat_pict = h->s.picture_structure == PICT_FRAME ? 1 : 0;
            }

            return 0; /* no need to evaluate the rest */
        }
        buf += consumed;
    }
    /* didn't find a picture! */
    av_log(h->s.avctx, AV_LOG_ERROR, "missing picture in access unit\n");
    return -1;
}

static int h264_parse(AVCodecParserContext *s,
                      AVCodecContext *avctx,
                      const uint8_t **poutbuf, int *poutbuf_size,
                      const uint8_t *buf, int buf_size)
{
	H264ParserContext *ctx = s->priv_data;
    H264Context *h = &ctx->context;
    ParseContext *pc = &h->s.parse_context;
    int next;
    int buf_size_orig = buf_size;
	int mb_width, mb_height;

#ifdef CFG_H264_PARSE_TO_SPS_AFTER_FIRST_INIT

    int state = -1;
    const uint8_t *buf_end = buf + buf_size;
    const uint8_t *tmpbuf ,*localbuf = buf;

    if (ctx->first_init)
    {
	
	 while(1)
	 {
	        localbuf = ff_find_start_code(localbuf, buf_end, &state);
	        if(localbuf >= buf_end)
	        {
	            *poutbuf = NULL;
 	            *poutbuf_size = 0;
 	            return buf_size;
	        }
	        tmpbuf = localbuf -4;
	        //av_log(NULL, AV_LOG_WARNING, ">> buf %02x %02x %02x %02x \n",*tmpbuf, *(tmpbuf+1), *(tmpbuf+2), *(tmpbuf+3));

	       if (state == 0x00000167)
	        {
	            av_log(NULL, AV_LOG_WARNING, ">> get the SPS \n");
	            localbuf -= 4;
	            buf_size -= localbuf - buf;
	            buf = localbuf;
	            ctx->first_init =0;
	        	break;
	        }else{
	           // av_log(NULL, AV_LOG_WARNING, ">> get not SPS NAL 0x%08x \n",state);
	        	continue;
	        }
	 }
	}

#endif

    //av_log(avctx, AV_LOG_ERROR, ">h264_parse()\n");
    if (!h->got_first) {
        h->got_first = 1;
        if (avctx->extradata_size) {
            h->s.avctx = avctx;
            ff_h264_decode_extradata(h);
        }
    }

    if(s->flags & PARSER_FLAG_COMPLETE_FRAMES){
        next= buf_size;
    }else{
        next= ff_h264_find_frame_end(h, buf, buf_size);

        if (ff_combine_frame(pc, next, &buf, &buf_size) < 0) {
            *poutbuf = NULL;
            *poutbuf_size = 0;
            return buf_size_orig;
        }

        if(next<0 && next != END_NOT_FOUND){
            assert(pc->last_index + next >= 0 );
            ff_h264_find_frame_end(h, &pc->buffer[pc->last_index + next], -next); //update state
        }

	}

	/* we have a full frame : we can parse nal information */
#ifdef CFG_H264_PARSER_COMBIND_FIELD_FRAMES
    int ret = parse_nal_units(s, avctx, buf+ctx->field_end_pos, buf_size-ctx->field_end_pos);
	if(ctx->field_end_pos){
		av_log(avctx, AV_LOG_DEBUG, "roll back first field info. type %d -> %d key %d -> %d\n",
			s->pict_type, ctx->first_field_picType, s->key_frame, ctx->first_field_keyframe);
		s->pict_type = ctx->first_field_picType;
		s->key_frame = ctx->first_field_keyframe;
	}
	ctx->field_end_pos = 0;
#else
    int ret = parse_nal_units(s, avctx, buf, buf_size);
#endif
	av_log(avctx, AV_LOG_DEBUG, "size %d type %d key %d\n",buf_size,s->pict_type,s->key_frame);

	//discard all the packets before the first keyframe
    if(ret == -1){
        *poutbuf = 0;
        *poutbuf_size = 0;
        return next;
    }

    if(s->key_frame == 1)
        ctx->first_keyframe = 1;

    if(!ctx->first_keyframe){
        *poutbuf = 0;
        *poutbuf_size = 0;
        return next;
    }
        
    if (h->sei_cpb_removal_delay >= 0) {
        s->dts_sync_point    = h->sei_buffering_period_present;
        s->dts_ref_dts_delta = h->sei_cpb_removal_delay;
        s->pts_dts_delta     = h->sei_dpb_output_delay;
    } else {
        s->dts_sync_point    = INT_MIN;
        s->dts_ref_dts_delta = INT_MIN;
        s->pts_dts_delta     = INT_MIN;
    }
    if (s->flags & PARSER_FLAG_ONCE)
        s->flags &= PARSER_FLAG_COMPLETE_FRAMES;

    mb_width= h->sps.mb_width;
    mb_height= h->sps.mb_height * (2 - h->sps.frame_mbs_only_flag);
	if(s->width != mb_width * 16 || s->height != mb_height * 16){
		int width = s->width, height = s->height;
		if(width || height)
			s->flags |= PARSER_FLAG_RESOLUTION_CHANGE;
		s->width = mb_width * 16;
		s->height = mb_height * 16;
		s->crop_left = 2*FFMIN(h->sps.crop_left, 7); // not sure
		s->crop_top = 2*FFMIN(h->sps.crop_top, 7); // not sure
		s->crop_right = 2*FFMIN(h->sps.crop_right, 7);
		if(h->sps.frame_mbs_only_flag)
			s->crop_bottom = 2*FFMIN(h->sps.crop_bottom, 7);
		else
			s->crop_bottom = 4*FFMIN(h->sps.crop_bottom, 3);
		av_log(avctx, AV_LOG_INFO, "[%s] Resolution Change %d/%d -> %d/%d crop (%d,%d)/(%d,%d)\n",__func__,
			width,height,s->width,s->height,s->crop_left,s->crop_top,s->crop_right,s->crop_bottom);
	}

#ifdef CFG_H264_PARSER_COMBIND_FIELD_FRAMES
	if(h->s.picture_structure == ctx->likely_first_field){
		av_log(NULL,AV_LOG_DEBUG,"Field not complete. reserve frame.\n");
		ff_combine_frame(pc, END_NOT_FOUND, &buf, &buf_size);
		ctx->field_end_pos = pc->index;
		ctx->first_field_keyframe = s->key_frame;
		ctx->first_field_picType = s->pict_type;
		buf_size = 0;
		buf = NULL;
	}
#endif

    *poutbuf = buf;
    *poutbuf_size = buf_size;
    return next;
}

static int h264_split(AVCodecContext *avctx,
                      const uint8_t *buf, int buf_size)
{
    int i;
    uint32_t state = -1;
    int has_sps= 0;

    for(i=0; i<=buf_size; i++){
        if((state&0xFFFFFF1F) == 0x107)
            has_sps=1;
/*        if((state&0xFFFFFF1F) == 0x101 || (state&0xFFFFFF1F) == 0x102 || (state&0xFFFFFF1F) == 0x105){
        }*/
        if((state&0xFFFFFF00) == 0x100 && (state&0xFFFFFF1F) != 0x107 && (state&0xFFFFFF1F) != 0x108 && (state&0xFFFFFF1F) != 0x109){
            if(has_sps){
                while(i>4 && buf[i-5]==0) i--;
                return i-4;
            }
        }
        if (i<buf_size)
            state= (state<<8) | buf[i];
    }
    return 0;
}

static void close(AVCodecParserContext *s)
{
	H264ParserContext *ctx = s->priv_data;
    H264Context *h = &ctx->context;
    ParseContext *pc = &h->s.parse_context;

    av_free(pc->buffer);
	ctx->first_keyframe = 0;
#ifdef CFG_H264_PARSER_COMBIND_FIELD_FRAMES
	ctx->field_end_pos = 0;
	ctx->first_field_keyframe = 0;
	ctx->first_field_picType = 0;
	ctx->likely_first_field = 0;
#endif	
    ff_h264_free_context(h);
}

static int init(AVCodecParserContext *s)
{
	H264ParserContext *ctx = s->priv_data;
    H264Context *h = &ctx->context;
    h->thread_context[0] = h;
	ctx->first_init = 1;
    ctx->first_keyframe = 0;
#ifdef CFG_H264_PARSER_COMBIND_FIELD_FRAMES
	ctx->field_end_pos = 0;
	ctx->first_field_keyframe = 0;
	ctx->first_field_picType = 0;
	ctx->likely_first_field = 0;
#endif	
    return 0;
}

AVCodecParser h264_parser = {
    { CODEC_ID_H264 },
    sizeof(H264ParserContext),
    init,
    h264_parse,
    close,
    h264_split,
    NULL,
};
