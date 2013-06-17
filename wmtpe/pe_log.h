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
#ifndef WMTPE_LOG_H
#define WMTPE_LOG_H
#include <stdarg.h>

#define PE_LOG_FLAG_TIME   0x1
#define PE_LOG_FLAG_ADDR   0x2
#define PE_LOG_FLAG_REPEAT 0x4

int pe_log_get_level(void);
void pe_log_set_level(int level);
void pe_log_config(int flag);
void pe_log_init(int level, int flag);
void pe_log(int level, const char* fmt, ...);

#endif /* WMTPE_LOG_H */
