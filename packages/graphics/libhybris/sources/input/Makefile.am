lib_LTLIBRARIES = \
	libis.la

libis_la_SOURCES = is.c
libis_la_CFLAGS = -I$(top_srcdir)/include $(ANDROID_HEADERS_CFLAGS)
if WANT_TRACE
libis_la_CFLAGS += -DDEBUG
endif
if WANT_DEBUG
libis_la_CFLAGS += -ggdb -O0
endif
libis_la_LDFLAGS = \
	$(top_builddir)/common/libhybris-common.la \
	-version-info "1":"0":"0"

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libis.pc
