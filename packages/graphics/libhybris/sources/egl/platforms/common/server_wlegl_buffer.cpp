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

#include <android-config.h>
#include <cstring>
#include <cassert>

#include "server_wlegl_buffer.h"
#include "server_wlegl_private.h"

#include <hybris/gralloc/gralloc.h>

static void
destroy(struct wl_client *client, struct wl_resource *resource)
{
	wl_resource_destroy(resource);
}

static const struct wl_buffer_interface server_wlegl_buffer_impl = {
	destroy
};

server_wlegl_buffer *
server_wlegl_buffer_from(struct wl_resource *buffer)
{
	return static_cast<server_wlegl_buffer *>(wl_resource_get_user_data(buffer));
}

static void
server_wlegl_buffer_dtor(struct wl_resource *resource)
{
	server_wlegl_buffer *buffer = server_wlegl_buffer_from(resource);
	buffer->buf->common.decRef(&buffer->buf->common);
	delete buffer;
}

server_wlegl_buffer *
server_wlegl_buffer_create(wl_client *client,
			   uint32_t id,
			   int32_t width,
			   int32_t height,
			   int32_t stride,
			   int32_t format,
			   int32_t usage,
			   buffer_handle_t handle,
			   server_wlegl *wlegl)
{
	server_wlegl_buffer *buffer = new server_wlegl_buffer;
	int ret;

	buffer->wlegl = wlegl;
	buffer->resource = wl_resource_create(client, &wl_buffer_interface, 1, id);
	wl_resource_set_implementation(buffer->resource, &server_wlegl_buffer_impl, buffer, server_wlegl_buffer_dtor);

	ret = hybris_gralloc_retain(handle);
	if (ret) {
		delete buffer;
		return NULL;
	}

	buffer->buf = new RemoteWindowBuffer(
	        width, height, stride, format, usage, handle);
	buffer->buf->common.incRef(&buffer->buf->common);
	return buffer;
}

server_wlegl_buffer *
server_wlegl_buffer_create_server(wl_client *client,
			   int32_t width,
			   int32_t height,
			   int32_t stride,
			   int32_t format,
			   int32_t usage,
			   buffer_handle_t handle,
			   server_wlegl *wlegl)
{
	server_wlegl_buffer *buffer = new server_wlegl_buffer;

	buffer->wlegl = wlegl;
	buffer->resource = wl_resource_create(client, &wl_buffer_interface, 1, 0);
	wl_resource_set_implementation(buffer->resource, &server_wlegl_buffer_impl, buffer, server_wlegl_buffer_dtor);

	buffer->buf = new RemoteWindowBuffer(
	        width, height, stride, format, usage, handle);
	buffer->buf->setAllocated(true);
	buffer->buf->common.incRef(&buffer->buf->common);
	return buffer;
}

