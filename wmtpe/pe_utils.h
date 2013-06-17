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
#ifndef WMTPE_UTILS_H
#define WMTPE_UTILS_H

#if defined(__cplusplus)
extern "C" {
#endif

typedef int InterruptReq(ModuleContext *mc);

char *skip_fields(char *str, int count);
unsigned long fast_strtoul(char **endptr);
int64_t get_task_ticks(int fd);
void get_pe_ticks(PlayEngineContext *pec, int fd);
void get_module_ticks(ModuleContext *mc);
int dump_stream(const char *input, char *output);
int module_open(ModuleContext *mc);
void module_close(ModuleContext *mc);
void module_start(ModuleContext *mc);
void module_stop(ModuleContext *mc);
const char* module_name(int type);
const char* module_ste_name(ModuleContext *mc);
const char* pe_ste_name(PlayEngineContext *pec);
void packet_queue_list(PacketQueue *q, char *msg);
void packet_queue_link(PacketQueue *q1, PacketQueue *q2, int type);
void packet_queue_init(PacketQueue *q);
void packet_queue_flush(PacketQueue *q);
void packet_queue_end(PacketQueue *q);
void packet_queue_abort(PacketQueue *q);
int packet_queue_full(PacketQueue *q);
int packet_queue_put(PacketQueue *q, AVPacket *pkt);
int packet_queue_get(PacketQueue *q, AVPacket *pkt);
void print_progress(PlayEngineContext *pec);
void parse_stream_info(AVFormatContext *ic, StreamInfo *si);

#if defined(__cplusplus)
}
#endif

#endif /* WMTPE_UTILS_H */
