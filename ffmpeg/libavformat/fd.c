/*
 * File description io for ffmpeg system
 *
 * This file is a extensition for android setDataSource implementation
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "libavutil/avstring.h"
#include "avformat.h"
#include <fcntl.h>
#if HAVE_SETMODE
#include <io.h>
#endif
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include "os_support.h"

typedef struct FDContext {
	int fd;
	int64_t pos;
	int64_t offset;
	int64_t length;
} FDContext;

/* standard fd protocol for wplayer */
static int fd_open(URLContext *h, const char *filename, int flags)
{
	FDContext *fdc = h->priv_data;
	int fd = -1;
    
	/* refer to wplayer.cpp setDataSource function */
	if( 3 != sscanf(filename, "fd://%d,%lld,%lld", &fd, &fdc->offset, &fdc->length)) {
		av_log(NULL, AV_LOG_WARNING, "[FD] can not recognize. uri: %s\n", filename);
		return -1;
	}

	// dup it preventing original fd from disturbing
    fdc->fd = dup(fd);
    if(fdc->fd < 0){
		av_log(NULL, AV_LOG_WARNING, "[FD] fd %d can't dup. uri %s\n", fd, filename);
		return -1;
    }


    /* Note: must seek to right position(usually 0)  */
    if ((lseek64(fdc->fd, fdc->offset, SEEK_SET)) < 0){
		av_log(NULL, AV_LOG_WARNING, "[FD] seek to %lld fail\n", fdc->offset);
		return AVERROR(ENOENT);
    }
    fdc->pos = fdc->offset;

    return 0;
}

static int fd_read(URLContext *h, unsigned char *buf, int size)
{
	FDContext *fdc = h->priv_data;
	size_t len;
	int64_t left;

	left = fdc->length - (fdc->pos - fdc->offset);
	if(left <= 0)
		return 0;

	if((int64_t)size > left)
		size = (int)left;

	if((len = read(fdc->fd, buf, size)) > 0)
		fdc->pos += len;

    return len;
}

static int fd_write(URLContext *h, unsigned char *buf, int size)
{
	FDContext *fdc = h->priv_data;
    return write(fdc->fd, buf, size);
}

#define SEEK_SET 0  
#define SEEK_CUR 1  
#define SEEK_END 2  

/* XXX: use llseek */
static int64_t fd_seek(URLContext *h, int64_t pos, int whence)
{
	FDContext *fdc = h->priv_data;

    if(whence == AVSEEK_SIZE) 
		return fdc->length;

	if(whence == SEEK_CUR)
		pos += fdc->pos;
	else if(whence == SEEK_SET)
		pos += fdc->offset;
	else
		pos = fdc->offset + fdc->length;

	lseek64(fdc->fd, pos, whence);

	fdc->pos = pos - fdc->offset;
    return fdc->pos;
}

static int fd_close(URLContext *h)
{
	FDContext *fdc = h->priv_data;
	// duplicated fd no needs to close
	// coz it bounded to fdc->file when fdopen
	return close(fdc->fd);
}

static int fd_get_handle(URLContext *h)
{
	FDContext *fdc = h->priv_data;
    return fdc->fd;
}

URLProtocol fd_protocol = {
    "fd",
    fd_open,
    fd_read,
    fd_write,
    fd_seek,
    fd_close,
    .url_get_file_handle = fd_get_handle,
	.priv_data_size = sizeof(FDContext),
};
