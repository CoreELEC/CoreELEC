lib_LTLIBRARIES = \
	libmedia.la

libmedia_la_SOURCES = media.c
libmedia_la_CFLAGS = -I$(top_srcdir)/include
if WANT_TRACE
libmedia_la_CFLAGS += -DDEBUG
endif
if WANT_DEBUG
libmedia_la_CFLAGS += -ggdb -O0
endif
libmedia_la_LDFLAGS = \
	$(top_builddir)/common/libhybris-common.la \
	-version-info "1":"0":"0"

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libmedia.pc
