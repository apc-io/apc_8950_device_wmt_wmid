/*++
Copyright(c) 2011  WonderMedia Technologies, Inc. All Rights Reserved.

This PROPRIETARY SOFTWARE is the property of WonderMedia Technologies, Inc.
and may contain trade secrets and/or other confidential information of
WonderMedia Technologies, Inc. This file shall not be disclosed to any third
party, in whole or in part, without prior written consent of WonderMedia.  

THIS PROPRIETARY SOFTWARE AND ANY RELATED DOCUMENTATION ARE PROVIDED AS IS,
WITH ALL FAULTS, AND WITHOUT WARRANTY OF ANY KIND EITHER EXPRESS OR IMPLIED,
AND WonderMedia TECHNOLOGIES, INC. DISCLAIMS ALL EXPRESS OR IMPLIED
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET
ENJOYMENT OR NON-INFRINGEMENT.  
--*/

#ifndef	__WMT_AC3_H__
#define __WMT_AC3_H__

#include "libavcodec/get_bits.h"
#include "libavcodec/dsputil.h"


typedef struct {
    AVCodecContext *avctx;                  ///< parent context
    GetBitContext gbc;                      ///< bitstream reader
    uint8_t *input_buffer;                  ///< temp buffer to prevent overread
} WMT_AC3DecContext;


typedef struct {
	int16_t SyncWord1;
	int16_t SyncWord2;
	int16_t BurstInfo;
	int16_t Length;
}WMT_AC3SPDIFHeader;


typedef	struct
{
	int	kcapablemode;		/* Karaoke capable reproduction mode, default = 3
								0 = no vocal
								1 = left vocal
								2 = right vocal
								3 = both vocals */

	int	compmode; 			/* Dynamic range compression mode, default = 2
								0 = custom mode, analog dialnorm
								1 = custom mode, digital dialnorm
								2 = line out mode
								3 = RF remod mode */

	int outlfeon;			/* Output subwoofer present flag, default = 1 */

	int outputmode;			/* Output channel configuration, default = 7
								0 = reserved
								1 = 1/0 (C)
								2 = 2/0 (L, R)
								3 = 3/0 (L, C, R)
								4 = 2/1 (L, R, l)
								5 = 3/1 (L, C, R, l)
								6 = 2/2 (L, R, l, r)
								7 = 3/2 (L, C, R, l, r)	*/

	int numchans;			/* Number of output channels, default = 6 */

	int	stereomode;			/* Stereo downmix mode, default = 0 (only effective when outputmod = 2)
								0 = auto detect
								1 = Dolby Surround compatible (Lt/Rt)
								2 = Stereo (Lo/Ro) */

	int dualmonomode;		/* Dual mono reproduction mode, default = 0
								0 = Stereo
								1 = Left mono
								2 = Right mono
								3 = Mixed mono */

	int	chanptr[6];			/* Channel routing assignment, default = [0, 1, 2, 3, 4, 5]
								where L=0, C=1, R=2, l=3, r=4, s=5 */
} dolbyAC3DecoderParam;


/*
 	If param == NULL, use default options to param
	RETURN 0, if succeeded, else failed
*/
int	dolbyAC3DecoderInit(dolbyAC3DecoderParam* param);


/*
	Caller mustn't feed a frame that size is smaller than a valid one
	If you feed a larger one and it's valid, then its return value will tell you how much it consumes to decode the current frame
	Also the outPCM is stored in frames, and outPCM Size would tell you how large in size it decoded to this frame
	RETURN:
   		0, if frame header is invalid
		-1, if frame header doesn't contain a valid AC3 Sync Word
		else if outPCMDataSize == 0, there are problems in frame data
*/
int dolbyAC3DecoderDecodeFrame(short* outPCM, int* outPCMSize, const void* inAC3Data, const int inAC3DataSize);

int dolbyAC3DecoderUninit();

#endif
