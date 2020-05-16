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

#include <EGL/egl.h>
#include <EGL/eglext.h>

extern "C" {
#include <cutils/native_handle.h>
}

#include "logging.h"

#include <wayland-server.h>
#include "wayland-android-server-protocol.h"
#include "server_wlegl_private.h"
#include "server_wlegl_handle.h"
#include "server_wlegl_buffer.h"

#include <hybris/gralloc/gralloc.h>

static inline server_wlegl *
server_wlegl_from(struct wl_resource *resource)
{
	return reinterpret_cast<server_wlegl *>(wl_resource_get_user_data(resource));
}

static void
server_wlegl_create_handle(struct wl_client *client,
			   struct wl_resource *resource,
			   uint32_t id,
			   int32_t num_fds,
			   struct wl_array *ints)
{
	server_wlegl_handle *handle;

	if (num_fds < 0) {
		wl_resource_post_error(resource,
				       ANDROID_WLEGL_ERROR_BAD_VALUE,
				       "num_fds is negative: %d", num_fds);
		return;
	}

	handle = server_wlegl_handle_create(id);
	wl_array_copy(&handle->ints, ints);
	handle->num_fds = num_fds;
	wl_client_add_resource(client, &handle->resource);
}

static void
server_wlegl_create_buffer(struct wl_client *client,
			   struct wl_resource *resource,
			   uint32_t id,
			   int32_t width,
			   int32_t height,
			   int32_t stride,
			   int32_t format,
			   int32_t usage,
			   struct wl_resource *hnd)
{
	server_wlegl *wlegl = server_wlegl_from(resource);
	server_wlegl_handle *handle = server_wlegl_handle_from(hnd);
	server_wlegl_buffer *buffer;
	buffer_handle_t native;

	if (width < 1 || height < 1) {
		wl_resource_post_error(resource,
				       ANDROID_WLEGL_ERROR_BAD_VALUE,
				       "bad width (%d) or height (%d)",
				       width, height);
		return;
	}

	native = server_wlegl_handle_to_native(handle);
	if (!native) {
		wl_resource_post_error(resource,
				       ANDROID_WLEGL_ERROR_BAD_HANDLE,
				       "fd count mismatch");
		return;
	}

	buffer = server_wlegl_buffer_create(client, id, width, height, stride,
					    format, usage, native, wlegl);
	if (!buffer) {
		native_handle_close((native_handle_t *)native);
		native_handle_delete((native_handle_t *)native);
		wl_resource_post_error(resource,
				       ANDROID_WLEGL_ERROR_BAD_HANDLE,
				       "invalid native handle");
		return;
	}
}

static void
server_wlegl_get_server_buffer_handle(wl_client *client, wl_resource *res, uint32_t id, int32_t width, int32_t height, int32_t format, int32_t usage)
{
	if (width == 0 || height == 0) {
		wl_resource_post_error(res, 0, "invalid buffer size: %u,%u\n", width, height);
		return;
	}

	server_wlegl *wlegl = server_wlegl_from(res);

	wl_resource *resource = wl_resource_create(client, &android_wlegl_server_buffer_handle_interface, wl_resource_get_version(res), id);

	buffer_handle_t _handle;
	int _stride;

	usage |= GRALLOC_USAGE_HW_COMPOSER;

	int r = hybris_gralloc_allocate(width, height, format, usage, &_handle, (uint32_t*)&_stride);
        if (r) {
            HYBRIS_ERROR_LOG(SERVER_WLEGL, "failed to allocate buffer\n");
        }
	server_wlegl_buffer *buffer = server_wlegl_buffer_create_server(client, width, height, _stride, format, usage, _handle, wlegl);

	struct wl_array ints;
	int *ints_data;
	wl_array_init(&ints);
	ints_data = (int*) wl_array_add(&ints, _handle->numInts * sizeof(int));
	memcpy(ints_data, _handle->data + _handle->numFds, _handle->numInts * sizeof(int));

	android_wlegl_server_buffer_handle_send_buffer_ints(resource, &ints);
	wl_array_release(&ints);

	for (int i = 0; i < _handle->numFds; i++) {
		android_wlegl_server_buffer_handle_send_buffer_fd(resource, _handle->data[i]);
	}

	android_wlegl_server_buffer_handle_send_buffer(resource, buffer->resource, format, _stride);
	wl_resource_destroy(resource);
}

static const struct android_wlegl_interface server_wlegl_impl = {
	server_wlegl_create_handle,
	server_wlegl_create_buffer,
	server_wlegl_get_server_buffer_handle,
};

static void
server_wlegl_bind(struct wl_client *client, void *data,
		  uint32_t version, uint32_t id)
{
	server_wlegl *wlegl = reinterpret_cast<server_wlegl *>(data);
	struct wl_resource *resource;

	resource = wl_resource_create(client, &android_wlegl_interface, version, id);
	wl_resource_set_implementation(resource, &server_wlegl_impl, wlegl, 0);
}

server_wlegl *
server_wlegl_create(struct wl_display *display)
{
	struct server_wlegl *wlegl;

	wlegl = new server_wlegl;

	wlegl->display = display;
	wlegl->global = wl_global_create(display, &android_wlegl_interface, 2,
					      wlegl, server_wlegl_bind);

	return wlegl;
}

void
server_wlegl_destroy(server_wlegl *wlegl)
{
	/* FIXME: server_wlegl_buffer objects may exist */

	/* FIXME: remove global_ */

	/* Better to leak than expose dtor segfaults, the server
	 * supposedly exits soon after. */
	//LOGW("server_wlegl object leaked on UnbindWaylandDisplayWL");
	/* delete wlegl; */
}

