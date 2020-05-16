pkglib_LTLIBRARIES = eglplatform_fbdev.la

eglplatform_fbdev_la_SOURCES = \
	eglplatform_fbdev.cpp \
	fbdev_window.cpp

eglplatform_fbdev_la_CXXFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/egl \
	-I$(top_srcdir)/egl/platforms/common \
	$(ANDROID_HEADERS_CFLAGS)

if WANT_TRACE
eglplatform_fbdev_la_CXXFLAGS += -DDEBUG
endif
if WANT_DEBUG
eglplatform_fbdev_la_CXXFLAGS += -ggdb -O0
endif

eglplatform_fbdev_la_LDFLAGS = \
	-avoid-version -module -shared -export-dynamic \
	$(top_builddir)/egl/platforms/common/libhybris-eglplatformcommon.la \
	$(top_builddir)/gralloc/libgralloc.la
