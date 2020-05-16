lib_LTLIBRARIES = \
	libnfc_ndef_nxp.la

libnfc_ndef_nxp_la_SOURCES = libnfc_ndef_nxp.c
libnfc_ndef_nxp_la_CFLAGS = \
	-I$(top_srcdir)/include \
	$(ANDROID_HEADERS_CFLAGS) \
	-Wno-error=unused-function

if HAS_LIBNFC_NXP_HEADERS
libnfc_ndef_nxp_la_CFLAGS += \
        -DHAS_LIBNFC_NXP_HEADERS=1
else
libnfc_ndef_nxp_la_CFLAGS += \
        -DHAS_LIBNFC_NXP_HEADERS=0
endif

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libnfc_ndef_nxp.pc

if WANT_TRACE
libnfc_ndef_nxp_la_CFLAGS += -DDEBUG
endif

if WANT_DEBUG
libnfc_ndef_nxp_la_CFLAGS += -ggdb -O0
endif

libnfc_ndef_nxp_la_LDFLAGS = \
	$(top_builddir)/common/libhybris-common.la \
	-version-info "1":"0":"0"
