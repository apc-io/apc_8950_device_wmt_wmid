#ifndef CODEC_CONFIG_H
#define CODEC_CONFIG_H

typedef struct{
	// Video Codec
	unsigned char WMT_MJPEG_DECODER;
	unsigned char WMT_MPEG1VIDEO_DECODER; // related to MPEG-1, switch ON/OFF
	unsigned char WMT_MPEG2VIDEO_DECODER; // releted to MPEG-2, switch ON/OFF
	unsigned char WMT_MPEGVIDEO_DECODER; // releted to MPEG-2, switch ON/OFF
	unsigned char WMT_MPEG4_DECODER; // releated to MPEG-4, switch ON/OFF
	unsigned char WMT_MSMPEG4V3_DECODER; // releated to DIVX, switch ON/OFF
	unsigned char WMT_H263_DECODER; // releted to H263, switch ON/OFF
	unsigned char WMT_FLV_DECODER; // releted to H263, switch ON/OFF
	unsigned char WMT_VP8_DECODER; // releted to VP8, switch ON/OFF
	unsigned char WMT_H264_DECODER; // releted to H264, switch ON/OFF
	unsigned char WMT_RV30_DECODER; // releted to RV, switch ON/OFF
	unsigned char WMT_RV40_DECODER; // releted to RV, switch ON/OFF
	unsigned char WMT_VC1_DECODER; // releted to VC1, switch ON/OFF
	unsigned char WMT_WMV3_DECODER; // releted to VC1, switch ON/OFF
	unsigned char AASC_DECODER;
	unsigned char AMV_DECODER;
	unsigned char ANM_DECODER;
	unsigned char ASV1_DECODER;
	unsigned char ASV2_DECODER;
	unsigned char AURA_DECODER;
	unsigned char AURA2_DECODER;
	unsigned char AVS_DECODER;
	unsigned char BETHSOFTVID_DECODER;
	unsigned char BFI_DECODER;
	unsigned char BINK_DECODER;
	unsigned char BMP_DECODER;
	unsigned char C93_DECODER;
	unsigned char CAVS_DECODER;
	unsigned char CDGRAPHICS_DECODER;
	unsigned char CINEPAK_DECODER;
	unsigned char CLJR_DECODER;
	unsigned char CSCD_DECODER;
	unsigned char CYUV_DECODER;
	unsigned char DNXHD_DECODER;
	unsigned char DPX_DECODER;
	unsigned char DSICINVIDEO_DECODER;
	unsigned char DVVIDEO_DECODER;
	unsigned char DXA_DECODER;
	unsigned char EACMV_DECODER;
	unsigned char EAMAD_DECODER;
	unsigned char EATGQ_DECODER;
	unsigned char EATGV_DECODER;
	unsigned char EATQI_DECODER;
	unsigned char EIGHTBPS_DECODER;
	unsigned char EIGHTSVX_EXP_DECODER;
	unsigned char EIGHTSVX_FIB_DECODER;
	unsigned char ESCAPE124_DECODER;
	unsigned char FFV1_DECODER;
	unsigned char FFVHUFF_DECODER;
	unsigned char FLASHSV_DECODER;
	unsigned char FLIC_DECODER;
	unsigned char FLV_DECODER;
	unsigned char FOURXM_DECODER;
	unsigned char FRAPS_DECODER;
	unsigned char FRWU_DECODER;
	unsigned char GIF_DECODER;
	unsigned char H261_DECODER;
	unsigned char H263_DECODER;
	unsigned char H263I_DECODER;
	unsigned char H264_DECODER;
	unsigned char H264_VDPAU_DECODER;
	unsigned char HUFFYUV_DECODER;
	unsigned char IDCIN_DECODER;
	unsigned char IFF_BYTERUN1_DECODER;
	unsigned char IFF_ILBM_DECODER;
	unsigned char INDEO2_DECODER;
	unsigned char INDEO3_DECODER;
	unsigned char INDEO5_DECODER;
	unsigned char INTERPLAY_VIDEO_DECODER;
	unsigned char JPEGLS_DECODER;
	unsigned char KGV1_DECODER;
	unsigned char KMVC_DECODER;
	unsigned char LOCO_DECODER;
	unsigned char MDEC_DECODER;
	unsigned char MIMIC_DECODER;
	unsigned char MJPEG_DECODER;
	unsigned char MJPEGB_DECODER;
	unsigned char MMVIDEO_DECODER;
	unsigned char MOTIONPIXELS_DECODER;
	unsigned char MPEG_XVMC_DECODER;
	unsigned char MPEG1VIDEO_DECODER;
	unsigned char MPEG2VIDEO_DECODER;
	unsigned char MPEG4_DECODER;
	unsigned char MPEG4_VDPAU_DECODER;
	unsigned char MPEGVIDEO_DECODER;
	unsigned char MPEG_VDPAU_DECODER;
	unsigned char MPEG1_VDPAU_DECODER;
	unsigned char MSMPEG4V1_DECODER;
	unsigned char MSMPEG4V2_DECODER;
	unsigned char MSMPEG4V3_DECODER;
	unsigned char MSRLE_DECODER;
	unsigned char MSVIDEO1_DECODER;
	unsigned char MSZH_DECODER;
	unsigned char NUV_DECODER;
	unsigned char PAM_DECODER;
	unsigned char PBM_DECODER;
	unsigned char PCX_DECODER;
	unsigned char PGM_DECODER;
	unsigned char PGMYUV_DECODER;
	unsigned char PICTOR_DECODER;
	unsigned char PNG_DECODER;
	unsigned char PPM_DECODER;
	unsigned char PTX_DECODER;
	unsigned char QDRAW_DECODER;
	unsigned char QPEG_DECODER;
	unsigned char QTRLE_DECODER;
	unsigned char R210_DECODER;
	unsigned char RAWVIDEO_DECODER;
	unsigned char RL2_DECODER;
	unsigned char ROQ_DECODER;
	unsigned char RPZA_DECODER;
	unsigned char RV10_DECODER;
	unsigned char RV20_DECODER;
	unsigned char RV30_DECODER;
	unsigned char RV40_DECODER;
	unsigned char SGI_DECODER;
	unsigned char SMACKER_DECODER;
	unsigned char SMC_DECODER;
	unsigned char SNOW_DECODER;
	unsigned char SP5X_DECODER;
	unsigned char SUNRAST_DECODER;
	unsigned char SVQ1_DECODER;
	unsigned char SVQ3_DECODER;
	unsigned char TARGA_DECODER;
	unsigned char THEORA_DECODER;
	unsigned char THP_DECODER;
	unsigned char TIERTEXSEQVIDEO_DECODER;
	unsigned char TIFF_DECODER;
	unsigned char TMV_DECODER;
	unsigned char TRUEMOTION1_DECODER;
	unsigned char TRUEMOTION2_DECODER;
	unsigned char TSCC_DECODER;
	unsigned char TXD_DECODER;
	unsigned char ULTI_DECODER;
	unsigned char V210_DECODER;
	unsigned char V210X_DECODER;
	unsigned char VB_DECODER;
	unsigned char VC1_DECODER;
	unsigned char VC1_VDPAU_DECODER;
	unsigned char VCR1_DECODER;
	unsigned char VMDVIDEO_DECODER;
	unsigned char VMNC_DECODER;
	unsigned char VP3_DECODER;
	unsigned char VP5_DECODER;
	unsigned char VP6_DECODER;
	unsigned char VP6A_DECODER;
	unsigned char VP6F_DECODER;
	unsigned char VP8_DECODER;
	unsigned char VQA_DECODER;
	unsigned char WMV1_DECODER;
	unsigned char WMV2_DECODER;
	unsigned char WMV3_DECODER;
	unsigned char WMV3_VDPAU_DECODER;
	unsigned char WNV1_DECODER;
	unsigned char XAN_WC3_DECODER;
	unsigned char XL_DECODER;
	unsigned char YOP_DECODER;
	unsigned char ZLIB_DECODER;
	unsigned char ZMBV_DECODER;
	// Audio Codec
	unsigned char WMT_AC3_DECODER;
	unsigned char WMT_EAC3_DECODER;
	unsigned char LIBMAD_MP1_DECODER; // releted to MP3, switch ON/OFF
	unsigned char LIBMAD_MP2_DECODER; // releted to MP3, switch ON/OFF
	unsigned char LIBMAD_MP3_DECODER; // releted to MP3, switch ON/OFF
	unsigned char LIBFAAD_DECODER; // releted to AAC, switch ON/OFF
	unsigned char AAC_DECODER; // releted to AAC, switch ON/OFF
	unsigned char AC3_DECODER; // releted to AC3, switch ON/OFF
	unsigned char ALAC_DECODER;
	unsigned char ALS_DECODER;
	unsigned char AMRNB_DECODER; // related to AMR 
	unsigned char APE_DECODER;
	unsigned char ATRAC1_DECODER;
	unsigned char ATRAC3_DECODER; // related to RM
	unsigned char BINKAUDIO_DCT_DECODER;
	unsigned char BINKAUDIO_RDFT_DECODER;
	unsigned char COOK_DECODER; // related to RM
	unsigned char DCA_DECODER; // related to DTS
	unsigned char DSICINAUDIO_DECODER;
	unsigned char EAC3_DECODER; // releted to AC3, switch ON/OFF
	unsigned char FLAC_DECODER;
	unsigned char GSM_DECODER;
	unsigned char GSM_MS_DECODER;
	unsigned char IMC_DECODER;
	unsigned char MACE3_DECODER;
	unsigned char MACE6_DECODER;
	unsigned char MLP_DECODER;
	unsigned char MP1_DECODER; // releted to MP3, switch ON/OFF
	unsigned char MP1FLOAT_DECODER; // releted to MP3, switch ON/OFF
	unsigned char MP2_DECODER; // releted to MP3, switch ON/OFF
	unsigned char MP2FLOAT_DECODER; // releted to MP3, switch ON/OFF
	unsigned char MP3_DECODER; // releted to MP3, switch ON/OFF
	unsigned char MP3FLOAT_DECODER; // releted to MP3, switch ON/OFF
	unsigned char MP3ADU_DECODER; // releted to MP3, switch ON/OFF
	unsigned char MP3ADUFLOAT_DECODER; // releted to MP3, switch ON/OFF
	unsigned char MP3ON4_DECODER; // releted to MP3, switch ON/OFF
	unsigned char MP3ON4FLOAT_DECODER; // releted to MP3, switch ON/OFF
	unsigned char MPC7_DECODER;
	unsigned char MPC8_DECODER;
	unsigned char NELLYMOSER_DECODER;
	unsigned char QCELP_DECODER;
	unsigned char QDM2_DECODER;
	unsigned char RA_144_DECODER; //  releted to RM
	unsigned char RA_288_DECODER; //  releted to RM
	unsigned char SHORTEN_DECODER;
	unsigned char SIPR_DECODER; // related to RM
	unsigned char SMACKAUD_DECODER;
	unsigned char SONIC_DECODER;
	unsigned char TRUEHD_DECODER;
	unsigned char TRUESPEECH_DECODER;
	unsigned char TTA_DECODER;
	unsigned char TWINVQ_DECODER;
	unsigned char VMDAUDIO_DECODER;
	unsigned char VORBIS_DECODER; // related to OGG, switch ON/OFF
	unsigned char WAVPACK_DECODER;
	unsigned char WMAPRO_DECODER; // related to WMA, switch ON/OFF
	unsigned char WMAV1_DECODER; // related to WMA, switch ON/OFF
	unsigned char WMAV2_DECODER; // related to WMA, switch ON/OFF
	unsigned char WMAVOICE_DECODER; // related to WMA, switch ON/OFF
	unsigned char WS_SND1_DECODER;
	unsigned char PCM_ALAW_DECODER; // related to PCM, switch ON/OFF
	unsigned char PCM_BLURAY_DECODER; // related to PCM, switch ON/OFF
	unsigned char PCM_DVD_DECODER; // related to PCM, switch ON/OFF
	unsigned char PCM_F32BE_DECODER; // related to PCM, switch ON/OFF 
	unsigned char PCM_F32LE_DECODER; // related to PCM, switch ON/OFF
	unsigned char PCM_F64BE_DECODER; // related to PCM, switch ON/OFF
	unsigned char PCM_F64LE_DECODER; // related to PCM, switch ON/OFF
	unsigned char PCM_MULAW_DECODER; // related to PCM, switch ON/OFF
	unsigned char PCM_S8_DECODER; // related to PCM, switch ON/OFF
	unsigned char PCM_S16BE_DECODER; // related to PCM, switch ON/OFF
	unsigned char PCM_S16LE_DECODER; // related to PCM, switch ON/OFF
	unsigned char PCM_S16LE_PLANAR_DECODER; // related to PCM, switch ON/OFF
	unsigned char PCM_S24BE_DECODER; // related to PCM, switch ON/OFF
	unsigned char PCM_S24DAUD_DECODER; // related to PCM, switch ON/OFF
	unsigned char PCM_S24LE_DECODER; // related to PCM, switch ON/OFF
	unsigned char PCM_S32BE_DECODER; // related to PCM, switch ON/OFF
	unsigned char PCM_S32LE_DECODER; // related to PCM, switch ON/OFF
	unsigned char PCM_U8_DECODER; // related to PCM, switch ON/OFF
	unsigned char PCM_U16BE_DECODER; // related to PCM, switch ON/OFF
	unsigned char PCM_U16LE_DECODER; // related to PCM, switch ON/OFF
	unsigned char PCM_U24BE_DECODER; // related to PCM, switch ON/OFF
	unsigned char PCM_U24LE_DECODER; // related to PCM, switch ON/OFF
	unsigned char PCM_U32BE_DECODER; // related to PCM, switch ON/OFF
	unsigned char PCM_U32LE_DECODER; // related to PCM, switch ON/OFF
	unsigned char PCM_ZORK_DECODER; // related to PCM, switch ON/OFF
	unsigned char INTERPLAY_DPCM_DECODER; // related to DPCM
	unsigned char ROQ_DPCM_DECODER; // related to DPCM
	unsigned char SOL_DPCM_DECODER; // related to DPCM
	unsigned char XAN_DPCM_DECODER; // related to DPCM
	unsigned char ADPCM_4XM_DECODER; // related to ADPCM
	unsigned char ADPCM_ADX_DECODER; // related to ADPCM
	unsigned char ADPCM_CT_DECODER; // related to ADPCM
	unsigned char ADPCM_EA_DECODER; // related to ADPCM
	unsigned char ADPCM_EA_MAXIS_XA_DECODER; // related to ADPCM
	unsigned char ADPCM_EA_R1_DECODER; // related to ADPCM
	unsigned char ADPCM_EA_R2_DECODER; // related to ADPCM
	unsigned char ADPCM_EA_R3_DECODER; // related to ADPCM
	unsigned char ADPCM_EA_XAS_DECODER; // related to ADPCM
	unsigned char ADPCM_G726_DECODER; // related to ADPCM
	unsigned char ADPCM_IMA_AMV_DECODER; // related to ADPCM
	unsigned char ADPCM_IMA_DK3_DECODER; // related to ADPCM
	unsigned char ADPCM_IMA_DK4_DECODER; // related to ADPCM
	unsigned char ADPCM_IMA_EA_EACS_DECODER; // related to ADPCM
	unsigned char ADPCM_IMA_EA_SEAD_DECODER; // related to ADPCM
	unsigned char ADPCM_IMA_ISS_DECODER; // related to ADPCM
	unsigned char ADPCM_IMA_QT_DECODER; // related to ADPCM
	unsigned char ADPCM_IMA_SMJPEG_DECODER; // related to ADPCM
	unsigned char ADPCM_IMA_WAV_DECODER; // related to ADPCM
	unsigned char ADPCM_IMA_WS_DECODER; // related to ADPCM
	unsigned char ADPCM_MS_DECODER; // related to ADPCM
	unsigned char ADPCM_SBPRO_2_DECODER; // related to ADPCM
	unsigned char ADPCM_SBPRO_3_DECODER; // related to ADPCM
	unsigned char ADPCM_SBPRO_4_DECODER; // related to ADPCM
	unsigned char ADPCM_SWF_DECODER; // related to ADPCM
	unsigned char ADPCM_THP_DECODER; // related to ADPCM
	unsigned char ADPCM_XA_DECODER; // related to ADPCM
	unsigned char ADPCM_YAMAHA_DECODER; // related to ADPCM
	// subtitle DECODER
	unsigned char DVBSUB_DECODER;
	unsigned char DVDSUB_DECODER;
	unsigned char PGSSUB_DECODER;
	unsigned char XSUB_DECODER;
	// plugin library DECODER
	unsigned char LIBDIRAC_DECODER;
	unsigned char LIBGSM_DECODER;
	unsigned char LIBGSM_MS_DECODER;
	unsigned char LIBOPENCORE_AMRNB_DECODER; // related to AMR
	unsigned char LIBOPENCORE_AMRWB_DECODER; // related to AMR
	unsigned char LIBOPENJPEG_DECODER;
	unsigned char LIBSCHROEDINGER_DECODER;
	unsigned char LIBSPEEX_DECODER;
	unsigned char LIBVPX_DECODER;
}codec_conf;

typedef struct{
	char *tag;
	unsigned char num;
	unsigned char **codecs;
}CodecSwitch;

#ifdef ALLCODECS_C
codec_conf PE_CODEC_CONF = {
	.WMT_MJPEG_DECODER = 1,
	.WMT_MPEG1VIDEO_DECODER = 1,
	.WMT_MPEG2VIDEO_DECODER = 1,
	.WMT_MPEGVIDEO_DECODER = 1,
	.WMT_MPEG4_DECODER = 1,
	.WMT_MSMPEG4V3_DECODER = 1,
	.WMT_H263_DECODER = 1,
	.WMT_FLV_DECODER = 1,
    .WMT_VP8_DECODER = 1,
	.WMT_H264_DECODER = 1,
	.WMT_RV30_DECODER = 1,
	.WMT_RV40_DECODER = 1,
	.WMT_VC1_DECODER = 1,
	.WMT_WMV3_DECODER = 1,
	.AASC_DECODER = 1,
	.AMV_DECODER = 1,
	.ANM_DECODER = 1,
	.ASV1_DECODER = 1,
	.ASV2_DECODER = 1,
	.AURA_DECODER = 1,
	.AURA2_DECODER = 1,
	.AVS_DECODER = 1,
	.BETHSOFTVID_DECODER = 1,
	.BFI_DECODER = 1,
	.BINK_DECODER = 1,
	.BMP_DECODER = 1,
	.C93_DECODER = 1,
	.CAVS_DECODER = 1,
	.CDGRAPHICS_DECODER = 1,
	.CINEPAK_DECODER = 1,
	.CLJR_DECODER = 1,
	.CSCD_DECODER = 1,
	.CYUV_DECODER = 1,
	.DNXHD_DECODER = 1,
	.DPX_DECODER = 1,
	.DSICINVIDEO_DECODER = 1,
	.DVVIDEO_DECODER = 1,
	.DXA_DECODER = 1,
	.EACMV_DECODER = 1,
	.EAMAD_DECODER = 1,
	.EATGQ_DECODER = 1,
	.EATGV_DECODER = 1,
	.EATQI_DECODER = 1,
	.EIGHTBPS_DECODER = 1,
	.EIGHTSVX_EXP_DECODER = 1,
	.EIGHTSVX_FIB_DECODER = 1,
	.ESCAPE124_DECODER = 1,
	.FFV1_DECODER = 1,
	.FFVHUFF_DECODER = 1,
	.FLASHSV_DECODER = 1,
	.FLIC_DECODER = 1,
	.FLV_DECODER = 1,
	.FOURXM_DECODER = 1,
	.FRAPS_DECODER = 1,
	.FRWU_DECODER = 1,
	.GIF_DECODER = 1,
	.H261_DECODER = 1,
	.H263_DECODER = 1,
	.H263I_DECODER = 1,
	.H264_DECODER = 1,
	.H264_VDPAU_DECODER = 1,
	.HUFFYUV_DECODER = 1,
	.IDCIN_DECODER = 1,
	.IFF_BYTERUN1_DECODER = 1,
	.IFF_ILBM_DECODER = 1,
	.INDEO2_DECODER = 1,
	.INDEO3_DECODER = 1,
	.INDEO5_DECODER = 1,
	.INTERPLAY_VIDEO_DECODER = 1,
	.JPEGLS_DECODER = 1,
	.KGV1_DECODER = 1,
	.KMVC_DECODER = 1,
	.LOCO_DECODER = 1,
	.MDEC_DECODER = 1,
	.MIMIC_DECODER = 1,
	.MJPEG_DECODER = 1,
	.MJPEGB_DECODER = 1,
	.MMVIDEO_DECODER = 1,
	.MOTIONPIXELS_DECODER = 1,
	.MPEG_XVMC_DECODER = 1,
	.MPEG1VIDEO_DECODER = 1,
	.MPEG2VIDEO_DECODER = 1,
	.MPEG4_DECODER = 1,
	.MPEG4_VDPAU_DECODER = 1,
	.MPEGVIDEO_DECODER = 1,
	.MPEG_VDPAU_DECODER = 1,
	.MPEG1_VDPAU_DECODER = 1,
	.MSMPEG4V1_DECODER = 1,
	.MSMPEG4V2_DECODER = 1,
	.MSMPEG4V3_DECODER = 1,
	.MSRLE_DECODER = 1,
	.MSVIDEO1_DECODER = 1,
	.MSZH_DECODER = 1,
	.NUV_DECODER = 1,
	.PAM_DECODER = 1,
	.PBM_DECODER = 1,
	.PCX_DECODER = 1,
	.PGM_DECODER = 1,
	.PGMYUV_DECODER = 1,
	.PICTOR_DECODER = 1,
	.PNG_DECODER = 1,
	.PPM_DECODER = 1,
	.PTX_DECODER = 1,
	.QDRAW_DECODER = 1,
	.QPEG_DECODER = 1,
	.QTRLE_DECODER = 1,
	.R210_DECODER = 1,
	.RAWVIDEO_DECODER = 1,
	.RL2_DECODER = 1,
	.ROQ_DECODER = 1,
	.RPZA_DECODER = 1,
	.RV10_DECODER = 1,
	.RV20_DECODER = 1,
	.RV30_DECODER = 1,
	.RV40_DECODER = 1,
	.SGI_DECODER = 1,
	.SMACKER_DECODER = 1,
	.SMC_DECODER = 1,
	.SNOW_DECODER = 1,
	.SP5X_DECODER = 1,
	.SUNRAST_DECODER = 1,
	.SVQ1_DECODER = 1,
	.SVQ3_DECODER = 1,
	.TARGA_DECODER = 1,
	.THEORA_DECODER = 1,
	.THP_DECODER = 1,
	.TIERTEXSEQVIDEO_DECODER = 1,
	.TIFF_DECODER = 1,
	.TMV_DECODER = 1,
	.TRUEMOTION1_DECODER = 1,
	.TRUEMOTION2_DECODER = 1,
	.TSCC_DECODER = 1,
	.TXD_DECODER = 1,
	.ULTI_DECODER = 1,
	.V210_DECODER = 1,
	.V210X_DECODER = 1,
	.VB_DECODER = 1,
	.VC1_DECODER = 1,
	.VC1_VDPAU_DECODER = 1,
	.VCR1_DECODER = 1,
	.VMDVIDEO_DECODER = 1,
	.VMNC_DECODER = 1,
	.VP3_DECODER = 1,
	.VP5_DECODER = 1,
	.VP6_DECODER = 1,
	.VP6A_DECODER = 1,
	.VP6F_DECODER = 1,
	.VP8_DECODER = 1,
	.VQA_DECODER = 1,
	.WMV1_DECODER = 1,
	.WMV2_DECODER = 1,
	.WMV3_DECODER = 1,
	.WMV3_VDPAU_DECODER = 1,
	.WNV1_DECODER = 1,
	.XAN_WC3_DECODER = 1,
	.XL_DECODER = 1,
	.YOP_DECODER = 1,
	.ZLIB_DECODER = 1,
	.ZMBV_DECODER = 1,
	.WMT_AC3_DECODER = 1,
	.WMT_EAC3_DECODER = 1,
	.LIBMAD_MP1_DECODER = 1,
	.LIBMAD_MP2_DECODER = 1,
	.LIBMAD_MP3_DECODER = 1,
	.LIBFAAD_DECODER = 1,
	.AAC_DECODER = 1,
	.AC3_DECODER = 1,
	.ALAC_DECODER = 1,
	.ALS_DECODER = 1,
	.AMRNB_DECODER = 1,
	.APE_DECODER = 1,
	.ATRAC1_DECODER = 1,
	.ATRAC3_DECODER = 1,
	.BINKAUDIO_DCT_DECODER = 1,
	.BINKAUDIO_RDFT_DECODER = 1,
	.COOK_DECODER = 1,
	.DCA_DECODER = 1,
	.DSICINAUDIO_DECODER = 1,
	.EAC3_DECODER = 1,
	.FLAC_DECODER = 1,
	.GSM_DECODER = 1,
	.GSM_MS_DECODER = 1,
	.IMC_DECODER = 1,
	.MACE3_DECODER = 1,
	.MACE6_DECODER = 1,
	.MLP_DECODER = 1,
	.MP1_DECODER = 1,
	.MP1FLOAT_DECODER = 1,
	.MP2_DECODER = 1,
	.MP2FLOAT_DECODER = 1,
	.MP3_DECODER = 1,
	.MP3FLOAT_DECODER = 1,
	.MP3ADU_DECODER = 1,
	.MP3ADUFLOAT_DECODER = 1,
	.MP3ON4_DECODER = 1,
	.MP3ON4FLOAT_DECODER = 1,
	.MPC7_DECODER = 1,
	.MPC8_DECODER = 1,
	.NELLYMOSER_DECODER = 1,
	.QCELP_DECODER = 1,
	.QDM2_DECODER = 1,
	.RA_144_DECODER = 1,
	.RA_288_DECODER = 1,
	.SHORTEN_DECODER = 1,
	.SIPR_DECODER = 1,
	.SMACKAUD_DECODER = 1,
	.SONIC_DECODER = 1,
	.TRUEHD_DECODER = 1,
	.TRUESPEECH_DECODER = 1,
	.TTA_DECODER = 1,
	.TWINVQ_DECODER = 1,
	.VMDAUDIO_DECODER = 1,
	.VORBIS_DECODER = 1,
	.WAVPACK_DECODER = 1,
	.WMAPRO_DECODER = 1,
	.WMAV1_DECODER = 1,
	.WMAV2_DECODER = 1,
	.WMAVOICE_DECODER = 1,
	.WS_SND1_DECODER = 1,
	.PCM_ALAW_DECODER = 1,
	.PCM_BLURAY_DECODER = 1,
	.PCM_DVD_DECODER = 1,
	.PCM_F32BE_DECODER = 1,
	.PCM_F32LE_DECODER = 1,
	.PCM_F64BE_DECODER = 1,
	.PCM_F64LE_DECODER = 1,
	.PCM_MULAW_DECODER = 1,
	.PCM_S8_DECODER = 1,
	.PCM_S16BE_DECODER = 1,
	.PCM_S16LE_DECODER = 1,
	.PCM_S16LE_PLANAR_DECODER = 1,
	.PCM_S24BE_DECODER = 1,
	.PCM_S24DAUD_DECODER = 1,
	.PCM_S24LE_DECODER = 1,
	.PCM_S32BE_DECODER = 1,
	.PCM_S32LE_DECODER = 1,
	.PCM_U8_DECODER = 1,
	.PCM_U16BE_DECODER = 1,
	.PCM_U16LE_DECODER = 1,
	.PCM_U24BE_DECODER = 1,
	.PCM_U24LE_DECODER = 1,
	.PCM_U32BE_DECODER = 1,
	.PCM_U32LE_DECODER = 1,
	.PCM_ZORK_DECODER = 1,
	.INTERPLAY_DPCM_DECODER = 1,
	.ROQ_DPCM_DECODER = 1,
	.SOL_DPCM_DECODER = 1,
	.XAN_DPCM_DECODER = 1,
	.ADPCM_4XM_DECODER = 1,
	.ADPCM_ADX_DECODER = 1,
	.ADPCM_CT_DECODER = 1,
	.ADPCM_EA_DECODER = 1,
	.ADPCM_EA_MAXIS_XA_DECODER = 1,
	.ADPCM_EA_R1_DECODER = 1,
	.ADPCM_EA_R2_DECODER = 1,
	.ADPCM_EA_R3_DECODER = 1,
	.ADPCM_EA_XAS_DECODER = 1,
	.ADPCM_G726_DECODER = 1,
	.ADPCM_IMA_AMV_DECODER = 1,
	.ADPCM_IMA_DK3_DECODER = 1,
	.ADPCM_IMA_DK4_DECODER = 1,
	.ADPCM_IMA_EA_EACS_DECODER = 1,
	.ADPCM_IMA_EA_SEAD_DECODER = 1,
	.ADPCM_IMA_ISS_DECODER = 1,
	.ADPCM_IMA_QT_DECODER = 1,
	.ADPCM_IMA_SMJPEG_DECODER = 1,
	.ADPCM_IMA_WAV_DECODER = 1,
	.ADPCM_IMA_WS_DECODER = 1,
	.ADPCM_MS_DECODER = 1,
	.ADPCM_SBPRO_2_DECODER = 1,
	.ADPCM_SBPRO_3_DECODER = 1,
	.ADPCM_SBPRO_4_DECODER = 1,
	.ADPCM_SWF_DECODER = 1,
	.ADPCM_THP_DECODER = 1,
	.ADPCM_XA_DECODER = 1,
	.ADPCM_YAMAHA_DECODER = 1,
	.DVBSUB_DECODER = 1,
	.DVDSUB_DECODER = 1,
	.PGSSUB_DECODER = 1,
	.XSUB_DECODER = 1,
	.LIBDIRAC_DECODER = 1,
	.LIBGSM_DECODER = 1,
	.LIBGSM_MS_DECODER = 1,
	.LIBOPENCORE_AMRNB_DECODER = 1,
	.LIBOPENCORE_AMRWB_DECODER = 1,
	.LIBOPENJPEG_DECODER = 1,
	.LIBSCHROEDINGER_DECODER = 1,
	.LIBSPEEX_DECODER = 1,
	.LIBVPX_DECODER = 1,
};
#else
extern codec_conf PE_CODEC_CONF;
#endif
#endif
