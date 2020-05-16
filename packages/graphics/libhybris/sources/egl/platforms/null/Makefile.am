pkglib_LTLIBRARIES = eglplatform_null.la


eglplatform_null_la_SOURCES = eglplatform_null.c
eglplatform_null_la_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/egl \
	-I$(top_srcdir)/egl/platforms/common \
	$(ANDROID_HEADERS_CFLAGS) \
	$(WAYLAND_SERVER_CFLAGS)

eglplatform_null_la_LDFLAGS = \
	-avoid-version -module -shared -export-dynamic \
	$(top_builddir)/egl/platforms/common/libhybris-eglplatformcommon.la \
	$(top_builddir)/gralloc/libgralloc.la \
	$(WAYLAND_SERVER_LIBS)

if WANT_DEBUG
eglplatform_null_la_CFLAGS += -I$(top_srcdir)/common -ggdb -O0
endif

if WANT_TRACE
eglplatform_null_la_CFLAGS += -DDEBUG
endif
