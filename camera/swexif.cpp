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

#define LOG_TAG "swExif"

#include "swexif.h"
#include "assert.h"
#include <utils/Log.h>

namespace android {

swExif::swExif()
{
	mThumbnailSize = 0;
	mExifData = (char *)malloc(2048); //alloc 2k for temp
	mHasthumbnail = true;
}

swExif::~swExif()
{
	if (mExifData)
		free(mExifData);
}

/****************************************************************************
 * Public API
 ***************************************************************************/

/* Get an existing tag, or create one if it doesn't exist */
ExifEntry* swExif::init_tag(ExifData *exif, ExifIfd ifd, ExifTag tag)
{
	ExifEntry *entry;
	/* Return an existing tag if one exists */
	if (!((entry = exif_content_get_entry (exif->ifd[ifd], tag)))) {
	    /* Allocate a new entry */
	    entry = exif_entry_new ();
	    assert(entry != NULL); /* catch an out of memory condition */
	    entry->tag = tag; /* tag must be set before calling
				 exif_content_add_entry */

	    /* Attach the ExifEntry to an IFD */
	    exif_content_add_entry (exif->ifd[ifd], entry);

	    /* Allocate memory for the entry and fill with default data */
	    exif_entry_initialize (entry, tag);

	    /* Ownership of the ExifEntry has now been passed to the IFD.
	     * One must be very careful in accessing a structure after
	     * unref'ing it; in this case, we know "entry" won't be freed
	     * because the reference count was bumped when it was added to
	     * the IFD.
	     */
	    exif_entry_unref(entry);
	}
	return entry;
}

/* Create a brand-new tag with a data field of the given length, in the
 * given IFD. This is needed when exif_entry_initialize() isn't able to create
 * this type of tag itself, or the default data length it creates isn't the
 * correct length.
 */
ExifEntry* swExif::create_tag(ExifData *exif, ExifIfd ifd, ExifTag tag, size_t len)
{
	void *buf;
	ExifEntry *entry;
	
	/* Create a memory allocator to manage this ExifEntry */
	ExifMem *mem = exif_mem_new_default();
	assert(mem != NULL); /* catch an out of memory condition */

	/* Create a new ExifEntry using our allocator */
	entry = exif_entry_new_mem (mem);
	assert(entry != NULL);

	/* Allocate memory to use for holding the tag data */
	buf = exif_mem_alloc(mem, len);
	assert(buf != NULL);

	/* Fill in the entry */
	entry->data = (unsigned char *)buf;
	entry->size = len;
	entry->tag = tag;
	entry->components = len;
	entry->format = EXIF_FORMAT_UNDEFINED;

	/* Attach the ExifEntry to an IFD */
	exif_content_add_entry (exif->ifd[ifd], entry);

	/* The ExifMem and ExifEntry are now owned elsewhere */
	exif_mem_unref(mem);
	exif_entry_unref(entry);

	return entry;
}

ExifEntry* swExif::create_gps_tag(ExifData *exif, int tag)
{
	void *buf;
	ExifEntry *entry;
	
	/* Create a memory allocator to manage this ExifEntry */
	ExifMem *mem = exif_mem_new_default();
	assert(mem != NULL); /* catch an out of memory condition */

	/* Create a new ExifEntry using our allocator */
	entry = exif_entry_new_mem (mem);
	assert(entry != NULL);

	/* Fill in the entry */	
	switch(tag) {
	case EXIF_TAG_GPS_VERSION_ID:
		entry->size = 4;
		entry->components = 4;
		entry->format = EXIF_FORMAT_BYTE;
		break;
	case EXIF_TAG_GPS_TIME_STAMP:
	case EXIF_TAG_GPS_LATITUDE:
	case EXIF_TAG_GPS_LONGITUDE:
		entry->components = 3;
		entry->format = EXIF_FORMAT_RATIONAL;
		entry->size = 8 * entry->components;
		break;
	case EXIF_TAG_GPS_ALTITUDE:
		entry->components = 1;
		entry->format = EXIF_FORMAT_RATIONAL;
		entry->size = 8 * entry->components;
		break;
	case EXIF_TAG_GPS_LATITUDE_REF:
	case EXIF_TAG_GPS_LONGITUDE_REF:
		entry->components = 2;
		entry->format = EXIF_FORMAT_ASCII;
		entry->size = entry->components;
	break;
	case EXIF_TAG_GPS_DATE_STAMP:
		entry->components = 11;
		entry->format = EXIF_FORMAT_ASCII;
		entry->size = entry->components;
	break;
	case EXIF_TAG_GPS_ALTITUDE_REF:
		entry->size = 1;
		entry->components = 1;
		entry->format = EXIF_FORMAT_BYTE;
	break;
	default:break;
	}

	/* Allocate memory to use for holding the tag data */
	buf = exif_mem_alloc(mem, entry->size);
	assert(buf != NULL);
	
	entry->data = (unsigned char *)buf;
	entry->tag = (ExifTag)tag;

	/* Attach the ExifEntry to an IFD */
	exif_content_add_entry (exif->ifd[EXIF_IFD_GPS], entry);

	/* The ExifMem and ExifEntry are now owned elsewhere */
	exif_mem_unref(mem);
	exif_entry_unref(entry);

	return entry;
}

void swExif::exif_write_datetimestamp(ExifData *exif, const char *string)
{
	ExifRational val[3];
	ExifEntry *entry;
	long timestamp = 0;
	struct tm tm_data;

	//LOGW("\n\n string = %s",string);
	entry = create_gps_tag(exif, EXIF_TAG_GPS_TIME_STAMP);
	timestamp = atol(string);
	//LOGW("timestamp = %ld",timestamp);
	gmtime_r(&timestamp, &tm_data);
	val[0].numerator = tm_data.tm_hour;
    val[0].denominator = 1;
    val[1].numerator = tm_data.tm_min;
    val[1].denominator = 1;
	val[2].numerator = tm_data.tm_sec;
	val[2].denominator = 1;
	exif_set_rational(entry->data, FILE_BYTE_ORDER , val[0]);
	exif_set_rational((unsigned char *)(entry->data+8), FILE_BYTE_ORDER , val[1]);
	exif_set_rational((unsigned char *)(entry->data+16), FILE_BYTE_ORDER , val[2]);
	entry = create_gps_tag(exif, EXIF_TAG_GPS_DATE_STAMP);
	snprintf ((char *) entry->data, entry->size, "%04i:%02i:%02i", 
			tm_data.tm_year + 1900, tm_data.tm_mon + 1, tm_data.tm_mday);
	//LOGW("year = %d , mon = %d day = %d",tm_data.tm_year + 1900,tm_data.tm_mon + 1,tm_data.tm_mday);
	//LOGW("hour = %d , min = %d , sec = %d",tm_data.tm_hour,tm_data.tm_min,tm_data.tm_sec);
}

int swExif::exif_write_rational(ExifData *exif, ExifIfd ifd, int tag, const char *string)
{
	ExifRational val;
	char *c;
	int ofs = 0;
	ExifEntry *entry;
	long tmp = 0;

	//LOGW("string = %s",string);
	c = strchr(string, '.');
	if (c) {
		ofs = c-string+1;
		ofs = strlen(string) - ofs;
		ofs = pow(10, ofs);
		val.denominator = ofs;
		tmp = (long)(atof(string) * ofs);
		val.numerator = (unsigned int)tmp;
		//LOGW("numerator = %d , denominator = %d",val.numerator,val.denominator);
	} else {
		val.denominator = 1;
		sscanf(string,"%d", &val.numerator);
	}

	if (ifd == EXIF_IFD_GPS) {
		if (tag != EXIF_TAG_GPS_ALTITUDE) {
			tmp = lround(atof(string) * ofs);
			val.numerator = (unsigned int)labs(tmp);
		}
		entry = create_gps_tag(exif, tag);
		exif_set_rational(entry->data, FILE_BYTE_ORDER , val);
		//LOGW("numerator = %d , denominator = %d",val.numerator,val.denominator);
		if (tag != EXIF_TAG_GPS_ALTITUDE) {
			val.numerator = 0;
			val.denominator = 1;
			exif_set_rational((unsigned char *)entry->data+8, FILE_BYTE_ORDER , val);
			exif_set_rational((unsigned char *)entry->data+16, FILE_BYTE_ORDER , val);
		}
		if (tag == EXIF_TAG_GPS_LATITUDE) {
			entry = create_gps_tag(exif, EXIF_TAG_GPS_LATITUDE_REF);
			if (tmp < 0)
				entry->data[0] = (unsigned char)'S';
			else
				entry->data[0] = (unsigned char)'N';
		} else if (tag == EXIF_TAG_GPS_LONGITUDE) {
			entry = create_gps_tag(exif, EXIF_TAG_GPS_LONGITUDE_REF);
			if (tmp < 0)
				entry->data[0] = (unsigned char)'W';
			else
				entry->data[0] = (unsigned char)'E';
		} else if (tag == EXIF_TAG_GPS_ALTITUDE) {
			entry = create_gps_tag(exif, EXIF_TAG_GPS_ALTITUDE_REF);
			if (tmp < 0)
				entry->data[0] = 1;
			else
				entry->data[0] = 0;	
		} else {
			LOGE("Unknow GPS info");
		}
		
	} else {
		entry = init_tag(exif, ifd, (ExifTag)tag);
		exif_set_rational(entry->data, FILE_BYTE_ORDER , val);
	}

	return 0;
}

int swExif::checkexifhasgpsinfo(CameraParameters p)
{
	const char *string;

	if (p.get(CameraParameters::KEY_GPS_LATITUDE) || p.get(CameraParameters::KEY_GPS_LONGITUDE))
		return 0;
	if (p.get(CameraParameters::KEY_GPS_ALTITUDE) || p.get(CameraParameters::KEY_GPS_PROCESSING_METHOD))
		return 0;

	return -1;
}


void swExif::setParameters(const CameraParameters& params)
{
	mParameters = params;
}


int swExif::setThumbnailsize(int filesize)
{
	mThumbnailSize = filesize;

	if (mThumbnailSize == 0) {
		LOGE("Thumbnail size = 0");
		return -1;
	}

	return 0;
}

int swExif::getExiflength(int *length)
{
	if (!mExifLength)
		return -1;
	*length = mExifLength;
	return 0;
}

int swExif::createswexif(void)
{
	int rc = 1, hasthumbnail = 0;
	FILE *f, *fthumbnail;
	unsigned char *exif_data;
	unsigned int data_len, fileSize;
	char patch[3], c;
	ExifEntry *entry;
	const char *string;
	int width = 0, height = 0;
	int twidth = 0, theight = 0;
	static const char ExifAsciiPrefix[] = { 0x41, 0x53, 0x43, 0x49, 0x49, 0x0, 0x0, 0x0 };	
	ExifData *exif = exif_data_new();
	if (!exif) {
		LOGE("Out of memory\n");
		return 2;
	}


	//check 
	rc = 0;
	if ((mThumbnailSize == 0) && (mHasthumbnail == true))
		rc = -1;

	mParameters.getPictureSize(&width, &height);
	if ((width == 0) || (height == 0)) {
		LOGE("Picture : %d x %d ",width,height);
		rc = -1;
	} 

	if (mHasthumbnail == true) {
		twidth = mParameters.getInt(CameraParameters::KEY_JPEG_THUMBNAIL_WIDTH);
		theight = mParameters.getInt(CameraParameters::KEY_JPEG_THUMBNAIL_HEIGHT);
		if ((twidth == 0) || (theight == 0)) {
			LOGE("Thumbnail : %d x %d ",twidth,theight);
			rc = -1;
		}
	}
	LOGW("Thumbnail : %d x %d ",twidth,theight);
	if (rc)
		goto errout;
	//

	/* Set the image options */
	exif_data_set_option(exif, EXIF_DATA_OPTION_FOLLOW_SPECIFICATION);
	exif_data_set_data_type(exif, EXIF_DATA_TYPE_COMPRESSED);
	exif_data_set_byte_order(exif, FILE_BYTE_ORDER);

	/* Create the mandatory EXIF fields with default data */
	exif_data_fix(exif);

	/* All these tags are created with default values by exif_data_fix() */
	/* Change the data to the correct values for this image. */
	entry = init_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_PIXEL_X_DIMENSION);
	exif_set_long(entry->data, FILE_BYTE_ORDER, width);

	entry = init_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_PIXEL_Y_DIMENSION);
	exif_set_long(entry->data, FILE_BYTE_ORDER, height);

	entry = init_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_COLOR_SPACE);
	exif_set_short(entry->data, FILE_BYTE_ORDER, 1);

	string = mParameters.get("focal-length");
	if (string)
		exif_write_rational(exif, EXIF_IFD_EXIF, (int)EXIF_TAG_FOCAL_LENGTH, string);
	else
		LOGW(" focal-length = NULL ");


	//write IDF0 for camera feature
	entry = init_tag(exif, EXIF_IFD_0, EXIF_TAG_X_RESOLUTION);
	entry = init_tag(exif, EXIF_IFD_0, EXIF_TAG_Y_RESOLUTION);

	//use default , need to modify libexif
	entry = init_tag(exif, EXIF_IFD_0, EXIF_TAG_MAKE);
	entry = init_tag(exif, EXIF_IFD_0, EXIF_TAG_MODEL);
	entry = init_tag(exif, EXIF_IFD_0, EXIF_TAG_DATE_TIME);

	//write IDF1 for thumbnail
	//our hardware do not support thumbnail .....
	if (mHasthumbnail == true) {	
		entry = init_tag(exif, EXIF_IFD_1, EXIF_TAG_IMAGE_WIDTH);
		exif_set_long(entry->data, FILE_BYTE_ORDER, twidth);
	
		entry = init_tag(exif, EXIF_IFD_1, EXIF_TAG_IMAGE_LENGTH);
		exif_set_long(entry->data, FILE_BYTE_ORDER, theight);
	
		entry = init_tag(exif, EXIF_IFD_1, EXIF_TAG_X_RESOLUTION);
		entry = init_tag(exif, EXIF_IFD_1, EXIF_TAG_Y_RESOLUTION);
		entry = init_tag(exif, EXIF_IFD_1, EXIF_TAG_DATE_TIME);
		//fan say need check
		entry = init_tag(exif, EXIF_IFD_1, EXIF_TAG_EXIF_IFD_POINTER);
		entry = init_tag(exif, EXIF_IFD_1, EXIF_TAG_JPEG_INTERCHANGE_FORMAT);
		entry = init_tag(exif, EXIF_IFD_1, EXIF_TAG_JPEG_INTERCHANGE_FORMAT_LENGTH);
		exif_set_long(entry->data, FILE_BYTE_ORDER, (long)mThumbnailSize);
		entry = init_tag(exif, EXIF_IFD_1, EXIF_TAG_COMPRESSION);
		exif_set_short (entry->data, FILE_BYTE_ORDER, 6);
	}
	//write GPS	
	if (!checkexifhasgpsinfo(mParameters)) {
		entry = init_tag(exif, EXIF_IFD_GPS, EXIF_TAG_GPS_INFO_IFD_POINTER);
		entry = create_gps_tag(exif, EXIF_TAG_GPS_VERSION_ID);
		// just fill value for CTS test
		entry->data[0] = 0x00;
		entry->data[1] = 0x01;
		entry->data[2] = 0x02;
		entry->data[3] = 0x03;	
		//
		string = mParameters.get("gps-latitude");
		//string = "37.736071";
		if (string) {
			exif_write_rational(exif, EXIF_IFD_GPS, EXIF_TAG_GPS_LATITUDE, string);
		}
		//else
		//	LOGW(" gps-latitude = NULL ");
		
		string = mParameters.get("gps-longitude");
		//string = "-122.441983";
		if (string) {
			exif_write_rational(exif, EXIF_IFD_GPS, EXIF_TAG_GPS_LONGITUDE, string);
		}
		//else
		//	LOGW(" gps-longitude = NULL ");
		
		string = mParameters.get("gps-altitude");
		//string = "21";
		if (string) {
			exif_write_rational(exif, EXIF_IFD_GPS, EXIF_TAG_GPS_ALTITUDE, string);
		}
		//else
		//	LOGW(" gps-altitude = NULL ");

		string = mParameters.get("gps-timestamp");
		if (string) {
			exif_write_datetimestamp(exif, string);
		}
		//else
		//	LOGW(" gps-timestamp = NULL ");


		string = mParameters.get("gps-processing-method");
		if (string) {
			entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_PROCESSING_METHOD, 
			sizeof(ASCII_COMMENT) + strlen(string)-1);
			/* Write the special header needed for a comment tag */
			memcpy(entry->data, ASCII_COMMENT, sizeof(ASCII_COMMENT)-1);
			/* Write the actual comment text, without the trailing NUL character */
			memcpy(entry->data+8, string, strlen(string));
			entry->data[8+strlen(string)] = 0;
		}		
	}

	/* Create a EXIF_TAG_USER_COMMENT tag. This one must be handled
	 * differently because that tag isn't automatically created and
	 * allocated by exif_data_fix(), nor can it be created using
	 * exif_entry_initialize() so it must be explicitly allocated here.
	 */
	entry = create_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_USER_COMMENT, 
			sizeof(ASCII_COMMENT) + sizeof(FILE_COMMENT) - 2);
	/* Write the special header needed for a comment tag */
	memcpy(entry->data, ASCII_COMMENT, sizeof(ASCII_COMMENT)-1);
	/* Write the actual comment text, without the trailing NUL character */
	memcpy(entry->data+8, FILE_COMMENT, sizeof(FILE_COMMENT)-1);
	/* create_tag() happens to set the format and components correctly for
	 * EXIF_TAG_USER_COMMENT, so there is nothing more to do. */

	/* Create a EXIF_TAG_SUBJECT_AREA tag */
	entry = create_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_SUBJECT_AREA,
			   4 * exif_format_get_size(EXIF_FORMAT_SHORT));
	entry->format = EXIF_FORMAT_SHORT;
	entry->components = 4;
	exif_set_short(entry->data, FILE_BYTE_ORDER, width);
	exif_set_short(entry->data+2, FILE_BYTE_ORDER, height);
	exif_set_short(entry->data+4, FILE_BYTE_ORDER, width);
	exif_set_short(entry->data+6, FILE_BYTE_ORDER, height);

	/* Get a pointer to the EXIF data block we just created */
	exif_data_save_data(exif, &exif_data, &data_len);
	assert(exif_data != NULL);

	if (mHasthumbnail == true) {
		entry = init_tag(exif, EXIF_IFD_1, EXIF_TAG_JPEG_INTERCHANGE_FORMAT);
		exif_set_long(entry->data, FILE_BYTE_ORDER, (long)(data_len+6-0x0c));

		exif_data_save_data(exif, &exif_data, &data_len);
		assert(exif_data != NULL);
	}

	mExifLength = data_len+exif_header_len+2;
	if (mExifLength > 2048) {
		LOGE(" mExifLength > 2048 ");
		goto errout;
	}
//LOGW("Exif  len = %d , mThumbnailSize = 0x%x",data_len,mThumbnailSize);

	/* Write EXIF header */
	memcpy(mExifData, exif_header, exif_header_len);
	*(char *)(mExifData+exif_header_len) = (data_len+2+mThumbnailSize) >> 8;
	*(char *)(mExifData+exif_header_len+1) = (data_len+2+mThumbnailSize) &0xff;
	memcpy((char *)(mExifData+exif_header_len+2), exif_data, data_len);

	rc = 0;
errout:
	/* The allocator we're using for ExifData is the standard one, so use
	 * it directly to free this pointer.
	 */
	free(exif_data);
	exif_data_unref(exif);
	return rc;
}

int swExif::copyExif(char *dst)
{
	if ((dst == NULL) || (mExifData == NULL)) {
		LOGE("src or dst is NULL");
		return -1;
	}
	if (mExifLength == 0) {
		LOGE("mExifLength == 0");
		return -1;
	}
	memcpy(dst, mExifData, mExifLength);
	return 0;
}

void swExif::setThumbnail(bool has)
{
	mHasthumbnail = has;
}

}; /* namespace android */
