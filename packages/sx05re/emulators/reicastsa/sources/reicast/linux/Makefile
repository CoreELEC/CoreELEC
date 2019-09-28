.DEFAULT_GOAL := all
LOCAL_PATH := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
#NO_REC := 1
#NO_REND := 1
WEBUI :=1
USE_OSS := 1
#USE_PULSEAUDIO := 1
#USE_SDLAUDIO := 1
#USE_LIBAO := 1
USE_EVDEV := 1
USE_MODEM := 1
PLATFORM_EXT := elf
SCRIPTING := 1
USE_X11 := 0

#CXX=${CC_PREFIX}g++
#CC=${CC_PREFIX}gcc
#AS=${CC_PREFIX}gcc
#STRIP=${CC_PREFIX}strip
#LD=${CC}
WINDRES=${CC_PREFIX}windres
CHD5_LZMA := 1
CHD5_FLAC := 1

MFLAGS :=
ASFLAGS :=
LDFLAGS := -g -Wl,-Map,$(notdir $@).map,--gc-sections -Wl,-O3 -Wl,--sort-common -fopenmp #defaults
INCS :=
LIBS := -lm -lpthread #defaults

CFLAGS := -g -O3 -D RELEASE -c -D USES_HOMEDIR -fopenmp # defaults
CFLAGS += -frename-registers -fno-strict-aliasing
CFLAGS += -ffast-math -ftree-vectorize

CXXFLAGS :=  -fno-rtti -fpermissive -fno-operator-names -D_GLIBCXX_USE_CXX11_ABI=0 -std=c++14 # defaults



# make install configuration
####################################
PREFIX ?= /usr/local
MAN_DIR ?= ${PREFIX}/share/man/man1
MENUENTRY_DIR ?= ${PREFIX}/share/applications
ICON_DIR ?= ${PREFIX}/share/pixmaps



# hacky BSD support
####################################
ifneq (, $(filter $(shell uname -s), FreeBSD OpenBSD NetBSD DragonFly))
    CFLAGS += -DTARGET_BSD
else
    USE_UDEV := 1
    USE_ALSA := 1
#    USE_JOYSTICK := 1
endif



# Platform auto-detection
# Can be overridden by using:
#   make platform=x64
####################################
include detect-platform.make

$(info Platform: $(platform))

# Expand certain features via plaftorm string
ifneq (,$(findstring noexceptions,$(platform)))
    CFLAGS += -DTARGET_NO_EXCEPTIONS
endif
ifneq (,$(findstring norwxpages,$(platform)))
    CFLAGS += -DFEAT_NO_RWX_PAGES
endif



# per platform configuration
####################################
include configure-platform.make



# core.mk / module source lists
####################################

RZDCY_SRC_DIR = $(LOCAL_PATH)/../../libswirl
include $(RZDCY_SRC_DIR)/core.mk


CFLAGS += $(RZDCY_CFLAGS)

CXXFLAGS += $(RZDCY_CFLAGS)

INCS += -I$(RZDCY_SRC_DIR) -I$(RZDCY_SRC_DIR)/deps



# libs configuration
####################################
ifdef PLATFORM_LINUX
    LIBS += -lrt -ldl # dl is the dynamic linker
endif

ifdef USE_SDL
    CXXFLAGS += `sdl2-config --cflags` -D USE_SDL
    LIBS += `sdl2-config --libs`
endif

ifdef USE_X11
    CFLAGS += `pkg-config --cflags x11` -D SUPPORT_X11
    CXXFLAGS += `pkg-config --cflags x11` -D SUPPORT_X11
    LIBS += `pkg-config --libs x11`
endif


ifdef USE_OMX
    CXXFLAGS += -D USE_OMX
    LIBS += -lopenmaxil
endif

ifdef USE_ALSA
    CXXFLAGS += `pkg-config --cflags alsa` -D USE_ALSA
    LIBS += `pkg-config --libs alsa`
endif

ifdef USE_PULSEAUDIO
    CXXFLAGS += `pkg-config --cflags libpulse-simple` -D USE_PULSEAUDIO
    LIBS += `pkg-config --libs libpulse-simple`
endif

ifdef USE_SDLAUDIO
    CXXFLAGS += `sdl2-config --cflags` -D USE_SDL_AUDIO
    LIBS += `sdl2-config --libs`
endif

ifdef USE_OSS
    CXXFLAGS += -D USE_OSS
endif

ifdef USE_LIBAO
    CXXFLAGS += `pkg-config --cflags ao` -D USE_LIBAO
    LIBS += `pkg-config --libs ao`
endif

ifdef USE_EVDEV
    CXXFLAGS += -D USE_EVDEV
    ifdef USE_UDEV
        CXXFLAGS += -D USE_UDEV
        LIBS += -ludev
    endif
endif

ifdef USE_JOYSTICK
    CXXFLAGS += -D USE_JOYSTICK
endif

ifdef USE_DISPMANX
    CFLAGS += -D SUPPORT_DISPMANX
    CXXFLAGS += -D SUPPORT_DISPMANX
endif

ifdef SUPPORT_EGL
    LIBS += -lEGL
endif

ifdef HAS_SOFTREND
    CFLAGS += -msse4.1
endif

ifdef SCRIPTING
    CXXFLAGS += -D SCRIPTING
endif

#softrend
# TODO: Move to a better place
ifdef HAS_SOFTREND
    CXXFLAGS += -DTARGET_SOFTREND
endif


# PGO options
ifdef PGO_MAKE
    CFLAGS += -fprofile-generate -pg
    LDFLAGS += -fprofile-generate
else
    CFLAGS += -fomit-frame-pointer
endif

ifdef PGO_USE
    CFLAGS += -fprofile-use
endif

# LTO options
ifdef LTO_TEST
    CFLAGS += -flto -fwhole-program
    LDFLAGS +=-flto -fwhole-program
endif

# executable name configuration
EXECUTABLE_STRIPPED=nosym-reicast.$(PLATFORM_EXT)
ifdef NAOMI
    CFLAGS += -D TARGET_NAOMI
    DC_PLATFORM=naomi
    EXECUTABLE=reicast_naomi.$(PLATFORM_EXT)
    EXECUTABLE_NAME=reicast-naomi
else ifdef ATOMISWAVE
    CFLAGS += -D DC_PLATFORM=DC_PLATFORM_ATOMISWAVE
    DC_PLATFORM=atomiswave
    EXECUTABLE=reicast_awave.$(PLATFORM_EXT)
    EXECUTABLE_NAME=reicast-awave
else ifdef DISPFRAME
    CFLAGS += -D TARGET_DISPFRAME #-D TARGET_NAOMI
    DC_PLATFORM=dispframe
    EXECUTABLE=dispframe.$(PLATFORM_EXT)
    EXECUTABLE_NAME=dispframe
else
    DC_PLATFORM=dreamcast
    EXECUTABLE=reicast.$(PLATFORM_EXT)
    EXECUTABLE_NAME=reicast
endif

# Assembler configuration
ASFLAGS += $(CFLAGS)

# builddir configuration
CHAR_EMPTY:=
CHAR_SPACE:=$(CHAR_EMPTY) $(CHAR_EMPTY)
BUILDDIR:=obj-$(DC_PLATFORM)-$(subst $(CHAR_SPACE),-,$(platform))

# collect objects
OBJECTS=$(RZDCY_FILES:.cpp=.build_obj)
OBJECTS:=$(OBJECTS:.cc=.build_obj)
OBJECTS:=$(OBJECTS:.c=.build_obj)
OBJECTS:=$(OBJECTS:.S=.build_obj)
OBJECTS:=$(patsubst $(RZDCY_SRC_DIR)/%,$(BUILDDIR)/%,$(OBJECTS))

# add windows resource file
# TODO: Move to a better place
ifdef PLATFORM_WINDOWS
    OBJECTS+=$(BUILDDIR)/reicastres.build_obj
endif

# depfiles
DEPDIR := .dep-$(BUILDDIR)
DEPFLAGS = -MT $@ -MD -MP -MF $(DEPDIR)/$*.Td
DEPS=$(RZDCY_FILES:.cpp=.d)
DEPS:=$(DEPS:.cc=.d)
DEPS:=$(DEPS:.c=.d)
DEPS:=$(DEPS:.S=.d)
DEPS:=$(patsubst $(RZDCY_SRC_DIR)/%,$(DEPDIR)/%,$(DEPS))
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

# build rules

all: $(CPPFILES) $(EXECUTABLE) $(EXECUTABLE_STRIPPED)
ifneq (,$(findstring gcwz,$(platform)))
	mksquashfs $(EXECUTABLE_STRIPPED) $(GCWZ_PKG_FILES) $(GCWZ_PKG) -all-root
endif

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(MFLAGS) $(EXTRAFLAGS) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $@

$(EXECUTABLE_STRIPPED): $(EXECUTABLE)
	cp $< $@ && $(STRIP) $@

$(BUILDDIR)/%.build_obj : $(RZDCY_SRC_DIR)/%.cpp
$(BUILDDIR)/%.build_obj: $(RZDCY_SRC_DIR)/%.cpp $(DEPDIR)/%.d $(VERSION_HEADER)
	mkdir -p $(dir $@)
	mkdir -p .dep-$(dir $@)
	$(CXX) $(EXTRAFLAGS) $(INCS) $(DEPFLAGS) $(CFLAGS) $(MFLAGS) $(CXXFLAGS) $< -o $@
	$(POSTCOMPILE)

$(BUILDDIR)/%.build_obj : $(RZDCY_SRC_DIR)/%.cc
$(BUILDDIR)/%.build_obj: $(RZDCY_SRC_DIR)/%.cc $(DEPDIR)/%.d $(VERSION_HEADER)
	mkdir -p $(dir $@)
	mkdir -p .dep-$(dir $@)
	$(CXX) $(EXTRAFLAGS) $(INCS) $(DEPFLAGS) $(CFLAGS) $(MFLAGS) $(CXXFLAGS) $< -o $@
	$(POSTCOMPILE)

$(BUILDDIR)/%.build_obj : $(RZDCY_SRC_DIR)/%.c
$(BUILDDIR)/%.build_obj: $(RZDCY_SRC_DIR)/%.c $(DEPDIR)/%.d $(VERSION_HEADER)
	mkdir -p $(dir $@)
	mkdir -p .dep-$(dir $@)
	$(CC) $(EXTRAFLAGS) $(INCS) $(DEPFLAGS) $(CFLAGS) $< -o $@
	$(POSTCOMPILE)

$(BUILDDIR)/%.build_obj : $(RZDCY_SRC_DIR)/%.S $(VERSION_HEADER)
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $(INCS) $< -o $@

$(BUILDDIR)/reicastres.build_obj: $(LOCAL_PATH)/../windows/reicast.rc $(LOCAL_PATH)/../windows/reicast.ico $(RZDCY_SRC_DIR)/version.h
	$(WINDRES) $< $@

install: $(EXECUTABLE)
	mkdir -p $(DESTDIR)$(PREFIX)/bin 2>/dev/null || true
	mkdir -p $(DESTDIR)$(PREFIX)/share/reicast/mappings 2>/dev/null || true
	mkdir -p $(DESTDIR)$(MAN_DIR) 2>/dev/null || true
	mkdir -p $(DESTDIR)$(MENUENTRY_DIR) 2>/dev/null || true
	mkdir -p $(DESTDIR)$(ICON_DIR) 2>/dev/null || true
	install -m755 $(EXECUTABLE) $(DESTDIR)$(PREFIX)/bin/$(EXECUTABLE_NAME)
	install -m755 tools/reicast-joyconfig.py $(DESTDIR)$(PREFIX)/bin/reicast-joyconfig
	install -m644 mappings/controller_gcwz.cfg $(DESTDIR)$(PREFIX)/share/reicast/mappings
	install -m644 mappings/controller_generic.cfg $(DESTDIR)$(PREFIX)/share/reicast/mappings
	install -m644 mappings/controller_pandora.cfg $(DESTDIR)$(PREFIX)/share/reicast/mappings
	install -m644 mappings/controller_xboxdrv.cfg $(DESTDIR)$(PREFIX)/share/reicast/mappings
	install -m644 mappings/controller_xpad.cfg $(DESTDIR)$(PREFIX)/share/reicast/mappings
	install -m644 mappings/keyboard.cfg $(DESTDIR)$(PREFIX)/share/reicast/mappings
	install -m644 man/reicast.1 $(DESTDIR)$(MAN_DIR)
	install -m644 man/reicast-joyconfig.1 $(DESTDIR)$(MAN_DIR)
	install -m644 reicast.desktop $(DESTDIR)$(MENUENTRY_DIR)
	install -m644 reicast.png $(DESTDIR)$(ICON_DIR)

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(EXECUTABLE_NAME)
	rm -f $(DESTDIR)$(PREFIX)/bin/reicast-joyconfig
	rm -f $(DESTDIR)$(PREFIX)/share/reicast/mappings/controller_gcwz.cfg
	rm -f $(DESTDIR)$(PREFIX)/share/reicast/mappings/controller_generic.cfg
	rm -f $(DESTDIR)$(PREFIX)/share/reicast/mappings/controller_pandora.cfg
	rm -f $(DESTDIR)$(PREFIX)/share/reicast/mappings/controller_xboxdrv.cfg
	rm -f $(DESTDIR)$(PREFIX)/share/reicast/mappings/controller_xpad.cfg
	rm -f $(DESTDIR)$(PREFIX)/share/reicast/mappings/keyboard.cfg
	rm -f $(DESTDIR)$(MAN_DIR)/reicast.1
	rm -f $(DESTDIR)$(MAN_DIR)/reicast-joyconfig.1
	rm -f $(DESTDIR)$(MENUENTRY_DIR)/reicast.desktop
	rm -f $(DESTDIR)$(ICON_DIR)/reicast.png

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) $(EXECUTABLE_STRIPPED) .map $(VERSION_HEADER)

.PRECIOUS = $(DEPDIR)/%.d
$(DEPDIR)/%.d: ;

-include $(DEPS)
