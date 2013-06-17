/*
 *
 * Copyright (c) 2001 Lutz Mueller <lutz@users.sourceforge.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301  USA.
 */
 
#ifndef ANDROID_SW_EXIF_H
#define ANDROID_SW_EXIF_H

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <libexif/exif-data.h>
#include <camera/CameraParameters.h>


/* raw EXIF header data */
static const unsigned char exif_header[] = {
  0xff, 0xd8, 0xff, 0xe1
};
/* length of data in exif_header */
static const unsigned int exif_header_len = sizeof(exif_header);

/* byte order to use in the EXIF block */
#define FILE_BYTE_ORDER EXIF_BYTE_ORDER_INTEL

/* comment to write into the EXIF block */
#define FILE_COMMENT "libexif demonstration image"

/* special header required for EXIF_TAG_USER_COMMENT */
#define ASCII_COMMENT "ASCII\0\0\0"

namespace android {

class swExif {

public:
    swExif();
    ~swExif();
	ExifEntry* init_tag(ExifData *exif, ExifIfd ifd, ExifTag tag);
	ExifEntry* create_tag(ExifData *exif, ExifIfd ifd, ExifTag tag, size_t len);
	ExifEntry* create_gps_tag(ExifData *exif, int tag);
	int exif_write_rational(ExifData *exif, ExifIfd ifd, int tag, const char *string);
	int checkexifhasgpsinfo(CameraParameters p);
	int createswexif(void);
	void SetCameraParameters(const CameraParameters& params);
	void setParameters(const CameraParameters& params);
	int setThumbnailsize(int filesize);
	void setHeader(char *header);
	int getExiflength(int *length);
	int copyExif(char *dst);
	void exif_write_datetimestamp(ExifData *exif, const char *string);
	void setThumbnail(bool has);
	void removeheader(int id, char *buffer);

private:
	CameraParameters mParameters;
	int mThumbnailSize;
	int mExifLength;
	char *mExifData;
	bool mHasthumbnail;
};

}; ///* namespace android */
#endif  // ANDROID_SW_EXIF_H



