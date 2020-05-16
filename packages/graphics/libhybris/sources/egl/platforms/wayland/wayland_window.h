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

#ifndef Wayland_WINDOW_H
#define Wayland_WINDOW_H
#include "nativewindowbase.h"
#include <linux/fb.h>

#include <hybris/gralloc/gralloc.h>

extern "C" {

#include <wayland-client.h>
#include <wayland-egl.h>
#include "wayland-android-client-protocol.h"
#include <pthread.h>
}

#include <list>
#include <deque>

class WaylandNativeWindowBuffer : public BaseNativeWindowBuffer
{
public:
    WaylandNativeWindowBuffer() : wlbuffer(0), busy(0), youngest(0), other(0), creation_callback(0) {}
    WaylandNativeWindowBuffer(ANativeWindowBuffer *other)
    {
        ANativeWindowBuffer::width = other->width;
        ANativeWindowBuffer::height = other->height;
        ANativeWindowBuffer::format = other->format;
        ANativeWindowBuffer::usage = other->usage;
        ANativeWindowBuffer::handle = other->handle;
        ANativeWindowBuffer::stride = other->stride;
        this->wlbuffer = NULL;
        this->creation_callback = NULL;
        this->busy = 0;
        this->other = other;
        this->youngest = 0;
    }

    struct wl_buffer *wlbuffer;
    int busy;
    int youngest;
    ANativeWindowBuffer *other;
    struct wl_callback *creation_callback;

    void wlbuffer_from_native_handle(struct android_wlegl *android_wlegl,
                                     struct wl_display *display,
                                     struct wl_event_queue *queue);

    virtual void init(struct android_wlegl *android_wlegl,
                      struct wl_display *display,
                      struct wl_event_queue *queue) {}
};

#ifdef HYBRIS_NO_SERVER_SIDE_BUFFERS

class ClientWaylandBuffer : public WaylandNativeWindowBuffer
{
friend class WaylandNativeWindow;
protected:
    ClientWaylandBuffer()
        : {}

    ClientWaylandBuffer(    unsigned int width,
                            unsigned int height,
                            unsigned int format,
                            unsigned int usage)
    {
        // Base members
        ANativeWindowBuffer::width = width;
        ANativeWindowBuffer::height = height;
        ANativeWindowBuffer::format = format;
        ANativeWindowBuffer::usage = usage;
        this->wlbuffer = NULL;
        this->creation_callback = NULL;
        this->busy = 0;
        this->other = NULL;
        int alloc_ok = hybris_gralloc_allocate(this->width ? this->width : 1, this->height ? this->height : 1,
                this->format, this->usage,
                &this->handle, &this->stride);
        assert(alloc_ok == 0);
        this->youngest = 0;
        this->common.incRef(&this->common);
    }

    ~ClientWaylandBuffer()
    {
        hybris_gralloc_release(this->handle, 1);
    }

    void init(struct android_wlegl *android_wlegl,
                                     struct wl_display *display,
                                     struct wl_event_queue *queue);

protected:
    void* vaddr;

public:

};

#else

class ServerWaylandBuffer : public WaylandNativeWindowBuffer
{
public:
    ServerWaylandBuffer(unsigned int w, unsigned int h, int format, int usage, android_wlegl *android_wlegl, struct wl_event_queue *queue);
    ~ServerWaylandBuffer();
    void init(struct android_wlegl *android_wlegl,
                                     struct wl_display *display,
                                     struct wl_event_queue *queue);

    struct wl_array ints;
    struct wl_array fds;
    wl_buffer *m_buf;
};

#endif // HYBRIS_NO_SERVER_SIDE_BUFFERS

class WaylandNativeWindow : public BaseNativeWindow {
public:
    WaylandNativeWindow(struct wl_egl_window *win, struct wl_display *display, android_wlegl *wlegl);
    ~WaylandNativeWindow();

    void lock();
    void unlock();
    void frame();
    void resize(unsigned int width, unsigned int height);
    void releaseBuffer(struct wl_buffer *buffer);
    int postBuffer(ANativeWindowBuffer *buffer);

    virtual int setSwapInterval(int interval);
    void prepareSwap(EGLint *damage_rects, EGLint damage_n_rects);
    void finishSwap();

    static void sync_callback(void *data, struct wl_callback *callback, uint32_t serial);
    static void registry_handle_global(void *data, struct wl_registry *registry, uint32_t name,
                       const char *interface, uint32_t version);
    static void resize_callback(struct wl_egl_window *egl_window, void *);
    static void destroy_window_callback(void *data);
    struct wl_event_queue *wl_queue;

protected:
    // overloads from BaseNativeWindow
    virtual int dequeueBuffer(BaseNativeWindowBuffer **buffer, int *fenceFd);
    virtual int lockBuffer(BaseNativeWindowBuffer* buffer);
    virtual int queueBuffer(BaseNativeWindowBuffer* buffer, int fenceFd);
    virtual int cancelBuffer(BaseNativeWindowBuffer* buffer, int fenceFd);
    virtual unsigned int type() const;
    virtual unsigned int width() const;
    virtual unsigned int height() const;
    virtual unsigned int format() const;
    virtual unsigned int defaultWidth() const;
    virtual unsigned int defaultHeight() const;
    virtual unsigned int queueLength() const;
    virtual unsigned int transformHint() const;
    virtual unsigned int getUsage() const;
    // perform calls
    virtual int setUsage(int usage);
    virtual int setBuffersFormat(int format);
    virtual int setBuffersDimensions(int width, int height);
    virtual int setBufferCount(int cnt);

private:
    WaylandNativeWindowBuffer *addBuffer();
    void destroyBuffer(WaylandNativeWindowBuffer *);
    void destroyBuffers();
    int readQueue(bool block);

    std::list<WaylandNativeWindowBuffer *> m_bufList;
    std::list<WaylandNativeWindowBuffer *> fronted;
    std::list<WaylandNativeWindowBuffer *> posted;
    std::list<WaylandNativeWindowBuffer *> post_registered;
    std::deque<WaylandNativeWindowBuffer *> queue;
    struct wl_egl_window *m_window;
    struct wl_display *m_display;
    WaylandNativeWindowBuffer *m_lastBuffer;
    int m_width;
    int m_height;
    int m_format;
    unsigned int m_defaultWidth;
    unsigned int m_defaultHeight;
    unsigned int m_usage;
    struct android_wlegl *m_android_wlegl;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int m_queueReads;
    int m_freeBufs;
    EGLint *m_damage_rects, m_damage_n_rects;
    struct wl_callback *frame_callback;
    int m_swap_interval;
#if WAYLAND_VERSION_MAJOR == 0 || (WAYLAND_VERSION_MAJOR == 1 && WAYLAND_VERSION_MINOR < 6)
    static int wl_roundtrip_queue(struct wl_display *display,
                                  struct wl_event_queue *queue);
#endif
};

#endif
// vim: noai:ts=4:sw=4:ss=4:expandtab
