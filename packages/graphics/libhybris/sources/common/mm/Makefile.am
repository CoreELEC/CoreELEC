bionic = $(top_srcdir)/common/mm/bionic
bionic_libc = $(bionic)/libc

modlibexecdir = $(libdir)/libhybris/linker
modlibexec_LTLIBRARIES = mm.la

ARCH_FLAGS =

AM_CFLAGS = \
	-std=gnu99

AM_CPPFLAGS = \
	-fno-stack-protector \
	-Wstrict-overflow=5

AM_CXXFLAGS = \
	-std=gnu++11 \
	-Wold-style-cast

mm_la_SOURCES = \
	hybris_compat.cpp \
	dlfcn.cpp \
	linker_allocator.cpp \
	linker_block_allocator.cpp \
	linker.cpp \
	linker_memory.cpp \
	linker_phdr.cpp \
	linker_sdk_versions.cpp \
	rt.cpp \
	../strlcpy.c \
	../strlcat.c
mm_la_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	-I$(bionic_libc)/ \
	-I$(bionic_libc)/include \
	-DDEFAULT_HYBRIS_LD_LIBRARY_PATH="\"@DEFAULT_HYBRIS_LD_LIBRARY_PATH@\"" \
	$(ARCH_FLAGS) \
	$(ANDROID_HEADERS_CFLAGS)
mm_la_LDFLAGS = \
	-lsupc++ \
	-module \
	-avoid-version

if WANT_DEBUG
mm_la_CPPFLAGS += \
	-DTRACE_DEBUG=1
else
mm_la_CPPFLAGS += \
	-DTRACE_DEBUG=1
endif

if WANT_ARM_TRACING
mm_la_CPPFLAGS += \
	-DWANT_ARM_TRACING
endif

