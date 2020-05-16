pkglib_LTLIBRARIES = eglplatform_wayland.la

eglplatform_wayland_la_SOURCES = eglplatform_wayland.cpp wayland_window.cpp
eglplatform_wayland_la_CXXFLAGS = \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/egl \
	-I$(top_srcdir)/egl/platforms/common \
	-I$(top_builddir)/egl/platforms/common \
	$(ANDROID_HEADERS_CFLAGS) \
	$(WAYLAND_CLIENT_CFLAGS)

if WANT_DEBUG
eglplatform_wayland_la_CXXFLAGS += -I$(top_srcdir)/common
endif

if WANT_TRACE
eglplatform_wayland_la_CXXFLAGS += -DDEBUG
endif

if WANT_DEBUG
eglplatform_wayland_la_CXXFLAGS += -ggdb -O0
endif

if !WANT_WL_SERVERSIDE_BUFFERS
eglplatform_wayland_la_CXXFLAGS += -DHYBRIS_NO_SERVER_SIDE_BUFFERS
endif



eglplatform_wayland_la_LDFLAGS = \
	-avoid-version -module -shared -export-dynamic \
	$(top_builddir)/egl/platforms/common/libhybris-eglplatformcommon.la \
	$(top_builddir)/gralloc/libgralloc.la \
	$(WAYLAND_CLIENT_LIBS)
if HAS_ANDROID_4_2_0
eglplatform_wayland_la_LDFLAGS += $(top_builddir)/libsync/libsync.la
endif

if HAS_ANDROID_5_0_0
eglplatform_wayland_la_LDFLAGS += $(top_builddir)/libsync/libsync.la
endif

