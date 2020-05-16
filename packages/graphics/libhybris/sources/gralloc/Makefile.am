lib_LTLIBRARIES = \
	libgralloc.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libgralloc.pc

libgralloc_la_SOURCES = gralloc.c

if HAS_GRALLOC1_HEADER
libgralloc_la_SOURCES += GrallocUsageConversion.cpp
endif

if HAS_GRALLOC1_HEADER
libgralloc_la_CFLAGS = -DHAS_GRALLOC1_HEADER=1
else
libgralloc_la_CFLAGS = -DHAS_GRALLOC1_HEADER=0
endif
libgralloc_la_CFLAGS += -I$(top_srcdir)/include $(ANDROID_HEADERS_CFLAGS)
libgralloc_la_CXXFLAGS = -std=gnu++11 -I$(top_srcdir)/include
if WANT_TRACE
libgralloc_la_CFLAGS += -DDEBUG
endif
if WANT_DEBUG
libgralloc_la_CFLAGS += -ggdb -O0
endif
libgralloc_la_LDFLAGS = \
	$(top_builddir)/hardware/libhardware.la \
	-version-info "1":"0":"0"
