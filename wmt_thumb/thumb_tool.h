#ifndef THUMB_TOOL__H
#define THUMB_TOOL__H

extern "C" {

#define INT64_C(v) int64_t(v ## ll)         //for AV_NOPTS_VALUE
#include "libavcodec/avcodec.h"

#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswscale/swscale.h"
}

enum OutputFormat {
    OUTPUT_FORMAT_RGB565,//just for a video file
    OUTPUT_FORMAT_YUV420P,//just for a video file
};

/**
  class used to hold result for VideoThumbTool
*/
class ThumbData{
public:
  	ThumbData(){
		memset(this,0,sizeof(ThumbData));
 	};
	
    int errorCode;  // reutrn EBUSY if can't get wmtlock in 2 seconds

    // All below members are meanful only when errorCode == 0
    int audioOnly;      // this file contans audio stream only

 	OutputFormat format;
	uint8_t *data[3];
	int lineSize[3];
	
	int displayWidth;//video's display width
	int displayHeight;//video's display height
	int thumbWidth; //video's thumb width
	int thumbHeight;//video's thumb height
	int level; // not use now 
	int duration; //media duration(audio or video)
 };

/**
  a tool used to decode video thumb, also you can use 
  it to decode a audio file 's duration
*/
class VideoThumbTool {
public:
	VideoThumbTool(const char* fileName,OutputFormat format);
	~VideoThumbTool();

	void setThumbSize(int w, int h);

    /**
     * @return ThumbData*  please check ThumbData.errorCode == 0 before use it
     */
    ThumbData * getThumbData(int getFrame);
private:
	//private internal used method 
	int decodeThumbData(int getFrame);
	int decodeVideoFrame(AVFormatContext * formatContext, int video_stream);
	//input 
	const char* mFileName;
	OutputFormat mFormat;
	ThumbData *mData;
	int mThumbWidth;
	int mThumbHeight;

    
	//private internal used to save scaled thumb,using the convenience of ffmpeg to alloc buffer
	AVFrame * mTmpAVFrame;
};


//For Video
#define VIDEO_THUMB_WIDTH   256
#define VIDEO_THUMB_HEIGHT  256

#endif
