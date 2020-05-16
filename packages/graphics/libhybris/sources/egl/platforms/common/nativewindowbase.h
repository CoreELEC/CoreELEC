#ifndef NATIVEWINDOWBASE_H
#define NATIVEWINDOWBASE_H

/* for ICS window.h */
#include <string.h>
#include <system/window.h>
#include <EGL/egl.h>
#include "support.h"
#include <stdarg.h>
#include <assert.h>

#ifdef DEBUG
#include <stdio.h>
#endif

#define NO_ERROR                0L
#define BAD_VALUE               -1

/**
 * @brief A Class to do common ANativeBuffer initialization and thunk c-style
 *        callbacks into C++ method calls.
 **/
class BaseNativeWindowBuffer : public ANativeWindowBuffer
{
protected:
	BaseNativeWindowBuffer();
	virtual ~BaseNativeWindowBuffer() ;

public:
	/* When passing the buffer to EGL functions it's required to pass always the native
	 * buffer. This method takes care about proper casting. */
	ANativeWindowBuffer* getNativeBuffer() const;

private:
	unsigned int refcount;
	static void _decRef(struct android_native_base_t* base);
	static void _incRef(struct android_native_base_t* base);
};

/**
 * @brief A Class to do common ANativeWindow initialization and thunk c-style
 *        callbacks into C++ method calls.
 **/
class BaseNativeWindow : public ANativeWindow
{
public:
	operator EGLNativeWindowType()
	{
		EGLNativeWindowType ret = reinterpret_cast<EGLNativeWindowType>(static_cast<ANativeWindow *>(this));
		return ret;
	}

protected:
	BaseNativeWindow();
	virtual ~BaseNativeWindow();

	// does this require more magic?
	unsigned int refcount;
	static void _decRef(struct android_native_base_t* base);
	static void _incRef(struct android_native_base_t* base);

	// these have to be implemented in the concrete implementation, eg. FBDEV or offscreen window
	virtual int setSwapInterval(int interval) = 0;

	virtual int dequeueBuffer(BaseNativeWindowBuffer **buffer, int *fenceFd) = 0;
	virtual int queueBuffer(BaseNativeWindowBuffer *buffer, int fenceFd) = 0;
	virtual int cancelBuffer(BaseNativeWindowBuffer *buffer, int fenceFd) = 0;
	virtual int lockBuffer(BaseNativeWindowBuffer *buffer) = 0; // DEPRECATED

	virtual unsigned int type() const = 0;
	virtual unsigned int width() const = 0;
	virtual unsigned int height() const = 0;
	virtual unsigned int format() const = 0;
	virtual unsigned int defaultWidth() const = 0;
	virtual unsigned int defaultHeight() const = 0;
	virtual unsigned int queueLength() const = 0;
	virtual unsigned int transformHint() const = 0;
	virtual unsigned int getUsage() const = 0;
	//perform interfaces
	virtual int setBuffersFormat(int format) = 0;
	virtual int setBuffersDimensions(int width, int height) = 0;
	virtual int setUsage(int usage) = 0;
	virtual int setBufferCount(int cnt) = 0;
private:
	static int _setSwapInterval(struct ANativeWindow* window, int interval);
	static int _dequeueBuffer_DEPRECATED(ANativeWindow* window, ANativeWindowBuffer** buffer);
	static const char *_native_window_operation(int what);
	static const char *_native_query_operation(int what);
	static int _lockBuffer_DEPRECATED(struct ANativeWindow* window, ANativeWindowBuffer* buffer);
	static int _queueBuffer_DEPRECATED(struct ANativeWindow* window, ANativeWindowBuffer* buffer);
	static int _query(const struct ANativeWindow* window, int what, int* value);
	static int _perform(struct ANativeWindow* window, int operation, ... );
	static int _cancelBuffer_DEPRECATED(struct ANativeWindow* window, ANativeWindowBuffer* buffer);
	static int _queueBuffer(struct ANativeWindow *window, ANativeWindowBuffer *buffer, int fenceFd);
	static int _dequeueBuffer(struct ANativeWindow *window, ANativeWindowBuffer **buffer, int *fenceFd);
	static int _cancelBuffer(struct ANativeWindow *window, ANativeWindowBuffer *buffer, int fenceFd);
};

#endif
