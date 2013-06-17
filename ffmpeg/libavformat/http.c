/*
 * HTTP protocol for ffmpeg client
 * Copyright (c) 2000, 2001 Fabrice Bellard
 *
 * This file is part of FFmpeg.
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
#include <unistd.h>
#include <strings.h>
#include "internal.h"
#include "network.h"
#include "http.h"
#include "os_support.h"
#include "httpauth.h"
#include "libavcodec/opt.h"

/* XXX: POST protocol is not completely implemented because ffmpeg uses
   only a subset of it. */
/* message macro for http handshaking */
#define hsprint(show, fmt, args...) \
	if(show) \
		av_log(NULL, AV_LOG_INFO, fmt, ## args)

/* used for protocol handling */
#define BUFFER_SIZE 1024
#define URL_SIZE    4096
#define MAX_REDIRECTS 8

typedef struct {
    const AVClass *class;
    URLContext *hd;
    unsigned char *buffer, *buf_ptr, *buf_end;
    int line_count;
    int http_code;
    int64_t chunksize;      /**< Used if "Transfer-Encoding: chunked" otherwise -1. */
    int64_t off, filesize;
    char location[URL_SIZE];
    HTTPAuthState auth_state;
    unsigned char headers[BUFFER_SIZE];
	// range type: -1 - init, 0 - none, 1 - bytes
	int accept_range; // if accept_range == 1(bytes), than give range tag in http request
	int stream_uri; // uri is stream-style, i.e, withou sub filename, 1 - steam, 0 - file
	int redo; // if accept_range == 1(bytes), than give range tag in http request
	int show_link;
	int no_section_cross;
	char *cookie;
} HTTPContext;

#define OFFSET(x) offsetof(HTTPContext, x)
static const AVOption options[] = {
{"chunksize", "use chunked transfer-encoding for posts, -1 disables it, 0 enables it", OFFSET(chunksize), FF_OPT_TYPE_INT64, 0, -1, 0 }, /* Default to 0, for chunked POSTs */
{NULL}
};
static const AVClass httpcontext_class = {
    "HTTP", av_default_item_name, options, LIBAVUTIL_VERSION_INT
};

static char *http_cookie = NULL;
static int buffer_size = 4096;
static int http_connect(URLContext *h, const char *path, const char *hoststr,
                        const char *auth, int *new_location);

void ff_http_set_cookie(const char *cookie)
{
	av_freep(&http_cookie);
	if(cookie && strlen(cookie)){
		http_cookie = av_malloc(strlen(cookie)+1);
		av_strlcpy(http_cookie,cookie,strlen(cookie)+1);
		av_log(NULL, AV_LOG_DEBUG, "Seting Http cookie \"%s\"\n",http_cookie);
	}
}

void ff_http_set_headers(URLContext *h, const char *headers)
{
    HTTPContext *s = h->priv_data;
    int len = strlen(headers);

    if (len && strcmp("\r\n", headers + len - 2))
        av_log(NULL, AV_LOG_ERROR, "No trailing CRLF found in HTTP header.\n");

    av_strlcpy(s->headers, headers, sizeof(s->headers));
}

void ff_http_set_chunked_transfer_encoding(URLContext *h, int is_chunked)
{
    ((HTTPContext*)h->priv_data)->chunksize = is_chunked ? 0 : -1;
}

void ff_http_init_auth_state(URLContext *dest, const URLContext *src)
{
    memcpy(&((HTTPContext*)dest->priv_data)->auth_state,
           &((HTTPContext*)src->priv_data)->auth_state, sizeof(HTTPAuthState));
}

/* return non zero if error */
static int http_open_cnx(URLContext *h)
{
    const char *path, *proxy_path;
    char hostname[1024], hoststr[1024];
    char auth[1024];
    char path1[1024];
    char buf[1024];
    int port, use_proxy, err, location_changed = 0, redirects = 0;
    HTTPAuthType cur_auth_type;
    HTTPContext *s = h->priv_data;
    URLContext *hd = NULL;

    proxy_path = getenv("http_proxy");
    use_proxy = (proxy_path != NULL) && !getenv("no_proxy") &&
        av_strstart(proxy_path, "http://", NULL);

    /* fill the dest addr */
 redo:
    /* needed in any case to build the host string */
    av_url_split(NULL, 0, auth, sizeof(auth), hostname, sizeof(hostname), &port,
                 path1, sizeof(path1), s->location);
    ff_url_join(hoststr, sizeof(hoststr), NULL, NULL, hostname, port, NULL);

    if (use_proxy) {
        av_url_split(NULL, 0, auth, sizeof(auth), hostname, sizeof(hostname), &port,
                     NULL, 0, proxy_path);
        path = s->location;
    } else {
        if (path1[0] == '\0')
            path = "/";
        else
            path = path1;
    }
    if (port < 0)
        port = 80;

    ff_url_join(buf, sizeof(buf), "tcp", NULL, hostname, port, NULL);
    err = url_open(&hd, buf, URL_RDWR);
    if (err < 0)
        goto fail;

    s->hd = hd;
    cur_auth_type = s->auth_state.auth_type;
    if (http_connect(h, path, hoststr, auth, &location_changed) < 0)
        goto fail;
    if (s->http_code == 401) {
        if (cur_auth_type == HTTP_AUTH_NONE && s->auth_state.auth_type != HTTP_AUTH_NONE) {
            url_close(hd);
            goto redo;
        } else
            goto fail;
    }
    if (((s->http_code == 302 || s->http_code == 303) && location_changed == 1) || s->redo){
		s->redo = 0;
        /* url moved, get next */
        url_close(hd);
        if (redirects++ >= MAX_REDIRECTS)
            return AVERROR(EIO);
        location_changed = 0;
        goto redo;
    }
    return 0;
 fail:
    if (hd)
        url_close(hd);
    s->hd = NULL;
    return AVERROR(EIO);
}

static int http_open(URLContext *h, const char *uri, int flags)
{
    HTTPContext *s = h->priv_data;

    h->is_streamed = 1;

    s->filesize = -1;
	s->accept_range = -1;
	s->stream_uri = 1;
	if(uri){
		const char *ptr, *end = uri;
		end += strlen(uri);
		if((ptr = strrchr(uri,'.')) != NULL){
			ptr++; // skip '.'
			for(;(ptr < end) && !(ispunct(*ptr)); ptr++);
			if(ptr >= end) // there is no special char in subfilename. so we make sure this is a file not a stream
				s->stream_uri = 0;
		}
	}

	av_log(NULL,AV_LOG_INFO,"HTTP: %s\n",(s->stream_uri)?"STREAM URI":"FILE URI");
	if(s->stream_uri && !(h->flags & URL_HTTP_NORANGE)){
		av_log(NULL,AV_LOG_INFO,"%s stream uri without setting norange, force to file uri\n",__func__);
		s->stream_uri = 0;
	}
	if(h->flags & URL_HTTP_HANDSHAKE)
		s->show_link = 1;
	if(h->flags & URL_HTTP_NO_SECTION_CROSS)
		s->no_section_cross = 1;
    av_strlcpy(s->location, uri, URL_SIZE);
	s->cookie = http_cookie;
	http_cookie = NULL;

    return http_open_cnx(h);
}

static int http_getc(HTTPContext *s)
{
    int len;
    if (s->buf_ptr >= s->buf_end) {
        len = url_read(s->hd, s->buffer, BUFFER_SIZE);
        if (len < 0) {
            return AVERROR(EIO);
        } else if (len == 0) {
            return -1;
        } else {
            s->buf_ptr = s->buffer;
            s->buf_end = s->buffer + len;
        }
    }
    return *s->buf_ptr++;
}

static int http_get_line(HTTPContext *s, char *line, int line_size)
{
    int ch;
    char *q;

    q = line;
    for(;;) {
        ch = http_getc(s);
        if (ch < 0)
            return AVERROR(EIO);
        if (ch == '\n') {
            /* process line */
            if (q > line && q[-1] == '\r')
                q--;
            *q = '\0';

            return 0;
        } else {
            if ((q - line) < line_size - 1)
                *q++ = ch;
        }
    }
}

static int process_line(URLContext *h, char *line, int line_count,
                        int *new_location)
{
    HTTPContext *s = h->priv_data;
    char *tag, *p;

    /* end of header */
    if (line[0] == '\0')
        return 0;

    p = line;
    if (line_count == 0) {
        while (!isspace(*p) && *p != '\0')
            p++;
        while (isspace(*p))
            p++;
        s->http_code = strtol(p, NULL, 10);

        dprintf(NULL, "http_code=%d\n", s->http_code);

        /* error codes are 4xx and 5xx, but regard 401 as a success, so we
         * don't abort until all headers have been parsed. */
        if (s->http_code >= 400 && s->http_code < 600 && s->http_code != 401)
            return -1;
    } else {
        while (*p != '\0' && *p != ':')
            p++;
        if (*p != ':')
            return 1;

        *p = '\0';
        tag = line;
        p++;
        while (isspace(*p))
            p++;
        if (!strcasecmp(tag, "Location")) {
            strcpy(s->location, p);
            *new_location = 1;
        } else if (!strcasecmp (tag, "Accept-Ranges")) {
            if (!strncasecmp (p, "bytes", 5)){
				if(s->accept_range == -1 && s->stream_uri)
					s->redo = 1;
				s->accept_range = 1;
            }
			else
				s->accept_range = 0;
        } else if (!strcasecmp (tag, "Content-Length") && s->filesize == -1) {
            s->filesize = atoll(p);
        } else if (!strcasecmp (tag, "Content-Range")) {
            /* "bytes $from-$to/$document_size" */
            const char *slash;
            if (!strncasecmp (p, "bytes ", 6)) {
                p += 6;
                s->off = atoll(p);
                if ((slash = strchr(p, '/')) && strlen(slash) > 0)
                    s->filesize = atoll(slash+1);
            }
            h->is_streamed = 0; /* we _can_ in fact seek */
        } else if (!strcasecmp (tag, "Transfer-Encoding") && !strncasecmp(p, "chunked", 7)) {
            s->filesize = -1;
            s->chunksize = 0;
        } else if (!strcasecmp (tag, "WWW-Authenticate")) {
            ff_http_auth_handle_header(&s->auth_state, tag, p);
        } else if (!strcasecmp (tag, "Authentication-Info")) {
            ff_http_auth_handle_header(&s->auth_state, tag, p);
        }
    }
    return 1;
}

static inline int has_header(const char *str, const char *header)
{
    /* header + 2 to skip over CRLF prefix. (make sure you have one!) */
    return av_stristart(str, header + 2, NULL) || av_stristr(str, header);
}

static int http_connect(URLContext *h, const char *path, const char *hoststr,
                        const char *auth, int *new_location)
{
    HTTPContext *s = h->priv_data;
    int post, err;
    char line[1024];
    char headers[1024] = "";
    char *authstr = NULL;
    int64_t off = s->off;
    int len = 0;

    if(!s->buffer)
        s->buffer = av_malloc(buffer_size);

    if(s->cookie) {
        if((strlen(s->cookie) + 1024) > buffer_size) {
            av_freep(&s->buffer);
            buffer_size *= 2;
            s->buffer = av_malloc(buffer_size);
        }
    }

    /* send http header */
    post = h->flags & URL_WRONLY;
    authstr = ff_http_auth_create_response(&s->auth_state, auth, path,
                                        post ? "POST" : "GET");

    /* set default headers if needed */
    if (!has_header(s->headers, "\r\nUser-Agent: "))
       len += av_strlcatf(headers + len, sizeof(headers) - len,
                          "User-Agent: %s\r\n", LIBAVFORMAT_IDENT);
    if (!has_header(s->headers, "\r\nAccept: "))
        len += av_strlcpy(headers + len, "Accept: */*\r\n",
                          sizeof(headers) - len);
	hsprint(s->show_link,"Http Range Condition: %s AcceptRange %d Header %s\n",
			(s->stream_uri)?"(STREAM)":"(FILE URI)",s->accept_range,s->headers);
    if ((s->accept_range > 0 || !s->stream_uri) && !has_header(s->headers, "\r\nRange: "))
	        len += av_strlcatf(headers + len, sizeof(headers) - len,
	                           "Range: bytes=%"PRId64"-\r\n", s->off);
    if (!has_header(s->headers, "\r\nConnection: "))
        len += av_strlcpy(headers + len, "Connection: close\r\n",
                          sizeof(headers)-len);
    if (!has_header(s->headers, "\r\nHost: "))
        len += av_strlcatf(headers + len, sizeof(headers) - len,
                           "Host: %s\r\n", hoststr);

    /* now add in custom headers */
    av_strlcpy(headers+len, s->headers, sizeof(headers)-len);

    snprintf(s->buffer, buffer_size,
             "%s %s HTTP/1.1\r\n"
             "%s"
             "%s"
             "%s"
             "%s"
             "\r\n",
             post ? "POST" : "GET",
             path,
             post && s->chunksize >= 0 ? "Transfer-Encoding: chunked\r\n" : "",
             headers,
             (s->cookie)?s->cookie:"",
             authstr ? authstr : "");

	hsprint(s->show_link,"Http Request:\n%s\n",s->buffer);
    av_freep(&authstr);
    if (url_write(s->hd, s->buffer, strlen(s->buffer)) < 0)
        return AVERROR(EIO);

    /* init input buffer */
more_response:
    s->buf_ptr = s->buffer;
    s->buf_end = s->buffer;
    s->line_count = 0;
    s->off = 0;
    s->filesize = -1;
    if (post) {
        /* Pretend that it did work. We didn't read any header yet, since
         * we've still to send the POST data, but the code calling this
         * function will check http_code after we return. */
        s->http_code = 200;
        return 0;
    }
    s->chunksize = -1;

    /* wait for header */
	hsprint(s->show_link, "Http Response:\n");
    for(;;) {
        if (http_get_line(s, line, sizeof(line)) < 0)
            return AVERROR(EIO);

        dprintf(NULL, "header='%s'\n", line);
		hsprint(s->show_link, "%s\n", line);
        err = process_line(h, line, s->line_count, new_location);
        if (err < 0)
            return err;
        if (err == 0)
            break;
        s->line_count++;
    }

	if(s->http_code == 100){
		hsprint(s->show_link, "Http Continue...\n");
		goto more_response;
	}

	if((s->http_code == 302 || s->http_code == 303) && *new_location == 1) 
		s->off = off;

    return (off == s->off) ? 0 : -1;
}


static int http_read(URLContext *h, uint8_t *buf, int size)
{
    HTTPContext *s = h->priv_data;
    int len;

	if(!s->hd){
		av_log(NULL,AV_LOG_ERROR,"[HTTP] Read fail. (NULL handler)\n");
		return AVERROR(EIO);
	}

    if (s->chunksize >= 0) {
        if (!s->chunksize) {
            char line[32];

            for(;;) {
                do {
                    if (http_get_line(s, line, sizeof(line)) < 0)
                        return AVERROR(EIO);
                } while (!*line);    /* skip CR LF from last chunk */

                s->chunksize = strtoll(line, NULL, 16);

                dprintf(NULL, "Chunked encoding data size: %"PRId64"'\n", s->chunksize);

                if (!s->chunksize)
                    return 0;
                break;
            }
        }
        size = FFMIN(size, s->chunksize);
    }
    /* read bytes from input buffer first */
    len = s->buf_end - s->buf_ptr;
    if (len > 0) {
        if (len > size)
            len = size;
        memcpy(buf, s->buf_ptr, len);
        s->buf_ptr += len;
    } else {
        len = url_read(s->hd, buf, size);
    }
    if (len > 0) {
        s->off += len;
        if (s->chunksize > 0)
            s->chunksize -= len;
    }
    return len;
}

/* used only when posting data */
static int http_write(URLContext *h, const uint8_t *buf, int size)
{
    char temp[11] = "";  /* 32-bit hex + CRLF + nul */
    int ret;
    char crlf[] = "\r\n";
    HTTPContext *s = h->priv_data;

	if(!s->hd){
		av_log(NULL,AV_LOG_ERROR,"[HTTP] Write fail. (NULL handler)\n");
		return AVERROR(EIO);
	}

    if (s->chunksize == -1) {
        /* non-chunked data is sent without any special encoding */
        return url_write(s->hd, buf, size);
    }

    /* silently ignore zero-size data since chunk encoding that would
     * signal EOF */
    if (size > 0) {
        /* upload data using chunked encoding */
        snprintf(temp, sizeof(temp), "%x\r\n", size);

        if ((ret = url_write(s->hd, temp, strlen(temp))) < 0 ||
            (ret = url_write(s->hd, buf, size)) < 0 ||
            (ret = url_write(s->hd, crlf, sizeof(crlf) - 1)) < 0)
            return ret;
    }
    return size;
}

static int http_close(URLContext *h)
{
    int ret = 0;
    char footer[] = "0\r\n\r\n";
    HTTPContext *s = h->priv_data;

    /* signal end of chunked encoding if used */
    if (s->hd && (h->flags & URL_WRONLY) && s->chunksize != -1) {
        ret = url_write(s->hd, footer, sizeof(footer) - 1);
        ret = ret > 0 ? 0 : ret;
    }
    av_freep(&s->cookie);
    av_freep(&s->buffer);
    if (s->hd)
        url_close(s->hd);
    return ret;
}

static int64_t http_seek(URLContext *h, int64_t off, int whence)
{
    HTTPContext *s = h->priv_data;
    URLContext *old_hd = s->hd;
    int64_t old_off = s->off;
    uint8_t old_buf[BUFFER_SIZE];
    int old_buf_size;

    if (whence == AVSEEK_SIZE)
        return s->filesize;
    else if ((s->filesize == -1 && whence == SEEK_END) || h->is_streamed)
        return -1;

    /* we save the old context in case the seek fails */
    old_buf_size = s->buf_end - s->buf_ptr;
    memcpy(old_buf, s->buf_ptr, old_buf_size);
    s->hd = NULL;
    if (whence == SEEK_CUR)
        off += s->off;
    else if (whence == SEEK_END)
        off += s->filesize;
    s->off = off;

	if(s->no_section_cross){
		url_close(old_hd);
		s->hd = NULL;
		if (http_open_cnx(h) < 0) {
			av_log(NULL,AV_LOG_ERROR,"[HTTP] SEEK fail. (open new socket fail)\n");
			return AVERROR(EIO);
		}
		return off;
	}

    /* if it fails, continue on old connection */
    if (http_open_cnx(h) < 0) {
        memcpy(s->buffer, old_buf, old_buf_size);
        s->buf_ptr = s->buffer;
        s->buf_end = s->buffer + old_buf_size;
        s->hd = old_hd;
        s->off = old_off;
        return -1;
    }
    url_close(old_hd);
    return off;
}

static int
http_get_file_handle(URLContext *h)
{
    HTTPContext *s = h->priv_data;
    if(!s->hd){
    	av_log(NULL,AV_LOG_ERROR,"[HTTP] Get file handle fail. (NULL handler)\n");
    	return -1;
    }
    return url_get_file_handle(s->hd);
}

URLProtocol http_protocol = {
    "http",
    http_open,
    http_read,
    http_write,
    http_seek,
    http_close,
    .url_get_file_handle = http_get_file_handle,
    .priv_data_size = sizeof(HTTPContext),
    .priv_data_class = &httpcontext_class,
};
