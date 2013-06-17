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
#ifndef WMT_WPLAYER_H_INCLUDED
#define WMT_WPLAYER_H_INCLUDED

#include <utils/KeyedVector.h>
#include <utils/String8.h>
#include <media/MediaPlayerInterface.h>
#include <media/AudioTrack.h>
#include <utils/Errors.h>
#include <gui/ISurfaceTexture.h>
#include <surfaceflinger/ISurface.h>
#include <surfaceflinger/Surface.h>

#include "pe_if.h"
#include "audio_output.h"
#include "video_output.h"

//Note: must same as MovieViewControl.java file, for report network input underflow
#define MEDIA_INFO_INPUT_UNDERFLOW   0xFF000001
#define MEDIA_HDMI_HOTPLUG_EVENT     0xFF000003
#define INVOKE_SET_VIDEO_OUT_ZOOM    0x0FF21001      
#define INVOKE_GET_VIDEO_OUT_ZOOM    0x0FF21002
#define INVOKE_SET_VIDEO_OUT_HDMI    0x0FF22001
#define INVOKE_GET_VIDEO_OUT_HDMI    0x0FF22002
#define INVOKE_SET_PE_STATUS         0x0FF22003
#define INVOKE_SET_PE_STEP           0x0FF22004
#define HDMI_PLUG_OUT                0
#define HDMI_PLUG_IN                 1

class WPLAYER: public android::MediaPlayerInterface 
{
public:
    static bool capCheck(const char* url);
    WPLAYER();
    virtual ~WPLAYER();
    virtual android::status_t    setDataSource(
            const char *url, const android::KeyedVector<android::String8, android::String8> *headers);
    virtual android::status_t    setDataSource(int fd, int64_t offset, int64_t length);
    virtual void                 setAudioSink(const android::sp<AudioSink>& audioSink);
    virtual android::status_t    initCheck();
    virtual android::status_t    setVideoSurfaceTexture(const android::sp<android::ISurfaceTexture>& surfaceTexture);
    virtual android::status_t    prepare();
    virtual android::status_t    prepareAsync();
    virtual android::status_t    start();
    virtual android::status_t    stop();
    virtual android::status_t    pause();
    virtual bool                 isPlaying();
    virtual android::status_t    seekTo(int msec);
    virtual android::status_t    getCurrentPosition(int *msec);
    virtual android::status_t    getDuration(int *msec);
    virtual android::status_t    reset();
    virtual android::status_t    setLooping(int loop);
    virtual android::player_type playerType();
    virtual android::status_t    invoke(const android::Parcel& request, android::Parcel *reply);
    virtual android::status_t    getMetadata(const android::media::Metadata::Filter& ids, android::Parcel *records);
    virtual android::status_t    setParameter(int key, const android::Parcel &request);
    virtual android::status_t    getParameter(int key, android::Parcel *reply);

    void  fireEvent(int msg, int ext1=0, int ext2=0);

	android::sp<ANativeWindow>     mNativeWindow;
	android::Mutex      mMutex;
	int                 mResult;
	int                 mSeekTime;
	int                 mAsyncOperation;
	int	                mUid;
	int             	mDebug;
	int                 mHttpURL;
    android::Condition  mAsyncOperationCondition;

private:
    android::status_t    setDataSource_l(const char *url, const char *argument);
    android::status_t    prepare_l();
	android::status_t    reset_l();
	android::status_t    start_l();

    enum {
        PLAYING             = 1,
        LOOPING             = 2,
        PREPARING           = 4,
        PREPARED            = 8,
    };

    PlayEngineContext   *mPec; // PlayEngine implementation pointer
    android::String8    mUrl;
	android::String8    mArgs;
	android::String8    mCookie;
    android::String8    mDebugArgs;
    int32_t             mFd;   // dup datasource handle
	int32_t             mPercent; // buffered percentage, only for network
	uint32_t            mState;
	PEVideoOutPath      mPath;

	audio_output_device	mAoDev;
	video_output_device	mVoDev;
};

#endif
