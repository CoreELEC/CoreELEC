/*
 * Copyright (C) 2013 libhybris
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <android-config.h>
#include "fbdev_window.h"
#include "logging.h"

#include <errno.h>
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#if ANDROID_VERSION_MAJOR>=4 && ANDROID_VERSION_MINOR>=2 || ANDROID_VERSION_MAJOR>=5
extern "C" {
#include <sync/sync.h>
};
#endif

#include <hybris/gralloc/gralloc.h>

#define FRAMEBUFFER_PARTITIONS 2

static pthread_cond_t _cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t _mutex = PTHREAD_MUTEX_INITIALIZER;


FbDevNativeWindowBuffer::FbDevNativeWindowBuffer(unsigned int width,
                            unsigned int height,
                            unsigned int format,
                            unsigned int usage)
{
    ANativeWindowBuffer::width  = width;
    ANativeWindowBuffer::height = height;
    ANativeWindowBuffer::format = format;
    ANativeWindowBuffer::usage  = usage;
    busy = 0;
    status = 0;

    hybris_gralloc_allocate(width, height, format, usage, &handle, (uint32_t*)&stride);

    TRACE("width=%d height=%d stride=%d format=x%x usage=x%x status=%s this=%p",
        width, height, stride, format, usage, strerror(-status), this);
}



FbDevNativeWindowBuffer::~FbDevNativeWindowBuffer()
{
    TRACE("%p", this);
    hybris_gralloc_release(handle, 1);
}


////////////////////////////////////////////////////////////////////////////////
FbDevNativeWindow::FbDevNativeWindow()
{
    m_bufFormat = hybris_gralloc_fbdev_format();
    m_usage = GRALLOC_USAGE_HW_FB;
    m_bufferCount = 0;
    m_allocateBuffers = true;

#if ANDROID_VERSION_MAJOR>=4 && ANDROID_VERSION_MINOR>=2 || ANDROID_VERSION_MAJOR>=5
    if (hybris_gralloc_fbdev_framebuffer_count() > 0)
        setBufferCount(hybris_gralloc_fbdev_framebuffer_count());
    else
        setBufferCount(FRAMEBUFFER_PARTITIONS);
#else
    setBufferCount(FRAMEBUFFER_PARTITIONS);
#endif
}




FbDevNativeWindow::~FbDevNativeWindow()
{
    destroyBuffers();
}



void FbDevNativeWindow::destroyBuffers()
{
    TRACE("");

    std::list<FbDevNativeWindowBuffer*>::iterator it = m_bufList.begin();
    for (; it!=m_bufList.end(); ++it)
    {
        FbDevNativeWindowBuffer* fbnb = *it;
        fbnb->common.decRef(&fbnb->common);
    }
    m_bufList.clear();
    m_freeBufs = 0;
    m_frontBuf = NULL;
}




/*
 * Set the swap interval for this surface.
 *
 * Returns 0 on success or -errno on error.
 */
int FbDevNativeWindow::setSwapInterval(int interval)
{
    TRACE("interval=%i", interval);
    return hybris_gralloc_fbdev_setSwapInterval(interval);
}


/*
 * Hook called by EGL to acquire a buffer. This call may block if no
 * buffers are available.
 *
 * The window holds a reference to the buffer between dequeueBuffer and
 * either queueBuffer or cancelBuffer, so clients only need their own
 * reference if they might use the buffer after queueing or canceling it.
 * Holding a reference to a buffer after queueing or canceling it is only
 * allowed if a specific buffer count has been set.
 *
 * The libsync fence file descriptor returned in the int pointed to by the
 * fenceFd argument will refer to the fence that must signal before the
 * dequeued buffer may be written to.  A value of -1 indicates that the
 * caller may access the buffer immediately without waiting on a fence.  If
 * a valid file descriptor is returned (i.e. any value except -1) then the
 * caller is responsible for closing the file descriptor.
 *
 * Returns 0 on success or -errno on error.
 */
int FbDevNativeWindow::dequeueBuffer(BaseNativeWindowBuffer** buffer, int *fenceFd)
{
    HYBRIS_TRACE_BEGIN("fbdev-platform", "dequeueBuffer", "");
    FbDevNativeWindowBuffer* fbnb=NULL;

    pthread_mutex_lock(&_mutex);

    if (m_allocateBuffers)
        reallocateBuffers();

    HYBRIS_TRACE_BEGIN("fbdev-platform", "dequeueBuffer-wait", "");
#if defined(DEBUG)

    if (m_frontBuf)
        TRACE("Status: Has front buf %p", m_frontBuf);

    std::list<FbDevNativeWindowBuffer*>::iterator cit = m_bufList.begin();
    for (; cit != m_bufList.end(); ++cit)
    {
        TRACE("Status: Buffer %p with busy %i\n", (*cit), (*cit)->busy);
    }
#endif

    while (m_freeBufs==0)
    {
        pthread_cond_wait(&_cond, &_mutex);
    }

    while (1)
    {
        std::list<FbDevNativeWindowBuffer*>::iterator it = m_bufList.begin();
        for (; it != m_bufList.end(); ++it)
        {
            if (*it==m_frontBuf)
                continue;
            if ((*it)->busy==0)
            {
                TRACE("Found a free non-front buffer");
                break;
            }
        }

        if (it == m_bufList.end())
        {
#if ANDROID_VERSION_MAJOR<=4 && ANDROID_VERSION_MINOR<2
            /*
             * This is acceptable in case you are on a stack that calls lock() before starting to render into buffer
             * When you are using fences (>= 2) you'll be waiting on the fence to signal instead. 
             * 
             * This optimization allows eglSwapBuffers to return and you can begin to utilize the GPU for rendering. 
             * The actual lock() probably first comes at glFlush/eglSwapBuffers
            */
            if (m_frontBuf && m_frontBuf->busy == 0)
            {
                TRACE("Used front buffer as buffer");
                fbnb = m_frontBuf;
                break;
            }
#endif
            // have to wait once again
            pthread_cond_wait(&_cond, &_mutex);
            continue;
        }

        fbnb = *it;
        break;
    }

    HYBRIS_TRACE_END("fbdev-platform", "dequeueBuffer-wait", "");
    assert(fbnb!=NULL);
    fbnb->busy = 1;
    m_freeBufs--;

    *buffer = fbnb;
    *fenceFd = -1;

    TRACE("%lu DONE --> %p", pthread_self(), fbnb);
    pthread_mutex_unlock(&_mutex);
    HYBRIS_TRACE_END("fbdev-platform", "dequeueBuffer", "");
    return 0;
}

/*
 * Hook called by EGL when modifications to the render buffer are done.
 * This unlocks and post the buffer.
 *
 * The window holds a reference to the buffer between dequeueBuffer and
 * either queueBuffer or cancelBuffer, so clients only need their own
 * reference if they might use the buffer after queueing or canceling it.
 * Holding a reference to a buffer after queueing or canceling it is only
 * allowed if a specific buffer count has been set.
 *
 * The fenceFd argument specifies a libsync fence file descriptor for a
 * fence that must signal before the buffer can be accessed.  If the buffer
 * can be accessed immediately then a value of -1 should be used.  The
 * caller must not use the file descriptor after it is passed to
 * queueBuffer, and the ANativeWindow implementation is responsible for
 * closing it.
 *
 * Returns 0 on success or -errno on error.
 */
int FbDevNativeWindow::queueBuffer(BaseNativeWindowBuffer* buffer, int fenceFd)
{
 
    FbDevNativeWindowBuffer* fbnb = (FbDevNativeWindowBuffer*) buffer;

    HYBRIS_TRACE_BEGIN("fbdev-platform", "queueBuffer", "-%p", fbnb);

    pthread_mutex_lock(&_mutex);

    assert(fbnb->busy==1);

    fbnb->busy = 2;

    pthread_mutex_unlock(&_mutex);

#if ANDROID_VERSION_MAJOR>=4 && ANDROID_VERSION_MINOR>=2 || ANDROID_VERSION_MAJOR>=5
    HYBRIS_TRACE_BEGIN("fbdev-platform", "queueBuffer_waiting_for_fence", "-%p", fbnb);
    if (fenceFd >= 0)
    {
        sync_wait(fenceFd, -1);
        close(fenceFd);
    }
    HYBRIS_TRACE_END("fbdev-platform", "queueBuffer_waiting_for_fence", "-%p", fbnb);
#endif

    HYBRIS_TRACE_BEGIN("fbdev-platform", "queueBuffer-post", "-%p", fbnb);

    int rv = hybris_gralloc_fbdev_post(fbnb->handle);
    if (rv!=0)
    {
        fprintf(stderr,"ERROR: fb->post(%s)\n",strerror(-rv));
    }
    HYBRIS_TRACE_END("fbdev-platform", "queueBuffer-post", "-%p", fbnb);

    pthread_mutex_lock(&_mutex);

    fbnb->busy=0;
    m_frontBuf = fbnb;

    m_freeBufs++;

    TRACE("%lu %p %p",pthread_self(), m_frontBuf, fbnb);

    pthread_cond_signal(&_cond);
    pthread_mutex_unlock(&_mutex);

    HYBRIS_TRACE_END("fbdev-platform", "queueBuffer", "-%p", fbnb);
    return rv;
}


/*
 * Hook used to cancel a buffer that has been dequeued.
 * No synchronization is performed between dequeue() and cancel(), so
 * either external synchronization is needed, or these functions must be
 * called from the same thread.
 *
 * The window holds a reference to the buffer between dequeueBuffer and
 * either queueBuffer or cancelBuffer, so clients only need their own
 * reference if they might use the buffer after queueing or canceling it.
 * Holding a reference to a buffer after queueing or canceling it is only
 * allowed if a specific buffer count has been set.
 *
 * The fenceFd argument specifies a libsync fence file decsriptor for a
 * fence that must signal before the buffer can be accessed.  If the buffer
 * can be accessed immediately then a value of -1 should be used.
 *
 * Note that if the client has not waited on the fence that was returned
 * from dequeueBuffer, that same fence should be passed to cancelBuffer to
 * ensure that future uses of the buffer are preceded by a wait on that
 * fence.  The caller must not use the file descriptor after it is passed
 * to cancelBuffer, and the ANativeWindow implementation is responsible for
 * closing it.
 *
 * Returns 0 on success or -errno on error.
 */
int FbDevNativeWindow::cancelBuffer(BaseNativeWindowBuffer* buffer, int fenceFd)
{
    TRACE("");
    FbDevNativeWindowBuffer* fbnb = (FbDevNativeWindowBuffer*)buffer;

    pthread_mutex_lock(&_mutex);

    fbnb->busy=0;

    m_freeBufs++;

    pthread_cond_signal(&_cond);
    pthread_mutex_unlock(&_mutex);

    return 0;
}



int FbDevNativeWindow::lockBuffer(BaseNativeWindowBuffer* buffer)
{

    FbDevNativeWindowBuffer* fbnb = (FbDevNativeWindowBuffer*)buffer;

    HYBRIS_TRACE_BEGIN("fbdev-platform", "lockBuffer", "-%p", fbnb);

    pthread_mutex_lock(&_mutex);

    // wait that the buffer we're locking is not front anymore
    while (m_frontBuf==fbnb)
    {
        TRACE("waiting %p %p", m_frontBuf, fbnb);
        pthread_cond_wait(&_cond, &_mutex);
    }

    pthread_mutex_unlock(&_mutex);
    HYBRIS_TRACE_END("fbdev-platform", "lockBuffer", "-%p", fbnb);
    return NO_ERROR;
}


/*
 * see NATIVE_WINDOW_FORMAT
 */
unsigned int FbDevNativeWindow::width() const
{
    unsigned int rv = hybris_gralloc_fbdev_width();
    TRACE("width=%i", rv);
    return rv;
}


/*
 * see NATIVE_WINDOW_HEIGHT
 */
unsigned int FbDevNativeWindow::height() const
{
    unsigned int rv = hybris_gralloc_fbdev_height();
    TRACE("height=%i", rv);
    return rv;
}


/*
 * see NATIVE_WINDOW_FORMAT
 */
unsigned int FbDevNativeWindow::format() const
{
    unsigned int rv = hybris_gralloc_fbdev_format();
    TRACE("format=x%x", rv);
    return rv;
}


/*
 * Default width and height of ANativeWindow buffers, these are the
 * dimensions of the window buffers irrespective of the
 * NATIVE_WINDOW_SET_BUFFERS_DIMENSIONS call and match the native window
 * size unless overridden by NATIVE_WINDOW_SET_BUFFERS_USER_DIMENSIONS.
 */
/*
 * see NATIVE_WINDOW_DEFAULT_HEIGHT
 */
unsigned int FbDevNativeWindow::defaultHeight() const
{
    unsigned int rv = hybris_gralloc_fbdev_height();
    TRACE("height=%i", rv);
    return rv;
}


/*
 * see BaseNativeWindow::_query(NATIVE_WINDOW_DEFAULT_WIDTH)
 */
unsigned int FbDevNativeWindow::defaultWidth() const
{
    unsigned int rv = hybris_gralloc_fbdev_width();
    TRACE("width=%i", rv);
    return rv;
}


/*
 * see NATIVE_WINDOW_QUEUES_TO_WINDOW_COMPOSER
 */
unsigned int FbDevNativeWindow::queueLength() const
{
    TRACE("");
    return 0;
}


/*
 * see NATIVE_WINDOW_CONCRETE_TYPE
 */
unsigned int FbDevNativeWindow::type() const
{
    TRACE("");
    return NATIVE_WINDOW_FRAMEBUFFER;
}


/*
 * see NATIVE_WINDOW_TRANSFORM_HINT
 */
unsigned int FbDevNativeWindow::transformHint() const
{
    TRACE("");
    return 0;
}

/*
 * returns the current usage of this window
 */
unsigned int FbDevNativeWindow::getUsage() const
{
    return m_usage;
}

/*
 *  native_window_set_usage(..., usage)
 *  Sets the intended usage flags for the next buffers
 *  acquired with (*lockBuffer)() and on.
 *  By default (if this function is never called), a usage of
 *      GRALLOC_USAGE_HW_RENDER | GRALLOC_USAGE_HW_TEXTURE
 *  is assumed.
 *  Calling this function will usually cause following buffers to be
 *  reallocated.
 */
int FbDevNativeWindow::setUsage(int usage)
{
    m_allocateBuffers = (m_usage != usage);
    TRACE("usage=x%x m_allocateBuffers=%d", usage, m_allocateBuffers);
    m_usage = usage;
    return NO_ERROR;
}


/*
 * native_window_set_buffers_format(..., int format)
 * All buffers dequeued after this call will have the format specified.
 *
 * If the specified format is 0, the default buffer format will be used.
 */
int FbDevNativeWindow::setBuffersFormat(int format)
{
    m_allocateBuffers |= (format != m_bufFormat);
    TRACE("format=x%x m_allocateBuffers=%d", format, m_allocateBuffers);
    m_bufFormat = format;
    return NO_ERROR;
}


/*
 * native_window_set_buffer_count(..., count)
 * Sets the number of buffers associated with this native window.
 */
int FbDevNativeWindow::setBufferCount(int cnt)
{
    TRACE("cnt=%d", cnt);
    if (m_bufferCount != cnt) {
        m_bufferCount = cnt;
        m_allocateBuffers = true;
    }
    return NO_ERROR;
}

void FbDevNativeWindow::reallocateBuffers()
{
    destroyBuffers();

    for(int i = 0; i < m_bufferCount; i++)
    {
        FbDevNativeWindowBuffer *fbnb = new FbDevNativeWindowBuffer(hybris_gralloc_fbdev_width(),
                            hybris_gralloc_fbdev_height(), hybris_gralloc_fbdev_format(), m_usage|GRALLOC_USAGE_HW_FB);

        fbnb->common.incRef(&fbnb->common);

        TRACE("buffer %i is at %p (native %p) err=%s handle=%p stride=%i",
                i, fbnb, (ANativeWindowBuffer*)fbnb,
                strerror(-fbnb->status), fbnb->handle, fbnb->stride);

        if (fbnb->status)
        {
            fbnb->common.decRef(&fbnb->common);
            fprintf(stderr,"WARNING: %s: allocated only %d buffers out of %d\n", __PRETTY_FUNCTION__, m_freeBufs, m_bufferCount);
            break;
        }

        m_freeBufs++;
        m_bufList.push_back(fbnb);
    }

    m_allocateBuffers = false;
}

/*
 * native_window_set_buffers_dimensions(..., int w, int h)
 * All buffers dequeued after this call will have the dimensions specified.
 * In particular, all buffers will have a fixed-size, independent from the
 * native-window size. They will be scaled according to the scaling mode
 * (see native_window_set_scaling_mode) upon window composition.
 *
 * If w and h are 0, the normal behavior is restored. That is, dequeued buffers
 * following this call will be sized to match the window's size.
 *
 * Calling this function will reset the window crop to a NULL value, which
 * disables cropping of the buffers.
 */
int FbDevNativeWindow::setBuffersDimensions(int width, int height)
{
    TRACE("WARN: stub. size=%ix%i", width, height);
    return NO_ERROR;
}
// vim: noai:ts=4:sw=4:ss=4:expandtab
