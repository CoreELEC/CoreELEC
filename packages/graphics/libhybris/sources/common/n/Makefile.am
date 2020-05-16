bionic = $(top_srcdir)/common/n/bionic
bionic_libc = $(bionic)/libc

modlibexecdir = $(libdir)/libhybris/linker
modlibexec_LTLIBRARIES = n.la

ARCH_FLAGS =

AM_CFLAGS = \
	-std=gnu99

AM_CPPFLAGS = \
	-fno-stack-protector \
	-Wstrict-overflow=5

AM_CXXFLAGS = \
	-std=gnu++11 \
	-Wold-style-cast

n_la_SOURCES = \
	hybris_compat.cpp \
	dlfcn.cpp \
	linker_allocator.cpp \
	linker_block_allocator.cpp \
	linker.cpp \
	linker_dlwarning.cpp \
	linker_gdb_support.cpp \
	linker_mapped_file_fragment.cpp \
	linker_memory.cpp \
	linker_phdr.cpp \
	linker_sdk_versions.cpp \
	linker_utils.cpp \
	rt.cpp \
	../strlcpy.c \
	../strlcat.c
n_la_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	-I$(bionic_libc)/ \
	-I$(bionic_libc)/include \
	-DDEFAULT_HYBRIS_LD_LIBRARY_PATH="\"@DEFAULT_HYBRIS_LD_LIBRARY_PATH@\"" \
	$(ARCH_FLAGS) \
	$(ANDROID_HEADERS_CFLAGS)
n_la_LDFLAGS = \
	-lsupc++ \
	-module \
	-avoid-version

if WANT_DEBUG
n_la_CPPFLAGS += \
	-DTRACE_DEBUG=1
else
n_la_CPPFLAGS += \
	-DTRACE_DEBUG=1
endif

if WANT_ARM_TRACING
n_la_CPPFLAGS += \
	-DWANT_ARM_TRACING
endif

