lib_LTLIBRARIES = libhybris-hwcomposerwindow.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = hwcomposer-egl.pc

hwcomposerwindowincludedir = $(includedir)/hybris/hwcomposerwindow
hwcomposerwindowinclude_HEADERS = \
        hwcomposer_window.h \
        hwcomposer.h

libhybris_hwcomposerwindow_la_SOURCES = \
	hwcomposer_window.cpp

libhybris_hwcomposerwindow_la_CXXFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/egl \
	-I$(top_srcdir)/egl/platforms/common \
	$(ANDROID_HEADERS_CFLAGS)

if WANT_TRACE
libhybris_hwcomposerwindow_la_CXXFLAGS += -DDEBUG
endif
if WANT_DEBUG
libhybris_hwcomposerwindow_la_CXXFLAGS += -ggdb -O0
endif

libhybris_hwcomposerwindow_la_LDFLAGS = \
	-version-info "1":"0":"0" \
	$(top_builddir)/egl/platforms/common/libhybris-eglplatformcommon.la \
	$(top_builddir)/gralloc/libgralloc.la

if HAS_ANDROID_4_2_0
libhybris_hwcomposerwindow_la_LDFLAGS += $(top_builddir)/libsync/libsync.la
endif
if HAS_ANDROID_5_0_0
libhybris_hwcomposerwindow_la_LDFLAGS += $(top_builddir)/libsync/libsync.la
endif
pkglib_LTLIBRARIES = eglplatform_hwcomposer.la

eglplatform_hwcomposer_la_SOURCES = \
	eglplatform_hwcomposer.cpp

eglplatform_hwcomposer_la_CXXFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/egl \
	-I$(top_srcdir)/egl/platforms/common \
	$(ANDROID_HEADERS_CFLAGS)

if WANT_TRACE
eglplatform_hwcomposer_la_CXXFLAGS += -DDEBUG
endif
if WANT_DEBUG
eglplatform_hwcomposer_la_CXXFLAGS += -ggdb -O0
endif
eglplatform_hwcomposer_la_LIBADD = libhybris-hwcomposerwindow.la
eglplatform_hwcomposer_la_LDFLAGS = \
	-avoid-version -module -shared -export-dynamic \
	$(top_builddir)/egl/platforms/common/libhybris-eglplatformcommon.la \
	$(top_builddir)/gralloc/libgralloc.la

