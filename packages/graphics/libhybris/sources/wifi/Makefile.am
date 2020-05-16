lib_LTLIBRARIES = \
	libwifi.la

libwifi_la_SOURCES = wifi.c
libwifi_la_CFLAGS = -I$(top_srcdir)/include
if WANT_TRACE
libwifi_la_CFLAGS += -DDEBUG
endif
if WANT_DEBUG
libwifi_la_CFLAGS += -ggdb -O0
endif
libwifi_la_LDFLAGS = \
	$(top_builddir)/common/libhybris-common.la \
	-version-info "1":"0":"0"

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libwifi.pc
