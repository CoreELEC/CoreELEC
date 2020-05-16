lib_LTLIBRARIES = \
	libGLESv2.la

libGLESv2_la_SOURCES = glesv2.c
libGLESv2_la_CFLAGS = -I$(top_srcdir) -I$(top_srcdir)/include $(ANDROID_HEADERS_CFLAGS)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = glesv2.pc

if WANT_MESA
libGLESv2_la_CFLAGS += -DLIBHYBRIS_WANTS_MESA_X11_HEADERS
endif
if WANT_TRACE
libGLESv2_la_CFLAGS += -DDEBUG
endif
if WANT_DEBUG
libGLESv2_la_CFLAGS += -ggdb -O0
endif
libGLESv2_la_LDFLAGS = \
	$(top_builddir)/common/libhybris-common.la \
	-version-info "2":"0":"0"
