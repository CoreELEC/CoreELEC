lib_LTLIBRARIES = \
	libandroid-properties.la

libandroid_properties_la_SOURCES = hybris_properties.c

if WANT_DEBUG
libandroid_properties_la_CFLAGS = -ggdb -O0
else
libandroid_properties_la_CFLAGS =
endif

libandroid_properties_la_CFLAGS += -I$(top_srcdir)/include

libandroid_properties_la_LDFLAGS = \
	$(top_builddir)/common/libhybris-common.la \
	-version-info "1":"0":"0"

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libandroid-properties.pc

