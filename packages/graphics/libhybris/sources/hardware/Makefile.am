lib_LTLIBRARIES = \
	libhardware.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libhardware.pc

libhardware_la_SOURCES = hardware.c
libhardware_la_CFLAGS = -I$(top_srcdir)/include $(ANDROID_HEADERS_CFLAGS)
if WANT_TRACE
libhardware_la_CFLAGS += -DDEBUG
endif
if WANT_DEBUG
libhardware_la_CFLAGS += -ggdb -O0
endif
libhardware_la_LDFLAGS = \
	$(top_builddir)/common/libhybris-common.la \
	-version-info "2":"0":"0"
