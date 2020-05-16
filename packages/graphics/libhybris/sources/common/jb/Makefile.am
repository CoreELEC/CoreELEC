if WANT_ARCH_ARM
ARCHFLAGS = -DHAVE_ARM_TLS_REGISTER -DANDROID_ARM_LINKER
endif

if  WANT_ARCH_X86
ARCHFLAGS = -DANDROID_X86_LINKER
endif

modlibexecdir = $(libdir)/libhybris/linker
modlibexec_LTLIBRARIES = jb.la
jb_la_SOURCES = \
	dlfcn.c \
	linker.c \
	linker_environ.c \
	linker_format.c \
	rt.c \
	../strlcpy.c
jb_la_CFLAGS = \
	-I$(top_srcdir)/include \
	$(ANDROID_HEADERS_CFLAGS) \
	-I$(top_srcdir)/common \
	-DDEFAULT_HYBRIS_LD_LIBRARY_PATH="\"@DEFAULT_HYBRIS_LD_LIBRARY_PATH@\"" \
	$(ARCHFLAGS)
jb_la_LDFLAGS = \
	-module \
	-avoid-version

if WANT_DEBUG
jb_la_CFLAGS += -DLINKER_DEBUG=1
else
jb_la_CFLAGS += -DLINKER_DEBUG=0
endif

if WANT_ARM_TRACING
jb_la_CPPFLAGS = \
	-DWANT_ARM_TRACING
endif

