/*
 * RTP network protocol
 * Copyright (c) 2002 Fabrice Bellard
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
/*
 * [RTP reorder routines]
 * Also handling of repeated UDP packets (a bug of ExtremeNetworks switches firmware)
 * rtpreord procedures, write rtp packets to buffer, and reoder rtp packets
 * [RTP Reordering functions]
 * Algorithm works as follows:
 * If next packet is in sequence just copy it to buffer
 * Otherwise copy it in buffer according to its sequence number
 * Buffer is a circular array where "rtpbuf.first" points to next sequence slot
 * and keeps track of expected sequence
*/

#include "libavutil/avstring.h"
#include "avformat.h"

#include <unistd.h>
#include <stdarg.h>
#include "internal.h"
#include "network.h"
#include "os_support.h"
#include <fcntl.h>
#if HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif
#include <sys/time.h>
#include <sys/ioctl.h>
#include <linux/sockios.h>

// #define CFG_WMTRTP_DGB
#ifdef CFG_WMTRTP_DGB
	#define vprintf(pctx, ...) do{ av_log(pctx, AV_LOG_INFO, __VA_ARGS__); }while(0)
	#define wprintf(pctx, ...) do{ av_log(pctx, AV_LOG_DEBUG, __VA_ARGS__); }while(0)
#else
	#define vprintf(pctx, ...) do{ }while(0)
	#define wprintf(pctx, ...) do{ }while(0)
#endif

#define MAX_BUFFER_PKTS 32   // The number of max packets being reordered
#define RTP_PAYLOAD_SIZE 16*1024

#if HAVE_STRUCT_IPV6_MREQ
#ifndef CONFIG_ANDROID
struct ip_mreq{
	struct in_addr imr_multiaddr;   /* IP multicast address of group */
	struct in_addr imr_interface;   /* local IP address of interface */
};
#endif
#endif

struct rtpbits {
	unsigned int v:2;           /* version: 2 */
	unsigned int p:1;           /* is there padding appended: 0 */
	unsigned int x:1;           /* number of extension headers: 0 */
	unsigned int cc:4;          /* number of CSRC identifiers: 0 */
	unsigned int m:1;           /* marker: 0 */
	unsigned int pt:7;          /* payload type: 33 for MPEG2 TS - RFC 1890 */
	unsigned int sequence:16;   /* sequence number: random */
}__attribute__((packed));

struct rtpheader {	/* in network byte order */
	struct rtpbits b;
	int timestamp;	/* start: random */
	int ssrc;		/* random */
};

struct rtpbuffer{
	unsigned char  data[MAX_BUFFER_PKTS][RTP_PAYLOAD_SIZE];
	unsigned short  seq[MAX_BUFFER_PKTS];
	unsigned short  len[MAX_BUFFER_PKTS];
	unsigned short first;
};

typedef struct WMTRTPContext {
	struct rtpbuffer buffer;
	char hostname[256];
    int fd;
	int port;
	int is_first;
	int next_seq;
	int64_t get_pkt_time;
} WMTRTPContext;

static int wmtrtp_buffer_status(WMTRTPContext *wrtp, int show)
{
	int i,rpkts = 0;

	for(i=0; i<MAX_BUFFER_PKTS; i++){
		if(wrtp->buffer.len[i]){
			vprintf(NULL,"[%d] seq %x len %d\n",i,wrtp->buffer.seq[i],wrtp->buffer.len[i]);
			rpkts++;
		}
	}
	vprintf(NULL,"[RTP Buffer Status] 1st %d Next Seq %x Tot %d\n",
		wrtp->buffer.first,wrtp->next_seq,rpkts);

	return rpkts;
}

/* Input:  interval - ms
           cnt      - times
   Return: < 0 - fail
           = 0 - time out
           > 0 - read and remain cnt
*/
static int wmtrtp_socket_ready(int sfd, int interval, int cnt)
{
	struct timeval tv;
	fd_set set;
	int err;

	while(cnt){
		cnt--;
		if (url_interrupt_cb()){
			av_log(NULL,AV_LOG_WARNING,"[RTP] socket interrupted\n");
			return AVERROR(EINTR);
		}
		FD_ZERO(&set);
		FD_SET(sfd, &set);
		tv.tv_sec = interval/1000;
		tv.tv_usec = (interval % 1000) * 1000;
		err = select(sfd + 1, &set, NULL, NULL, &tv);
		if (err < 0) {
			if (ff_neterrno() == FF_NETERROR(EINTR))
				continue;
			av_log(NULL,AV_LOG_ERROR,"[RTP] socket error (select)\n");
			return AVERROR(EIO);
		}
		if (!(err > 0 && FD_ISSET(sfd, &set)))
			continue;
		break;
	}

	return cnt;
}

static int wmtrtp_get_packet(int fd, struct rtpheader *rh, char** data, int* lengthData)
{
	static char buf[RTP_PAYLOAD_SIZE];
	static int prevSeq = 0;
	unsigned int intP, cnt = 0;
	char* charP = (char*) &intP;
	int headerSize, lengthPacket, ret;

	for(;;) {
		cnt++;
		ret = wmtrtp_socket_ready(fd,100,20); // 2s
		if(ret < 0)
			return ret;
		if(!ret){
			av_log(NULL,AV_LOG_WARNING,"[RTP] socket read idle %d ms\n",cnt*2000);
			continue;
		}

		lengthPacket = recv(fd, buf, RTP_PAYLOAD_SIZE, 0);
		if (lengthPacket < 0) {
			if (ff_neterrno() != FF_NETERROR(EAGAIN) &&
				ff_neterrno() != FF_NETERROR(EINTR)){
				av_log(NULL,AV_LOG_ERROR,"[RTP] socket read error (recv)\n");
				return AVERROR(EIO);
			}
		} else
			break;
	}

	if(lengthPacket<12) {
		av_log(NULL,AV_LOG_ERROR,"[RTP] packet too small (%d) to be an rtp frame (>12bytes)\n",lengthPacket);
		*lengthData = 0;
		return 0;
	}
	rh->b.v  = (unsigned int) ((buf[0]>>6)&0x03);
	rh->b.p  = (unsigned int) ((buf[0]>>5)&0x01);
	rh->b.x  = (unsigned int) ((buf[0]>>4)&0x01);
	rh->b.cc = (unsigned int) ((buf[0]>>0)&0x0f);
	rh->b.m  = (unsigned int) ((buf[1]>>7)&0x01);
	rh->b.pt = (unsigned int) ((buf[1]>>0)&0x7f);
	intP = 0;
	memcpy(charP+2,&buf[2],2);
	rh->b.sequence = ntohl(intP);
	intP = 0;
	memcpy(charP,&buf[4],4);
	rh->timestamp = ntohl(intP);

	headerSize = 12 + 4*rh->b.cc; /* in bytes */

	*lengthData = lengthPacket - headerSize;
	*data = (char*) buf + headerSize;

	wprintf(NULL,"Reading rtp: v=%x p=%x x=%x cc=%x m=%x pt=%x seq=%x ts=%x lgth=%d off=%x\n",
		rh->b.v,rh->b.p,rh->b.x,rh->b.cc,rh->b.m,rh->b.pt,rh->b.sequence,rh->timestamp,lengthPacket,rh->b.sequence-prevSeq);
	prevSeq = rh->b.sequence;

	return 0;
}

// Initialize rtp buffer
static void wmtrtp_buffer_reset(WMTRTPContext *wrtp, unsigned short seq)
{
	int i;

	wmtrtp_buffer_status(wrtp,1);
	wrtp->buffer.first = 0;
	wrtp->buffer.seq[0] = ++seq;
	for (i=0; i<MAX_BUFFER_PKTS; i++)
		wrtp->buffer.len[i] = 0;
	wprintf(NULL,"RTP Buffer Reset\n");
}

// fill buffer with the rtp packet in right rtp sequence order
static int wmtrtp_fill_buffer(WMTRTPContext *wrtp, char *buffer, int length)
{
	struct rtpheader rh;
	unsigned short seq;
	int newseq, i, ret, off;
	char *data;
	int64_t diff = 0, tt = av_gettime();

	if(wrtp->get_pkt_time)
		diff = tt - wrtp->get_pkt_time;
	wrtp->get_pkt_time = tt;
	if((ret = wmtrtp_get_packet(wrtp->fd, &rh, &data, &length)) != 0)
		return ret;

	if(!length){
		av_log(NULL,AV_LOG_WARNING,"[RTP] null packet.\n");
		return 0;
	}

	seq = rh.b.sequence;
	off = seq - wrtp->buffer.seq[wrtp->buffer.first];
	wprintf(NULL,"[RTP] buf(%d) (seq[%d]=%x seq=%x, off=%d)\n",wmtrtp_buffer_status(wrtp,0),
		wrtp->buffer.first, wrtp->buffer.seq[wrtp->buffer.first], seq, off);
	if(diff > 100*1000)
		av_log(NULL,AV_LOG_INFO,"[RTP] buf(%d) (seq[%d]=%x seq=%x len=%d, off=%d, tm=%lld)\n",
			wmtrtp_buffer_status(wrtp,0),	wrtp->buffer.first, wrtp->buffer.seq[wrtp->buffer.first], 
			seq, length, off, diff/1000);
	if ((off == 0) || wrtp->is_first){
		wrtp->is_first = 0;
		wprintf(NULL,"[RTP] (seq[%d]=%x seq=%x, off=%d)\n", 
			wrtp->buffer.first, wrtp->buffer.seq[wrtp->buffer.first], seq, off);
		wrtp->buffer.first = ( 1 + wrtp->buffer.first ) % MAX_BUFFER_PKTS;
		wrtp->buffer.seq[wrtp->buffer.first] = ++seq;
		goto feed;
	}

	if (off > MAX_BUFFER_PKTS){
		av_log(NULL, AV_LOG_WARNING, "[RTP] Overrun(seq[%d]=%x seq=%x, off=%x, tm=%lld, nb=%d) ... Reset\n", 
			wrtp->buffer.first, wrtp->buffer.seq[wrtp->buffer.first], seq, off, diff/1000, wmtrtp_buffer_status(wrtp,0));
		wmtrtp_buffer_reset(wrtp,seq);
		goto feed;
	}

	if (off < 0){
		// Is it a stray packet re-sent to network?
		for (i=0; i<MAX_BUFFER_PKTS; i++){
			if (wrtp->buffer.seq[i] == seq) {
				av_log(NULL, AV_LOG_WARNING, "[RTP] Stray packet (seq[%d]=%x seq=%x, off=%d found at %d, tm=%lld)\n", 
					wrtp->buffer.first, wrtp->buffer.seq[wrtp->buffer.first], seq, off, i, diff/1000);
				return  0; // Yes, it is!
			}
		}
		// Some heuristic to decide when to drop packet or to restart everything
		if (off > -(3 * MAX_BUFFER_PKTS)) {
			av_log(NULL, AV_LOG_WARNING, "[RTP] Too Old packet (seq[%d]=%x seq=%x, off=%d, tm=%lld)\n",
				wrtp->buffer.first, wrtp->buffer.seq[wrtp->buffer.first], seq, off, diff/1000);
			return  0; // Yes, it is!
		}

		av_log(NULL, AV_LOG_WARNING,  "[RTP] Underrun(seq[%d]=%x seq=%x, off=%d, tm=%lld) ... Reset\n", 
			wrtp->buffer.first, wrtp->buffer.seq[wrtp->buffer.first], seq, off, diff/1000);

		wmtrtp_buffer_reset(wrtp,seq);
		goto feed;
	}

	newseq = ( off + wrtp->buffer.first ) % MAX_BUFFER_PKTS;
	vprintf(NULL,"[RTP] Out of Seq, Off %d. Save to buf[%d] Seq %x/%x len %d/%d\n",
		off, newseq, wrtp->buffer.seq[newseq], seq, wrtp->buffer.len[newseq], length);
	memcpy (wrtp->buffer.data[newseq], data, length);
	wrtp->buffer.len[newseq] = length;
	wrtp->buffer.seq[newseq] = seq;

	return 0;

feed:
	wrtp->next_seq = wrtp->buffer.seq[wrtp->buffer.first];
	memcpy (buffer, data, length);
	return length;
}

// Get next packet in buffer
// Look in buffer to get first packet in sequence
static int wmtrtp_get_buffer(WMTRTPContext *wrtp, char *buffer, int length)
{
	int i, idx = 0, minseq = 0x10000; // bigger than all seq number
	unsigned short nextseq;

	// If we have empty buffer we loop to fill it
	for (i=0; i < MAX_BUFFER_PKTS - 3; i++) {
		if (wrtp->buffer.len[wrtp->buffer.first] != 0)
			break;
		length = wmtrtp_fill_buffer(wrtp, buffer, length);
		if(length)
			return length;
	}

	// wmtrtp_buffer_status(wrtp,1);
	for(i = 0; i < MAX_BUFFER_PKTS; i++){
		if(wrtp->buffer.len[i] && minseq > wrtp->buffer.seq[i]){
			wrtp->buffer.first = i;	
			minseq = wrtp->buffer.seq[i];
		}
	}
	i = wrtp->buffer.first;
	while (wrtp->buffer.len[i] == 0) {
		idx++;
		av_log(NULL,AV_LOG_WARNING,"[RTP] Lost packet (%d-%d) seq %x len %d\n",
			idx,i,wrtp->buffer.seq[i],wrtp->buffer.len[i]);
		i = ( 1 + i ) % MAX_BUFFER_PKTS;
		if (wrtp->buffer.first == i)
			break;
	}
	wrtp->buffer.first = i;

	// Copy next non empty packet from buffer
	if(wrtp->buffer.seq[wrtp->buffer.first] > wrtp->next_seq)
		av_log(NULL,AV_LOG_WARNING,"[RTP] Get rtp from buf [%d] %x lost %d pkts\n", 
			wrtp->buffer.first, wrtp->buffer.seq[wrtp->buffer.first],
			wrtp->buffer.seq[wrtp->buffer.first]-wrtp->next_seq);
	memcpy (buffer, wrtp->buffer.data[wrtp->buffer.first], wrtp->buffer.len[wrtp->buffer.first]);
	length = wrtp->buffer.len[wrtp->buffer.first]; // can be zero?

	// Reset fisrt slot and go next in buffer
	wrtp->buffer.len[wrtp->buffer.first] = 0;
	nextseq = wrtp->buffer.seq[wrtp->buffer.first];
	wrtp->buffer.first = ( 1 + wrtp->buffer.first ) % MAX_BUFFER_PKTS;
	wrtp->next_seq = wrtp->buffer.seq[wrtp->buffer.first] = nextseq + 1;

	return length;
}

static int wmtrtp_open_socket(WMTRTPContext *wrtp)
{
	int fd, rxbufsz, err;
	socklen_t err_len;
	struct sockaddr_in saddr;
	struct hostent *hp;

	av_log(NULL, AV_LOG_INFO,
          "[RTP] Listening for traffic on %s:%d ...\n", wrtp->hostname, wrtp->port);

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(fd == -1){
		av_log(NULL, AV_LOG_ERROR, "[RTP] Create socket fail\n");
		return -1;
	}

	memset(&saddr, 0, sizeof(saddr));
	if(isalpha(wrtp->hostname[0])){
		hp = (struct hostent *) gethostbyname(wrtp->hostname);
		if(!hp){
			av_log(NULL, AV_LOG_ERROR,
				"[RTP] Counldn't resolve name: %s\n", wrtp->hostname);
			goto fail;
		}
		memcpy ((void *)&saddr.sin_addr.s_addr,
				(void *)hp->h_addr_list[0], hp->h_length);
	}
	else
		inet_aton(wrtp->hostname, &saddr.sin_addr);

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(wrtp->port);
	if(bind(fd, (struct sockaddr *)&saddr, sizeof(saddr)) == -1){
		if(errno != EINPROGRESS){
			av_log(NULL, AV_LOG_ERROR, "[RTP] Connect to host fail\n");
			goto fail;
		}
	}

	if((ntohl (saddr.sin_addr.s_addr) >> 28) == 0xe){
		struct ip_mreq mcast;
		mcast.imr_multiaddr.s_addr = saddr.sin_addr.s_addr;
		mcast.imr_interface.s_addr = 0;
		if(setsockopt (fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mcast, sizeof (mcast))){
			av_log(NULL, AV_LOG_ERROR, 
				"[RTP] IP_ADD_MEMBERSHIP failed (do multicasting enable in kernel?)\n");
			goto fail;
		}
	}

	if(wmtrtp_socket_ready(fd, 1000, 30) < 0) // wait 30s
		goto fail;

	err_len = sizeof (err);
	getsockopt(fd, SOL_SOCKET, SO_ERROR, &err, &err_len);
	if(err){
		av_log(NULL, AV_LOG_ERROR, "[RTP] Socket error: %d\n", err);
		goto fail;
	}

	rxbufsz = 0;
	err_len = sizeof(rxbufsz);
	if(getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &rxbufsz, &err_len))
		av_log(NULL, AV_LOG_INFO, "[RTP] Couldn't get receive socket buffer size\n");
	else
		av_log(NULL, AV_LOG_INFO, "[RTP] Get receive socket buffer size %d\n",rxbufsz);

	return fd;

fail:
	 closesocket(fd);
	 return -1;
}

// URL Syntax: wrtp://host:port
static int wmtrtp_open(URLContext *h, const char *uri, int flags)
{
    WMTRTPContext *wrtp = h->priv_data;

	av_log(NULL, AV_LOG_INFO, "[RTP] URL: %s\n", uri);
	av_url_split(NULL, 0, NULL, 0, wrtp->hostname, 
		sizeof(wrtp->hostname), &wrtp->port, NULL, 0, uri);

	if(wrtp->port == 0){
		av_log(NULL, AV_LOG_ERROR, "[RTP] unknown port number for RTP streams!\n");
		return AVERROR(EIO);
	}

	wrtp->fd = wmtrtp_open_socket(wrtp);
	if(wrtp->fd < 0){
		av_log(NULL, AV_LOG_ERROR, "[RTP] open socket fail!\n");
		return AVERROR(EIO);
	}

	wrtp->get_pkt_time = 0;
    wrtp->is_first = h->is_streamed = 1;
    h->max_packet_size = RTP_PAYLOAD_SIZE;

	av_log(NULL,AV_LOG_INFO,"[RTP] open socket (fd %d)\n",wrtp->fd);

    return 0;
}

// Read next rtp packet using buffer
static int wmtrtp_read(URLContext *h, uint8_t *buf, int size)
{
    WMTRTPContext *wrtp = h->priv_data;

	// Following test is ASSERT (i.e. uneuseful if code is correct)
	if(buf==NULL || size<RTP_PAYLOAD_SIZE) {
		av_log(NULL, AV_LOG_ERROR, "[RTP] buffer invalid; buf %p size %d\n", buf, size);
		return 0;
	}

	// loop just to skip empty packets
	while((size = wmtrtp_get_buffer(wrtp, buf, size)) == 0){
		av_log(NULL, AV_LOG_ERROR, "[RTP] Got empty packet from buffer!?\n");
	}

	return size;
}

static int wmtrtp_write(URLContext *h, const uint8_t *buf, int size)
{
	av_log(NULL,AV_LOG_ERROR,"[RTP] Write is not supported. (size %d)\n",size);
    return AVERROR(EIO);
}

static int wmtrtp_close(URLContext *h)
{
    WMTRTPContext *wrtp = h->priv_data;
	if(wrtp->fd >= 0)
		closesocket(wrtp->fd);

	av_log(NULL,AV_LOG_INFO,"[RTP] close socket (fd %d)\n",wrtp->fd);
	wrtp->fd = -1;
    return 0;
}

static int wmtrtp_get_file_handle(URLContext *h)
{
    WMTRTPContext *wrtp = h->priv_data;
    return wrtp->fd;
}

URLProtocol wmtrtp_protocol = {
    "rtp",
    wmtrtp_open,
    wmtrtp_read,
    wmtrtp_write,
    NULL, /* seek */
    wmtrtp_close,
    .url_get_file_handle = wmtrtp_get_file_handle,
	.priv_data_size = sizeof(WMTRTPContext),
	// .priv_data_class = &httpcontext_class, // not use now
};
