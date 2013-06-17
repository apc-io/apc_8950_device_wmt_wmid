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
/*--- History ------------------------------------------------------------------- 
*
*  4.3.0 (2012-03-01): 
*       - abstract output device interface
*
*  4.2.1 (2012-02-01): 
*       - Http no section cross support
*       - Drop audio output with 4KB unit in realtime case
*
*  4.2.0 (2012-01-18): 
*       - AAC pass through support
*
*  4.1.0 (2011-12-20): 
*       - Add MMS and HLS(applehttp) support
*
*  4.0.0 (2011-12-08): 
*       - Android, DMR, and WiDi common play engine integration
*
*  3.0.0 (2011-06-12): 
*		- WiDi support
*       - State machine refine
*
*  2.0.0 (2011-01-18): 
*       - Improved performance and pefactory
*
*  1.0.0 (2010-12-17): 
*       - First formal release
*------------------------------------------------------------------------------*/

#define WMT_PE_VERSION     "4.3.0"
