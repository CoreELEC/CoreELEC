lib_LTLIBRARIES = \
	libGLESv1_CM.la

libGLESv1_CM_la_SOURCES = glesv1_cm.c
libGLESv1_CM_la_CFLAGS = -I$(top_srcdir)/include $(ANDROID_HEADERS_CFLAGS)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = glesv1_cm.pc

if WANT_MESA
libGLESv1_CM_la_CFLAGS += -DLIBHYBRIS_WANTS_MESA_X11_HEADERS
endif
if WANT_TRACE
libGLESv1_CM_la_CFLAGS += -DDEBUG
endif
if WANT_DEBUG
libGLESv1_CM_la_CFLAGS += -ggdb -O0
endif
libGLESv1_CM_la_LDFLAGS = \
	$(top_builddir)/common/libhybris-common.la \
	-version-info "1":"1":"0"
