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

#ifndef SERVER_WLEGL_HANDLE_H
#define SERVER_WLEGL_HANDLE_H

#include <stdint.h>
#include <string.h>
#include <wayland-server.h>
#include <cutils/native_handle.h>
#include <system/window.h>
struct server_wlegl_handle {
	struct wl_resource resource;

	struct wl_array ints;
	struct wl_array fds;
	int num_fds;
        int num_ints;
};

server_wlegl_handle *
server_wlegl_handle_create(uint32_t id);

static inline server_wlegl_handle *
server_wlegl_handle_from(struct wl_resource *resource)
{
	return reinterpret_cast<server_wlegl_handle *>(resource->data);
}

buffer_handle_t
server_wlegl_handle_to_native(server_wlegl_handle *handle);

#endif /* SERVER_WLEGL_HANDLE_H */
