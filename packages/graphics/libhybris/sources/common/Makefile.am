lib_LTLIBRARIES = \
	libhybris-common.la

SUBDIRS =

if !WANT_ARCH_ARM64
SUBDIRS += jb
endif

if HAS_ANDROID_6_0_0
SUBDIRS += mm
endif

if HAS_ANDROID_7_0_0
SUBDIRS += n
endif

if HAS_ANDROID_8_0_0
SUBDIRS += o
endif

libhybris_common_la_SOURCES = \
	hooks.c \
	hooks_shm.c \
	strlcpy.c \
	strlcat.c \
	logging.c \
	sysconf.c \
	legacy_properties/properties.c \
	legacy_properties/cache.c

if WANT_RUNTIME_PROPERTY_CACHE
libhybris_common_la_SOURCES += \
        legacy_properties/runtime_cache.c
libhybris_common_la_CFLAGS =
else
libhybris_common_la_CFLAGS = -DNO_RUNTIME_PROPERTY_CACHE
endif
if WANT_ARM_TRACING
libhybris_common_la_SOURCES += \
	wrappers.c \
	wrapper_code_generic_arm.c
endif

libhybris_common_la_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(ANDROID_HEADERS_CFLAGS) \
	-I$(top_srcdir)/common \
	-DLINKER_PLUGIN_DIR=\"$(libdir)/libhybris/linker\"

if WANT_ADRENO_QUIRKS
libhybris_common_la_CPPFLAGS += -DWANT_ADRENO_QUIRKS
endif

if WANT_ARM_TRACING
# thumb mode not supported
libhybris_common_la_CFLAGS += \
	-marm
libhybris_common_la_CPPFLAGS += \
	-marm
endif

if WANT_TRACE
libhybris_common_la_CPPFLAGS += -DDEBUG
endif
if WANT_DEBUG
libhybris_common_la_CPPFLAGS += -ggdb -O0
endif
if WANT_MALI_QUIRKS
libhybris_common_la_CPPFLAGS += -DMALI_QUIRKS
endif
if WANT_UBUNTU_LINKER_OVERRIDES
libhybris_common_la_CPPFLAGS += -DUBUNTU_LINKER_OVERRIDES
endif
if !WANT_ARCH_ARM64
libhybris_common_la_CPPFLAGS += -DWANT_LINKER_JB
endif
if HAS_ANDROID_6_0_0
libhybris_common_la_CPPFLAGS += -DWANT_LINKER_MM
endif
if WANT_ARM_TRACING
libhybris_common_la_CPPFLAGS += -DWANT_ARM_TRACING
endif
if HAS_ANDROID_7_0_0
libhybris_common_la_CPPFLAGS += -DWANT_LINKER_N
endif
if HAS_ANDROID_8_0_0
libhybris_common_la_CPPFLAGS += -DWANT_LINKER_O
endif
libhybris_common_la_LDFLAGS = \
	-ldl \
	-lrt \
	-pthread \
	-lc \
	-lstdc++ \
	-version-info "$(LT_CURRENT)":"$(LT_REVISION)":"$(LT_AGE)"
