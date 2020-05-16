/****************************************************************************************
 **
 ** Copyright (C) 2013 Jolla Ltd.
 ** Contact: Carsten Munk <carsten.munk@jollamobile.com>
 ** All rights reserved.
 **
 ** This file is part of Wayland enablement for libhybris
 **
 ** You may use this file under the terms of the GNU Lesser General
 ** Public License version 2.1 as published by the Free Software Foundation
 ** and appearing in the file license.lgpl included in the packaging
 ** of this file.
 **
 ** This library is free software; you can redistribute it and/or
 ** modify it under the terms of the GNU Lesser General Public
 ** License version 2.1 as published by the Free Software Foundation
 ** and appearing in the file license.lgpl included in the packaging
 ** of this file.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 ** Lesser General Public License for more details.
 **
 ****************************************************************************************/


#include <android-config.h>
#include "wayland_window.h"
#include "wayland-egl-priv.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "logging.h"
#include <eglhybris.h>

#if ANDROID_VERSION_MAJOR>=4 && ANDROID_VERSION_MINOR>=2 || ANDROID_VERSION_MAJOR>=5
extern "C" {
#include <sync/sync.h>
}
#endif

static void
buffer_create_sync_callback(void *data, struct wl_callback *callback, uint32_t serial)
{
   struct wl_callback **created_callback = static_cast<struct wl_callback **>(data);

   *created_callback = NULL;
   wl_callback_destroy(callback);
}

static const struct wl_callback_listener buffer_create_sync_listener = {
   buffer_create_sync_callback
};

void WaylandNativeWindowBuffer::wlbuffer_from_native_handle(struct android_wlegl *android_wlegl,
                                                            struct wl_display *display, struct wl_event_queue *queue)
{
    struct wl_array ints;
    int *ints_data;
    struct android_wlegl_handle *wlegl_handle;

    wl_array_init(&ints);
    ints_data = (int*) wl_array_add(&ints, handle->numInts*sizeof(int));
    memcpy(ints_data, handle->data + handle->numFds, handle->numInts*sizeof(int));

    wlegl_handle = android_wlegl_create_handle(android_wlegl, handle->numFds, &ints);

    wl_array_release(&ints);

    for (int i = 0; i < handle->numFds; i++) {
        android_wlegl_handle_add_fd(wlegl_handle, handle->data[i]);
    }

    wlbuffer = android_wlegl_create_buffer(android_wlegl,
            width, height, stride,
            format, usage, wlegl_handle);
    wl_proxy_set_queue((struct wl_proxy *) wlbuffer, queue);

    android_wlegl_handle_destroy(wlegl_handle);

    creation_callback = wl_display_sync(display);
    wl_callback_add_listener(creation_callback, &buffer_create_sync_listener, &creation_callback);
    wl_proxy_set_queue((struct wl_proxy *)creation_callback, queue);
}

void WaylandNativeWindow::resize(unsigned int width, unsigned int height)
{
    lock();
    this->m_defaultWidth = width;
    this->m_defaultHeight = height;
    unlock();
}

void WaylandNativeWindow::resize_callback(struct wl_egl_window *egl_window, void *)
{
    TRACE("%dx%d",egl_window->width,egl_window->height);
    ((WaylandNativeWindow *) egl_window->driver_private)->resize(egl_window->width, egl_window->height);
}

void WaylandNativeWindow::destroy_window_callback(void *data)
{
    WaylandNativeWindow *native = (WaylandNativeWindow*)data;

    native->lock();
    native->m_window = 0;
    native->unlock();
}

void WaylandNativeWindow::lock()
{
    pthread_mutex_lock(&this->mutex);
}

void WaylandNativeWindow::unlock()
{
    pthread_mutex_unlock(&this->mutex);
}

void
WaylandNativeWindow::sync_callback(void *data, struct wl_callback *callback, uint32_t serial)
{
    int *done = static_cast<int *>(data);

    *done = 1;
    wl_callback_destroy(callback);
}

static const struct wl_callback_listener sync_listener = {
    WaylandNativeWindow::sync_callback
};


#if WAYLAND_VERSION_MAJOR == 0 || (WAYLAND_VERSION_MAJOR == 1 && WAYLAND_VERSION_MINOR < 6)
int
WaylandNativeWindow::wl_display_roundtrip_queue(struct wl_display *display,
                                                struct wl_event_queue *queue)
{
    struct wl_callback *callback;
    int done = 0, ret = 0;
    wl_display_dispatch_queue_pending(display, queue);

    callback = wl_display_sync(display);
    wl_callback_add_listener(callback, &sync_listener, &done);
    wl_proxy_set_queue((struct wl_proxy *) callback, queue);
    while (ret >= 0 && !done)
        ret = wl_display_dispatch_queue(display, queue);

    return ret;
}
#endif

static void check_fatal_error(struct wl_display *display)
{
    int error = wl_display_get_error(display);

    if (error == 0)
        return;

    fprintf(stderr, "Wayland display got fatal error %i: %s\n", error, strerror(error));

    if (errno != 0)
        fprintf(stderr, "Additionally, errno was set to %i: %s\n", errno, strerror(errno));

    fprintf(stderr, "The display is now unusable, aborting.\n");
    abort();
}

static void
wayland_frame_callback(void *data, struct wl_callback *callback, uint32_t time)
{
    WaylandNativeWindow *surface = static_cast<WaylandNativeWindow *>(data);
    surface->frame();
    wl_callback_destroy(callback);
}

static const struct wl_callback_listener frame_listener = {
    wayland_frame_callback
};

WaylandNativeWindow::WaylandNativeWindow(struct wl_egl_window *window, struct wl_display *display, android_wlegl *wlegl)
    : m_android_wlegl(wlegl)
{
    HYBRIS_TRACE_BEGIN("wayland-platform", "create_window", "");
    this->m_window = window;
    this->m_window->driver_private = (void *) this;
    this->m_display = display;
    this->m_width = window->width;
    this->m_height = window->height;
    this->m_defaultWidth = window->width;
    this->m_defaultHeight = window->height;
    this->m_window->resize_callback = resize_callback;
    this->m_window->destroy_window_callback = destroy_window_callback;
    this->frame_callback = NULL;
    this->wl_queue = wl_display_create_queue(display);
    this->m_format = 1;

	const_cast<int&>(ANativeWindow::minSwapInterval) = 0;
	const_cast<int&>(ANativeWindow::maxSwapInterval) = 1;
    // This is the default as per the EGL documentation
    this->m_swap_interval = 1;

    m_usage=GRALLOC_USAGE_HW_RENDER | GRALLOC_USAGE_HW_TEXTURE;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    m_queueReads = 0;
    m_freeBufs = 0;
    m_damage_rects = NULL;
    m_damage_n_rects = 0;
    m_lastBuffer = 0;
    setBufferCount(3);
    HYBRIS_TRACE_END("wayland-platform", "create_window", "");
}

WaylandNativeWindow::~WaylandNativeWindow()
{
    destroyBuffers();
    if (frame_callback)
        wl_callback_destroy(frame_callback);
    wl_event_queue_destroy(wl_queue);
    if (m_window) {
	    m_window->driver_private = NULL;
	    m_window->resize_callback = NULL;
	    m_window->destroy_window_callback = NULL;
    }
}

void WaylandNativeWindow::frame() {
    HYBRIS_TRACE_BEGIN("wayland-platform", "frame_event", "");

    this->frame_callback = NULL;

    HYBRIS_TRACE_END("wayland-platform", "frame_event", "");
}

// overloads from BaseNativeWindow
int WaylandNativeWindow::setSwapInterval(int interval) {
    TRACE("interval:%i", interval);

    if (interval < 0)
        interval = 0;
    if (interval > 1)
        interval = 1;

    HYBRIS_TRACE_BEGIN("wayland-platform", "swap_interval", "=%d", interval);

    lock();
    m_swap_interval = interval;
    unlock();

    HYBRIS_TRACE_END("wayland-platform", "swap_interval", "");

    return 0;
}

    static void
wl_buffer_release(void *data, struct wl_buffer *buffer)
{
    WaylandNativeWindow *win = static_cast<WaylandNativeWindow *>(data);
    win->releaseBuffer(buffer);
}

static struct wl_buffer_listener wl_buffer_listener = {
    wl_buffer_release
};

void WaylandNativeWindow::releaseBuffer(struct wl_buffer *buffer)
{
    std::list<WaylandNativeWindowBuffer *>::iterator it = posted.begin();

    for (; it != posted.end(); it++)
    {
        if ((*it)->wlbuffer == buffer)
            break;
    }

    if (it != posted.end())
    {
        WaylandNativeWindowBuffer* pwnb = *it;
        posted.erase(it);
        TRACE("released posted buffer: %p", buffer);
        pwnb->busy = 0;
        unlock();
        return;
    }

    it = fronted.begin();

    for (; it != fronted.end(); it++)
    {
        if ((*it)->wlbuffer == buffer)
            break;
    }
    assert(it != fronted.end());



    WaylandNativeWindowBuffer* wnb = *it;
    fronted.erase(it);
    HYBRIS_TRACE_COUNTER("wayland-platform", "fronted.size", "%i", fronted.size());

    for (it = m_bufList.begin(); it != m_bufList.end(); it++)
    {
        if ((*it) == wnb)
            break;
    }
    assert(it != m_bufList.end());
    HYBRIS_TRACE_BEGIN("wayland-platform", "releaseBuffer", "-%p", wnb);
    wnb->busy = 0;

    ++m_freeBufs;
    HYBRIS_TRACE_COUNTER("wayland-platform", "m_freeBufs", "%i", m_freeBufs);
    for (it = m_bufList.begin(); it != m_bufList.end(); it++)
    {
        (*it)->youngest = 0;
    }
    wnb->youngest = 1;


    HYBRIS_TRACE_END("wayland-platform", "releaseBuffer", "-%p", wnb);
}


int WaylandNativeWindow::dequeueBuffer(BaseNativeWindowBuffer **buffer, int *fenceFd){
    HYBRIS_TRACE_BEGIN("wayland-platform", "dequeueBuffer", "");

    WaylandNativeWindowBuffer *wnb=NULL;
    TRACE("%p", buffer);

    lock();
    readQueue(false);

    HYBRIS_TRACE_BEGIN("wayland-platform", "dequeueBuffer_wait_for_buffer", "");

    HYBRIS_TRACE_COUNTER("wayland-platform", "m_freeBufs", "%i", m_freeBufs);

    while (m_freeBufs==0) {
        HYBRIS_TRACE_COUNTER("wayland-platform", "m_freeBufs", "%i", m_freeBufs);
        readQueue(true);
    }

    std::list<WaylandNativeWindowBuffer *>::iterator it = m_bufList.begin();
    for (; it != m_bufList.end(); it++)
    {
         if ((*it)->busy)
             continue;
         if ((*it)->youngest == 1)
             continue;
         break;
    }

    if (it==m_bufList.end()) {
        HYBRIS_TRACE_BEGIN("wayland-platform", "dequeueBuffer_worst_case_scenario", "");
        HYBRIS_TRACE_END("wayland-platform", "dequeueBuffer_worst_case_scenario", "");

        it = m_bufList.begin();
        for (; it != m_bufList.end() && (*it)->busy; it++)
        {}

    }
    if (it==m_bufList.end()) {
        unlock();
        HYBRIS_TRACE_BEGIN("wayland-platform", "dequeueBuffer_no_free_buffers", "");
        HYBRIS_TRACE_END("wayland-platform", "dequeueBuffer_no_free_buffers", "");
        TRACE("%p: no free buffers", buffer);
        return NO_ERROR;
    }

    wnb = *it;
    assert(wnb!=NULL);
    HYBRIS_TRACE_END("wayland-platform", "dequeueBuffer_wait_for_buffer", "");

    /* If the buffer doesn't match the window anymore, re-allocate */
    if (wnb->width != m_width || wnb->height != m_height
        || wnb->format != m_format || wnb->usage != m_usage)
    {
        TRACE("wnb:%p,win:%p %i,%i %i,%i x%x,x%x x%x,x%x",
            wnb,m_window,
            wnb->width,m_width, wnb->height,m_height,
            wnb->format,m_format, wnb->usage,m_usage);
        destroyBuffer(wnb);
        m_bufList.erase(it);
        wnb = addBuffer();
    }

    wnb->busy = 1;
    *buffer = wnb;
    queue.push_back(wnb);
    --m_freeBufs;

    HYBRIS_TRACE_COUNTER("wayland-platform", "m_freeBufs", "%i", m_freeBufs);
    HYBRIS_TRACE_BEGIN("wayland-platform", "dequeueBuffer_gotBuffer", "-%p", wnb);
    HYBRIS_TRACE_END("wayland-platform", "dequeueBuffer_gotBuffer", "-%p", wnb);
    HYBRIS_TRACE_END("wayland-platform", "dequeueBuffer_wait_for_buffer", "");

    unlock();
    return NO_ERROR;
}

int WaylandNativeWindow::lockBuffer(BaseNativeWindowBuffer* buffer){
    WaylandNativeWindowBuffer *wnb = (WaylandNativeWindowBuffer*) buffer;
    (void)wnb;
    HYBRIS_TRACE_BEGIN("wayland-platform", "lockBuffer", "-%p", wnb);
    HYBRIS_TRACE_END("wayland-platform", "lockBuffer", "-%p", wnb);
    return NO_ERROR;
}

int WaylandNativeWindow::postBuffer(ANativeWindowBuffer* buffer)
{
    TRACE("");
    WaylandNativeWindowBuffer *wnb = NULL;

    lock();
    std::list<WaylandNativeWindowBuffer *>::iterator it = post_registered.begin();
    for (; it != post_registered.end(); it++)
    {
        if ((*it)->other == buffer)
        {
            wnb = (*it);
            break;
        }
    }
    unlock();
    if (!wnb)
    {
        wnb = new WaylandNativeWindowBuffer(buffer);

        wnb->common.incRef(&wnb->common);
        buffer->common.incRef(&buffer->common);
    }

    int ret = 0;

    lock();
    wnb->busy = 1;
    ret = readQueue(false);

    if (ret < 0) {
        unlock();
        return ret;
    }

    if (wnb->wlbuffer == NULL)
    {
        wnb->wlbuffer_from_native_handle(m_android_wlegl, m_display, wl_queue);
        TRACE("%p add listener with %p inside", wnb, wnb->wlbuffer);
        wl_buffer_add_listener(wnb->wlbuffer, &wl_buffer_listener, this);
        wl_proxy_set_queue((struct wl_proxy *) wnb->wlbuffer, this->wl_queue);
        post_registered.push_back(wnb);
    }
    TRACE("%p DAMAGE AREA: %dx%d", wnb, wnb->width, wnb->height);
    wl_surface_attach(m_window->surface, wnb->wlbuffer, 0, 0);
    wl_surface_damage(m_window->surface, 0, 0, wnb->width, wnb->height);
    wl_surface_commit(m_window->surface);
    wl_display_flush(m_display);

    posted.push_back(wnb);
    unlock();

    return NO_ERROR;
}

int WaylandNativeWindow::readQueue(bool block)
{
    int ret = 0;

    if (++m_queueReads == 1) {
        if (block) {
            ret = wl_display_dispatch_queue(m_display, wl_queue);
        } else {
            ret = wl_display_dispatch_queue_pending(m_display, wl_queue);
        }

        // all threads waiting on the false branch will wake and return now, so we
        // can safely set m_queueReads to 0 here instead of relying on every thread
        // to decrement it. This prevents a race condition when a thread enters readQueue()
        // before the one in this thread returns.
        // The new thread would go in the false branch, and there would be no thread in the
        // true branch, blocking the new thread and any other that will call readQueue in
        // the future.
        m_queueReads = 0;

        pthread_cond_broadcast(&cond);

        if (ret < 0) {
            TRACE("wl_display_dispatch_queue returned an error");
            check_fatal_error(m_display);
            return ret;
        }
    } else if (block) {
        while (m_queueReads > 0) {
            pthread_cond_wait(&cond, &mutex);
        }
    }

    return ret;
}

void WaylandNativeWindow::prepareSwap(EGLint *damage_rects, EGLint damage_n_rects)
{
    lock();
    m_damage_rects = damage_rects;
    m_damage_n_rects = damage_n_rects;
    unlock();
}

void WaylandNativeWindow::finishSwap()
{
    int ret = 0;
    lock();
    if (!m_window) {
        unlock();
        return;
    }

    WaylandNativeWindowBuffer *wnb = queue.front();
    if (!wnb) {
        wnb = m_lastBuffer;
    } else {
        queue.pop_front();
    }
    assert(wnb);
    m_lastBuffer = wnb;
    wnb->busy = 1;

    ret = readQueue(false);
    if (this->frame_callback) {
        do {
            ret = readQueue(true);
        } while (this->frame_callback && ret != -1);
    }
    if (ret < 0) {
        HYBRIS_TRACE_END("wayland-platform", "queueBuffer_wait_for_frame_callback", "-%p", wnb);
        unlock();
        return;
    }

    if (wnb->wlbuffer == NULL)
    {
        wnb->init(m_android_wlegl, m_display, wl_queue);
        TRACE("%p add listener with %p inside", wnb, wnb->wlbuffer);
        wl_buffer_add_listener(wnb->wlbuffer, &wl_buffer_listener, this);
        wl_proxy_set_queue((struct wl_proxy *) wnb->wlbuffer, this->wl_queue);
    }

    if (m_swap_interval > 0) {
        this->frame_callback = wl_surface_frame(m_window->surface);
        wl_callback_add_listener(this->frame_callback, &frame_listener, this);
        wl_proxy_set_queue((struct wl_proxy *) this->frame_callback, this->wl_queue);
    }

    wl_surface_attach(m_window->surface, wnb->wlbuffer, 0, 0);
    wl_surface_damage(m_window->surface, 0, 0, wnb->width, wnb->height);
    wl_surface_commit(m_window->surface);
    // Some compositors, namely Weston, queue buffer release events instead
    // of sending them immediately.  If a frame event is used, this should
    // not be a problem.  Without a frame event, we need to send a sync
    // request to ensure that they get flushed.
    wl_callback_destroy(wl_display_sync(m_display));
    wl_display_flush(m_display);
    fronted.push_back(wnb);

    m_window->attached_width = wnb->width;
    m_window->attached_height = wnb->height;

    m_damage_rects = NULL;
    m_damage_n_rects = 0;
    unlock();
}

static int debugenvchecked = 0;

int WaylandNativeWindow::queueBuffer(BaseNativeWindowBuffer* buffer, int fenceFd)
{
    WaylandNativeWindowBuffer *wnb = (WaylandNativeWindowBuffer*) buffer;

    HYBRIS_TRACE_BEGIN("wayland-platform", "queueBuffer", "-%p", wnb);
    lock();

    if (debugenvchecked == 0)
    {
        if (getenv("HYBRIS_WAYLAND_DUMP_BUFFERS") != NULL)
            debugenvchecked = 2;
        else
            debugenvchecked = 1;
    }
    if (debugenvchecked == 2)
    {
        HYBRIS_TRACE_BEGIN("wayland-platform", "queueBuffer_dumping_buffer", "-%p", wnb);
        hybris_dump_buffer_to_file(wnb->getNativeBuffer());
        HYBRIS_TRACE_END("wayland-platform", "queueBuffer_dumping_buffer", "-%p", wnb);

    }

#if ANDROID_VERSION_MAJOR>=4 && ANDROID_VERSION_MINOR>=2 || ANDROID_VERSION_MAJOR>=5
    HYBRIS_TRACE_BEGIN("wayland-platform", "queueBuffer_waiting_for_fence", "-%p", wnb);
    if (fenceFd >= 0)
    {
        sync_wait(fenceFd, -1);
        close(fenceFd);
    }
    HYBRIS_TRACE_END("wayland-platform", "queueBuffer_waiting_for_fence", "-%p", wnb);
#endif

    HYBRIS_TRACE_COUNTER("wayland-platform", "fronted.size", "%i", fronted.size());
    HYBRIS_TRACE_END("wayland-platform", "queueBuffer", "-%p", wnb);
    unlock();

    return NO_ERROR;
}

int WaylandNativeWindow::cancelBuffer(BaseNativeWindowBuffer* buffer, int fenceFd){
    std::list<WaylandNativeWindowBuffer *>::iterator it;
    WaylandNativeWindowBuffer *wnb = (WaylandNativeWindowBuffer*) buffer;

    lock();
    HYBRIS_TRACE_BEGIN("wayland-platform", "cancelBuffer", "-%p", wnb);

    /* Check first that it really is our buffer */
    for (it = m_bufList.begin(); it != m_bufList.end(); it++)
    {
        if ((*it) == wnb)
            break;
    }
    assert(it != m_bufList.end());

    wnb->busy = 0;
    ++m_freeBufs;
    HYBRIS_TRACE_COUNTER("wayland-platform", "m_freeBufs", "%i", m_freeBufs);

    for (it = m_bufList.begin(); it != m_bufList.end(); it++)
    {
        (*it)->youngest = 0;
    }
    wnb->youngest = 1;

    if (m_queueReads != 0) {
        // Some thread is waiting on wl_display_dispatch_queue(), possibly waiting for a wl_buffer.release
        // event. Since we have now cancelled a buffer push an artificial event so that the dispatch returns
        // and the thread can notice the cancelled buffer. This means there is a delay of one roundtrip,
        // but I don't see other solution except having one dedicated thread for calling wl_display_dispatch_queue().
        wl_callback_destroy(wl_display_sync(m_display));
    }

    HYBRIS_TRACE_END("wayland-platform", "cancelBuffer", "-%p", wnb);
    unlock();

    return 0;
}

unsigned int WaylandNativeWindow::width() const {
    TRACE("value:%i", m_width);
    return m_width;
}

unsigned int WaylandNativeWindow::height() const {
    TRACE("value:%i", m_height);
    return m_height;
}

unsigned int WaylandNativeWindow::format() const {
    TRACE("value:%i", m_format);
    return m_format;
}

unsigned int WaylandNativeWindow::defaultWidth() const {
    TRACE("value:%i", m_defaultWidth);
    return m_defaultWidth;
}

unsigned int WaylandNativeWindow::defaultHeight() const {
    TRACE("value:%i", m_defaultHeight);
    return m_defaultHeight;
}

unsigned int WaylandNativeWindow::queueLength() const {
    TRACE("WARN: stub");
    return 1;
}

unsigned int WaylandNativeWindow::type() const {
    TRACE("");
#if ANDROID_VERSION_MAJOR>=4 && ANDROID_VERSION_MINOR>=3 || ANDROID_VERSION_MAJOR>=5
    /* https://android.googlesource.com/platform/system/core/+/bcfa910611b42018db580b3459101c564f802552%5E!/ */
    return NATIVE_WINDOW_SURFACE;
#else
    return NATIVE_WINDOW_SURFACE_TEXTURE_CLIENT;
#endif
}

unsigned int WaylandNativeWindow::transformHint() const {
    TRACE("WARN: stub");
    return 0;
}

/*
 * returns the current usage of this window
 */
unsigned int WaylandNativeWindow::getUsage() const {
    return m_usage;
}

int WaylandNativeWindow::setBuffersFormat(int format) {
    if (format != m_format)
    {
        TRACE("old-format:x%x new-format:x%x", m_format, format);
        m_format = format;
        /* Buffers will be re-allocated when dequeued */
    } else {
        TRACE("format:x%x", format);
    }
    return NO_ERROR;
}

void WaylandNativeWindow::destroyBuffer(WaylandNativeWindowBuffer* wnb)
{
    TRACE("wnb:%p", wnb);

    assert(wnb != NULL);

    int ret = 0;
    while (ret != -1 && wnb->creation_callback)
        ret = wl_display_dispatch_queue(m_display, wl_queue);

    if (wnb->creation_callback) {
        wl_callback_destroy(wnb->creation_callback);
        wnb->creation_callback = NULL;
    }

    if (wnb->wlbuffer)
        wl_buffer_destroy(wnb->wlbuffer);
    wnb->wlbuffer = NULL;
    wnb->common.decRef(&wnb->common);
    m_freeBufs--;
}

void WaylandNativeWindow::destroyBuffers()
{
    TRACE("");

    std::list<WaylandNativeWindowBuffer*>::iterator it = m_bufList.begin();
    for (; it!=m_bufList.end(); ++it)
    {
        destroyBuffer(*it);
        it = m_bufList.erase(it);
    }
    m_bufList.clear();
    m_freeBufs = 0;
}

WaylandNativeWindowBuffer *WaylandNativeWindow::addBuffer() {

    WaylandNativeWindowBuffer *wnb;

#ifndef HYBRIS_NO_SERVER_SIDE_BUFFERS
    wnb = new ServerWaylandBuffer(m_width, m_height, m_format, m_usage, m_android_wlegl, wl_queue);
    wl_display_roundtrip_queue(m_display, wl_queue);
#else
    wnb = new ClientWaylandBuffer(m_width, m_height, m_format, m_usage);
#endif
    m_bufList.push_back(wnb);
    ++m_freeBufs;

    TRACE("wnb:%p width:%i height:%i format:x%x usage:x%x",
         wnb, wnb->width, wnb->height, wnb->format, wnb->usage);

    return wnb;
}


int WaylandNativeWindow::setBufferCount(int cnt) {
    TRACE("cnt:%d", cnt);

    if ((int)m_bufList.size() == cnt)
        return NO_ERROR;

    lock();

    if ((int)m_bufList.size() > cnt) {
        /* Decreasing buffer count, remove from beginning */
        std::list<WaylandNativeWindowBuffer*>::iterator it = m_bufList.begin();
        for (int i = 0; i <= (int)m_bufList.size() - cnt; i++ )
        {
            destroyBuffer(*it);
            ++it;
            m_bufList.pop_front();
        }

    } else {
        /* Increasing buffer count, start from current size */
        for (int i = (int)m_bufList.size(); i < cnt; i++)
            (void)addBuffer();

    }

    unlock();

    return NO_ERROR;
}




int WaylandNativeWindow::setBuffersDimensions(int width, int height) {
    if (m_width != width || m_height != height)
    {
        TRACE("old-size:%ix%i new-size:%ix%i", m_width, m_height, width, height);
        m_width = width;
        m_height = height;
        /* Buffers will be re-allocated when dequeued */
    } else {
        TRACE("size:%ix%i", width, height);
    }
    return NO_ERROR;
}

int WaylandNativeWindow::setUsage(int usage) {
    if ((usage | GRALLOC_USAGE_HW_TEXTURE) != m_usage)
    {
        TRACE("old-usage:x%x new-usage:x%x", m_usage, usage);
        m_usage = usage | GRALLOC_USAGE_HW_TEXTURE;
        /* Buffers will be re-allocated when dequeued */
    } else {
        TRACE("usage:x%x", usage);
    }
    return NO_ERROR;
}

#ifdef HYBRIS_NO_SERVER_SIDE_BUFFERS

void ClientWaylandBuffer::init(struct android_wlegl *android_wlegl,
                                     struct wl_display *display,
                                     struct wl_event_queue *queue)
{
    wlbuffer_from_native_handle(android_wlegl, display, queue);
}

#else // HYBRIS_NO_SERVER_SIDE_BUFFERS

static void ssb_ints(void *data, android_wlegl_server_buffer_handle *, wl_array *ints)
{
    ServerWaylandBuffer *wsb = static_cast<ServerWaylandBuffer *>(data);
    wl_array_copy(&wsb->ints, ints);
}

static void ssb_fd(void *data, android_wlegl_server_buffer_handle *, int fd)
{
    ServerWaylandBuffer *wsb = static_cast<ServerWaylandBuffer *>(data);
    int *ptr = (int *)wl_array_add(&wsb->fds, sizeof(int));
    *ptr = fd;
}

static void ssb_buffer(void *data, android_wlegl_server_buffer_handle *, wl_buffer *buffer, int32_t format, int32_t stride)
{
    ServerWaylandBuffer *wsb = static_cast<ServerWaylandBuffer *>(data);

    native_handle_t *native;
    int numFds = wsb->fds.size / sizeof(int);
    int numInts = wsb->ints.size / sizeof(int32_t);

    native = native_handle_create(numFds, numInts);

    memcpy(&native->data[0], wsb->fds.data, wsb->fds.size);
    memcpy(&native->data[numFds], wsb->ints.data, wsb->ints.size);
    /* ownership of fds passed to native_handle_t */
    wsb->fds.size = 0;

    wsb->handle = (buffer_handle_t) native;
    wsb->format = format;
    wsb->stride = stride;

    int ret = hybris_gralloc_retain(wsb->handle);
    if (ret) {
        fprintf(stderr,"failed to register buffer\n");
        return;
    }

    wsb->common.incRef(&wsb->common);
    wsb->m_buf = buffer;
}

static const struct android_wlegl_server_buffer_handle_listener server_handle_listener = {
    ssb_fd,
    ssb_ints,
    ssb_buffer,
};

ServerWaylandBuffer::ServerWaylandBuffer(unsigned int w, unsigned int h, int f, int u, android_wlegl *android_wlegl, struct wl_event_queue *queue)
                   : WaylandNativeWindowBuffer()
                   , m_buf(0)
{
    ANativeWindowBuffer::width = w;
    ANativeWindowBuffer::height = h;
    usage = u;

    wl_array_init(&ints);
    wl_array_init(&fds);

    android_wlegl_server_buffer_handle *ssb = android_wlegl_get_server_buffer_handle(android_wlegl, width, height, f, u);
    wl_proxy_set_queue((struct wl_proxy *) ssb, queue);
    android_wlegl_server_buffer_handle_add_listener(ssb, &server_handle_listener, this);
}

ServerWaylandBuffer::~ServerWaylandBuffer()
{
    if (m_buf)
        wl_buffer_destroy(m_buf);

    hybris_gralloc_release(handle, 1);
    wl_array_release(&ints);
    wl_array_release(&fds);
}

void ServerWaylandBuffer::init(android_wlegl *, wl_display *, wl_event_queue *queue)
{
    wlbuffer = m_buf;
    m_buf = 0;
    wl_proxy_set_queue((struct wl_proxy *) wlbuffer, queue);
}

#endif // HYBRIS_NO_SERVER_SIDE_BUFFERS

// vim: noai:ts=4:sw=4:ss=4:expandtab
