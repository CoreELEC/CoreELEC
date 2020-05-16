/*
 * Copyright © 2012 Collabora, Ltd.
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the copyright holders not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  The copyright holders make
 * no representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 * CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef SERVER_WLEGL_BUFFER_H
#define SERVER_WLEGL_BUFFER_H

#include <cutils/native_handle.h>
#include <string.h>
#include <system/window.h>
#include <hardware/gralloc.h>
#include <wayland-server.h>
#include "windowbuffer.h"

struct server_wlegl;

struct server_wlegl_buffer {
	struct wl_resource *resource;
	server_wlegl *wlegl;

	RemoteWindowBuffer *buf;
};

server_wlegl_buffer *
server_wlegl_buffer_create(wl_client *client, uint32_t id, int32_t width, int32_t height,
			   int32_t stride, int32_t format, int32_t usage,
			   buffer_handle_t handle, server_wlegl *wlegl);

server_wlegl_buffer *
server_wlegl_buffer_create_server(wl_client *client, int32_t width, int32_t height,
			   int32_t stride, int32_t format, int32_t usage,
			   buffer_handle_t handle, server_wlegl *wlegl);

server_wlegl_buffer *
server_wlegl_buffer_from(struct wl_resource *);

#endif /* SERVER_WLEGL_BUFFER_H */
