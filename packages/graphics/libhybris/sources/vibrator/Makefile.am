lib_LTLIBRARIES = \
	libvibrator.la

libvibrator_la_SOURCES = vibrator.c
libvibrator_la_CFLAGS = -I$(top_srcdir)/include $(ANDROID_HEADERS_CFLAGS)
if WANT_TRACE
libvibrator_la_CFLAGS += -DDEBUG
endif
if WANT_DEBUG
libvibrator_la_CFLAGS += -ggdb -O0
endif
libvibrator_la_LDFLAGS = \
	$(top_builddir)/common/libhybris-common.la \
	-version-info "1":"0":"0"

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libvibrator.pc
