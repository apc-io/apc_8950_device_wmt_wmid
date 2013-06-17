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
#ifndef WPLAYER_AUDIO_OUTPUT_H_INCLUDED
#define WPLAYER_AUDIO_OUTPUT_H_INCLUDED

#if defined(__cplusplus)
extern "C" {
#endif

#include "pe_ao.h"

int wp_audio_init(pe_ao_ctx *ctx, void *priv, char *arg);
void wp_audio_uninit(pe_ao_ctx *ctx, int immed);
void wp_audio_reset(pe_ao_ctx *ctx);
void wp_audio_pause(pe_ao_ctx *ctx, int enable);
int wp_audio_control(pe_ao_ctx *ctx, int cmd, void *data);
int wp_audio_play(pe_ao_ctx *ctx, void* data, int len);
float wp_audio_get_delay(pe_ao_ctx *ctx);
int wp_audio_get_space(pe_ao_ctx *ctx);

#if defined(__cplusplus)
}
#endif
#endif
