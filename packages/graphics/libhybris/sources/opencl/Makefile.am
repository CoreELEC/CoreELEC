lib_LTLIBRARIES = \
	libOpenCL.la

libOpenCL_la_SOURCES = \
	opencl.c

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = OpenCL.pc

libOpenCL_la_CFLAGS = -I$(top_srcdir)/include $(ANDROID_HEADERS_CFLAGS) -DPKGLIBDIR="\"$(pkglibdir)/\"" -I$(top_srcdir)/common
if WANT_MESA
libOpenCL_la_CFLAGS += -DLIBHYBRIS_WANTS_MESA_X11_HEADERS
endif
if WANT_TRACE
libOpenCL_la_CFLAGS += -DDEBUG
endif
if WANT_DEBUG
libOpenCL_la_CFLAGS += -ggdb -O0
endif

libOpenCL_la_CXXFLAGS = -I$(top_srcdir)/include $(ANDROID_HEADERS_CFLAGS) -I$(top_srcdir)/common -DPKGLIBDIR="\"$(pkglibdir)/\""
if WANT_MESA
libOpenCL_la_CXXFLAGS += -DLIBHYBRIS_WANTS_MESA_X11_HEADERS
endif
if WANT_TRACE
libOpenCL_la_CXXFLAGS += -DDEBUG
endif
if WANT_DEBUG
libOpenCL_la_CXXFLAGS += -ggdb -O0
endif
libOpenCL_la_LDFLAGS = \
	-ldl \
	$(top_builddir)/common/libhybris-common.la \
	-version-info "1":"0":"0"

