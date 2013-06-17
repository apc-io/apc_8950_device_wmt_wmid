/*
 * Copyright (c) 2008-2011 WonderMedia Technologies, Inc. All Rights Reserved.
 *
 * This PROPRIETARY SOFTWARE is the property of WonderMedia Technologies, Inc.
 * and may contain trade secrets and/or other confidential information of 
 * WonderMedia Technologies, Inc. This file shall not be disclosed to any
 * third party, in whole or in part, without prior written consent of
 * WonderMedia.
 *
 * THIS PROPRIETARY SOFTWARE AND ANY RELATED DOCUMENTATION ARE PROVIDED
 * AS IS, WITH ALL FAULTS, AND WITHOUT WARRANTY OF ANY KIND EITHER EXPRESS
 * OR IMPLIED, AND WONDERMEDIA TECHNOLOGIES, INC. DISCLAIMS ALL EXPRESS
 * OR IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,
 * QUIET ENJOYMENT OR NON-INFRINGEMENT.
 */
#ifndef WMTPE_IF_H
#define WMTPE_IF_H

#include "pe_core.h"

#if defined(__cplusplus)
extern "C" {
#endif

PlayEngineContext *pe_init(uint32_t cfg, PeCallback callback, void *priv);
void pe_uninit(PlayEngineContext *pec);
/* type 0 - absolute time (value = sec), 1 - byte (value = bytes) */
int pe_seek(PlayEngineContext *pec, int type, float value);
int pe_loop(PlayEngineContext *pec, int loop);
int pe_play(PlayEngineContext *pec);
int pe_stop(PlayEngineContext *pec);
int pe_pause(PlayEngineContext *pec);
int pe_step(PlayEngineContext *pec);
int pe_open(PlayEngineContext *pec, const char *filename, int argc, char **argv);
void pe_close(PlayEngineContext *pec);
void pe_status(PlayEngineContext *pec);
int pe_voconfig(PlayEngineContext *pec, PEVideoOutPath path, PEHDMIMode hdmi, pe_view_config *view);
int pe_register_audio_device(PlayEngineContext *pec, audio_output_device *dev);
int pe_register_video_device(PlayEngineContext *pec, video_output_device *dev);
int pe_register_subtitle_device(PlayEngineContext *pec, video_output_device *dev);

#if defined(__cplusplus)
}
#endif

#endif /* WMTPE_IF_H */ 
