lib_LTLIBRARIES = \
	libsf.la

libsf_la_SOURCES = sf.c
libsf_la_CFLAGS = -I$(top_srcdir)/include $(ANDROID_HEADERS_CFLAGS)
if WANT_TRACE
libsf_la_CFLAGS += -DDEBUG
endif
if WANT_DEBUG
libsf_la_CFLAGS += -ggdb -O0
endif
libsf_la_LDFLAGS = \
	$(top_builddir)/common/libhybris-common.la \
	-version-info "1":"0":"0"

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libsf.pc
