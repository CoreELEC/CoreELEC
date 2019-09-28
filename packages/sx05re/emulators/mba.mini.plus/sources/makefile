###########################################################################
#
#   makefile
#
#   Core makefile for building MAME and derivatives
#
#   Copyright (c) Nicola Salmoria and the MAME Team.
#   Visit http://mamedev.org for licensing and usage restrictions.
#
###########################################################################

NATIVE := 0
ALIGNED = 0
MDEBUG = 0
PTR64 = 0
BIGENDIAN = 0

# ------------------------------------------------------------
# Set the BIOS used by NEOGEO
# NEOGEO_BIOS = 1 - Use all NEOGEO BIOS ,
# NEOGEO_BIOS = 0 - Use part of the NEOGEO BIOS. (default)
# ------------------------------------------------------------
NEOGEO_BIOS = 0

# system platform
UNAME = $(shell echo armv)

ifeq ($(platform),)
   platform = unix
   system_platform = unix
   ifeq ($(UNAME),)
	platform = win
	EXE_EXT = .exe
	system_platform = win
   else ifneq ($(findstring MINGW,$(UNAME)),)
	platform = win
	system_platform = win
   else ifneq ($(findstring Darwin,$(UNAME)),)
	platform = osx
	system_platform = osx
   else ifneq ($(findstring win,$(UNAME)),)
	platform = win
   endif
endif

# Autodetect PTR64 and ENDIAN
UNAME = $(shell echo armv)

ifeq ($(firstword $(filter x86_64,$(UNAME))),x86_64)
   PTR64 = 1
endif
ifeq ($(firstword $(filter amd64,$(UNAME))),amd64)
   PTR64 = 1
endif
ifneq (,$(findstring mingw64-w64,$(PATH)))
   PTR64 = 1
endif
ifeq ($(firstword $(filter ppc64,$(UNAME))),ppc64)
   PTR64 = 1
endif
ifeq ($(firstword $(filter aarch64,$(UNAME))),aarch64)
   PTR64 = 1
endif
ifneq (,$(findstring Power,$(UNAME)))
   BIGENDIAN = 1
endif
ifneq (,$(findstring ppc,$(UNAME)))
   BIGENDIAN = 1
endif

# CR/LF setup: use both on win32/os2, CR only on everything else
DEFS = -DCRLF=2 -DDISABLE_MIDI=1
# Default to something reasonable for all platforms
ARFLAGS = -cr
# uncomment next line to build PortMidi as part of MAME/MESS build
BUILD_MIDILIB = 0

#-------------------------------------------------
# compile flags
# CCOMFLAGS are common flags
# CONLYFLAGS are flags only used when compiling for C
# CPPONLYFLAGS are flags only used when compiling for C++
# COBJFLAGS are flags only used when compiling for Objective-C(++)
#-------------------------------------------------

# start with empties for everything
CCOMFLAGS = -DDISABLE_MIDI
CONLYFLAGS =
COBJFLAGS =
CPPONLYFLAGS =

# LDFLAGS are used generally; LDFLAGSEMULATOR are additional flags only used when linking the core emulator
LDFLAGS =
LDFLAGSEMULATOR =

TARGET_NAME := mba_mini
fpic := 
EXE = 
LIBS = -lm
CORE_DIR = .

CCOMFLAGS += -D__LIBRETRO__

# When doing 64bit build, is it optimize for local machine (hence the result might not run on different machines).
OPTFLAG ?= 0

# The default is software rendering.
VRENDER ?= soft
HW_RENDER ?= 0

ifeq ($(VRENDER),opengl)
	CCOMFLAGS += -DHAVE_OPENGL
	HW_RENDER = 1
else ifeq ($(VRENDER),gles)
	CCOMFLAGS += -DHAVE_OPENGLES
	HW_RENDER = 2
endif

# UNIX
ifeq ($(platform), unix)
   TARGETLIB := $(TARGET_NAME)_libretro.so
   TARGETOS = linux
   fpic = -fPIC
   SHARED := -shared -Wl,--version-script=src/osd/retro/link.T
ifeq ($(HW_RENDER), 1)
   LIBS += -lGL
else ifeq ($(HW_RENDER), 2)
   LIBS += -lGLESv2
endif
   LDFLAGS += $(SHARED)
   NATIVELD = g++
   NATIVELDFLAGS = -Wl,--warn-common -lstdc++
   NATIVECC = g++
   NATIVECFLAGS = -std=gnu99
   CC_AS = gcc
   CC = g++
   AR = @ar
   LD = g++
   PLATCFLAGS += -fno-common -fno-merge-constants -fsingle-precision-constant
   CCOMFLAGS += $(PLATCFLAGS) -ffast-math
   LIBS += -lstdc++ -lpthread
   ALIGNED = 1


# Android
else ifeq ($(platform), android)
   TARGETLIB := $(TARGET_NAME)_libretro_android.so
   TARGETOS = linux
   fpic = -fPIC
   SHARED := -shared -Wl,--version-script=src/osd/retro/link.T
   NATIVE = 1
   CC_AS = @arm-linux-androideabi-gcc
   CC = @arm-linux-androideabi-g++
   AR = @arm-linux-androideabi-ar
   LD = @arm-linux-androideabi-g++
   CCOMFLAGS += -mstructure-size-boundary=32 -mthumb-interwork -fsigned-char -finline -fno-common -fno-builtin -fweb -frename-registers -falign-functions=16 -fsingle-precision-constant
   PLATCFLAGS += -march=armv7-a -mfloat-abi=softfp -fstrict-aliasing -fno-merge-constants -DSDLMAME_NO64BITIO -DANDTIME -DRANDPATH
   PLATCFLAGS += -DANDROID
   LDFLAGS += -Wl,--fix-cortex-a8 -llog $(SHARED)
   NATIVELD = g++
   NATIVELDFLAGS = -Wl,--warn-common -lstdc++
   NATIVECC = g++
   NATIVECFLAGS = -std=gnu99
   CCOMFLAGS += $(PLATCFLAGS) -ffast-math
   LIBS += -lstdc++
   EXTRA_RULES = 1
   ARM_ENABLED = 1
   ALIGNED = 1


# OS X
else ifeq ($(platform), osx)
   TARGETLIB := $(TARGET_NAME)_libretro.dylib
   TARGETOS = macosx
   fpic = -fPIC
   LDFLAGSEMULATOR += -stdlib=libc++
   SHARED := -dynamiclib
   CC = c++ -stdlib=libc++
   LD = c++ -stdlib=libc++
   NATIVELD = c++ -stdlib=libc++
   NATIVECC = c++
   LDFLAGS +=  $(SHARED)
   CC_AS = clang
   AR = @ar
ifeq ($(COMMAND_MODE),"legacy")
   ARFLAGS = -crs
endif


# iOS
else ifneq (,$(findstring ios,$(platform)))
   TARGETLIB := $(TARGET_NAME)_libretro_ios.dylib
   TARGETOS = macosx
   EXTRA_RULES = 1
   ARM_ENABLED = 1
   fpic = -fPIC
   SHARED := -dynamiclib
   PTR64 = 0
ifeq ($(IOSSDK),)
   IOSSDK := $(shell xcodebuild -version -sdk iphoneos Path)
endif
   CC = c++ -arch armv7 -isysroot $(IOSSDK)
   CCOMFLAGS += -DSDLMAME_NO64BITIO -DIOS
   CFLAGS += -DIOS
   CXXFLAGS += -DIOS
   NATIVELD = $(CC) -stdlib=libc++
   LDFLAGS +=  $(SHARED)
   LD = $(CC)


# QNX
else ifeq ($(platform), qnx)
   TARGETLIB := $(TARGET_NAME)_libretro_$(platform).so
   TARGETOS = linux
   fpic = -fPIC
   SHARED := -shared -Wl,--version-script=src/osd/retro/link.T
   CC = qcc -Vgcc_ntoarmv7le
   AR = qcc -Vgcc_ntoarmv7le
   CFLAGS += -D__BLACKBERRY_QNX__
   LIBS += -lstdc++ -lpthread


# PS3
else ifeq ($(platform), ps3)
   TARGETLIB := $(TARGET_NAME)_libretro_$(platform).a
   CC = $(CELL_SDK)/host-win32/ppu/bin/ppu-lv2-gcc.exe
   AR = $(CELL_SDK)/host-win32/ppu/bin/ppu-lv2-ar.exe
   CFLAGS += -DBLARGG_BIG_ENDIAN=1 -D__ppc__
   STATIC_LINKING = 1
   BIGENDIAN=1
   LIBS += -lstdc++ -lpthread


# PS3 (SNC)
else ifeq ($(platform), sncps3)
   TARGETLIB := $(TARGET_NAME)_libretro_ps3.a
   CC = $(CELL_SDK)/host-win32/sn/bin/ps3ppusnc.exe
   AR = $(CELL_SDK)/host-win32/sn/bin/ps3snarl.exe
   CFLAGS += -DBLARGG_BIG_ENDIAN=1 -D__ppc__
   STATIC_LINKING = 1
   BIGENDIAN = 1
   LIBS += -lstdc++ -lpthread


# Lightweight PS3 Homebrew SDK
else ifeq ($(platform), psl1ght)
   TARGETLIB := $(TARGET_NAME)_libretro_$(platform).a
   CC = $(PS3DEV)/ppu/bin/ppu-gcc$(EXE_EXT)
   AR = $(PS3DEV)/ppu/bin/ppu-ar$(EXE_EXT)
   CFLAGS += -DBLARGG_BIG_ENDIAN=1 -D__ppc__
   STATIC_LINKING = 1
   BIGENDIAN = 1
   LIBS += -lstdc++ -lpthread


# PSP
else ifeq ($(platform), psp1)
   TARGETLIB := $(TARGET_NAME)_libretro_$(platform).a
   CC = psp-g++$(EXE_EXT)
   AR = psp-ar$(EXE_EXT)
   CFLAGS += -DPSP -G0
   STATIC_LINKING = 1
   LIBS += -lstdc++ -lpthread


# Xbox 360
else ifeq ($(platform), xenon)
   TARGETLIB := $(TARGET_NAME)_libretro_xenon360.a
   CC = xenon-g++$(EXE_EXT)
   AR = xenon-ar$(EXE_EXT)
   CFLAGS += -D__LIBXENON__ -m32 -D__ppc__
   STATIC_LINKING = 1
   BIGENDIAN = 1
   LIBS += -lstdc++ -lpthread


# Nintendo Game Cube
else ifeq ($(platform), ngc)
   TARGETLIB := $(TARGET_NAME)_libretro_$(platform).a
   CC = $(DEVKITPPC)/bin/powerpc-eabi-g++$(EXE_EXT)
   AR = $(DEVKITPPC)/bin/powerpc-eabi-ar$(EXE_EXT)
   CFLAGS += -DGEKKO -DHW_DOL -mrvl -mcpu=750 -meabi -mhard-float -DBLARGG_BIG_ENDIAN=1 -D__ppc__
   STATIC_LINKING = 1
   BIGENDIAN = 1
   LIBS += -lstdc++ -lpthread


# Nintendo Wii
else ifeq ($(platform), wii)
   TARGETLIB := $(TARGET_NAME)_libretro_$(platform).a
   CC = $(DEVKITPPC)/bin/powerpc-eabi-g++$(EXE_EXT)
   AR = $(DEVKITPPC)/bin/powerpc-eabi-ar$(EXE_EXT)
   CFLAGS += -DGEKKO -DHW_RVL -mrvl -mcpu=750 -meabi -mhard-float -DBLARGG_BIG_ENDIAN=1 -D__ppc__
   STATIC_LINKING = 1
   BIGENDIAN = 1
   LIBS += -lstdc++ -lpthread


# Raspberry Pi 2/3
else ifneq (,$(findstring rpi,$(platform)))
   CC = g++
   CC_AS = gcc
   AR = @ar
   NATIVELD = g++
   LD = g++
   TARGETLIB := $(TARGET_NAME)_libretro.so
   SHARED := -shared -Wl,--version-script=src/osd/retro/link.T
   fpic = -fPIC
   LDFLAGS += $(SHARED)
   LIBS += -lstdc++ -lpthread
   CCOMFLAGS += -marm -fomit-frame-pointer -fsingle-precision-constant -ffast-math
   CCOMFLAGS += -fsigned-char -falign-functions=16 -fno-merge-constants
   ARM_ENABLED = 1
   ALIGNED = 1
ifneq (,$(findstring rpi2, $(platform)))
   CCOMFLAGS += -mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard
else ifneq (,$(findstring rpi3, $(platform)))
   CCOMFLAGS += -mcpu=cortex-a53 -mfpu=neon-fp-armv8 -mfloat-abi=hard
endif


# ARM
else ifneq (,$(findstring armv,$(platform)))
   CC = g++
   LD = g++
   TARGETLIB := $(TARGET_NAME)_libretro.so
   SHARED := -shared -Wl,--version-script=src/osd/retro/link.T
   fpic = -fPIC
   LDFLAGS += -Wl,--fix-cortex-a8 $(SHARED)
   LIBS += -lstdc++ -lpthread
   CCOMFLAGS += -marm -ffast-math -mfloat-abi=hard
   ARM_ENABLED = 1
   ALIGNED = 1

# Thanks to 5schatten, EmuELEC: Amlogic S905/S905X/S912 (AMLGXBB/AMLGXL/AMLGXM) e.g. Khadas VIM1/2 / S905X2 (AMLG12A) & S922X/A311D (AMLG12B) e.g. Khadas VIM3 - 32-bit userspace
else ifneq (,$(findstring AMLG,$(platform)))
   TARGETLIB := $(TARGET_NAME)_libretro.so
   SHARED := -shared -Wl,--version-script=src/osd/retro/link.T
   LDFLAGS += -Wl,--fix-cortex-a8 $(SHARED)
   fpic = -fPIC
   LIBS += -lstdc++ -lpthread
   CCOMFLAGS += -marm -ffast-math -mfloat-abi=hard
   ARM_ENABLED = 1
   ALIGNED = 1

  CPUFLAGS += -DNO_ASM -DARM_ASM -frename-registers -ftree-vectorize
  CFLAGS += -march=armv8-a+crc -mfloat-abi=hard -mfpu=neon-fp-armv8 $(CPUFLAGS)
  CXXFLAGS += -march=armv8-a+crc -mfloat-abi=hard -mfpu=neon-fp-armv8 $(CPUFLAGS)
  CCOMFLAGS += -march=armv8-a+crc -mfloat-abi=hard -mfpu=neon-fp-armv8 $(CPUFLAGS)
  
  ifneq (,$(findstring AMLG12,$(platform)))
    ifneq (,$(findstring AMLG12B,$(platform)))
      CFLAGS += -mtune=cortex-a73.cortex-a53
      CXXFLAGS += -mtune=cortex-a73.cortex-a53
      CCOMFLAGS += -mtune=cortex-a73.cortex-a53
    else
      CFLAGS += -mtune=cortex-a53
      CXXFLAGS += -mtune=cortex-a53
      CCOMFLAGS += -mtune=cortex-a53
    endif
  else ifneq (,$(findstring AMLGX,$(platform)))
    CFLAGS += -mtune=cortex-a53
    CXXFLAGS += -mtune=cortex-a53
    CCOMFLAGS += -mtune=cortex-a53
    CORE_DEFINES += -DLOW_END
  endif

  ASFLAGS += $(CFLAGS) -c -frename-registers -fno-strict-aliasing -ffast-math -ftree-vectorize

  PLATFORM_EXT := unix
  WITH_DYNAREC=arm
  HAVE_GENERIC_JIT = 0

#EmuELEC: 64 bits odroid-n2, amlogic
else ifneq (,$(findstring emuelec,$(platform))) 
   TARGETLIB := $(TARGET_NAME)_libretro.so
   SHARED := -shared -Wl,--version-script=src/osd/retro/link.T
   LDFLAGS += $(SHARED)
   fpic = -fPIC
   LIBS += -lstdc++ -lpthread
   ARM_ENABLED = 1
   ALIGNED = 1
   PTR64 = 1

  CPUFLAGS += -DNO_ASM -DARM_ASM -frename-registers -ftree-vectorize
  CFLAGS += -march=armv8-a+crc $(CPUFLAGS)
  CXXFLAGS += -march=armv8-a+crc $(CPUFLAGS)
  CCOMFLAGS += -march=armv8-a+crc $(CPUFLAGS)
  
 ifneq (,$(findstring emuelec-n2,$(platform)))
      CFLAGS += -mtune=cortex-a73.cortex-a53
      CXXFLAGS += -mtune=cortex-a73.cortex-a53
      CCOMFLAGS += -mtune=cortex-a73.cortex-a53
    else
      CFLAGS += -mtune=cortex-a53
      CXXFLAGS += -mtune=cortex-a53
      CCOMFLAGS += -mtune=cortex-a53
    endif

  ASFLAGS += $(CFLAGS) -c -frename-registers -fno-strict-aliasing -ffast-math -ftree-vectorize

  PLATFORM_EXT := unix
  WITH_DYNAREC=arm64
  HAVE_GENERIC_JIT = 0

else ifeq ($(platform), wincross)
   TARGETLIB := $(TARGET_NAME)_libretro.dll
   TARGETOS = win32
   CC ?= g++
   LD ?= g++
   NATIVELD = $(LD)
   CC_AS ?= gcc
   SHARED := -shared -static-libgcc -static-libstdc++ -s -Wl,--version-script=src/osd/retro/link.T
   CCOMFLAGS +=-D__WIN32__ -D__WIN32_LIBRETRO__
ifeq ($(HW_RENDER), 1)
   LIBS += -lopengl32
endif
   LDFLAGS += $(SHARED)
   EXE = .exe
   DEFS = -DCRLF=3


# Windows
else
   TARGETLIB := $(TARGET_NAME)_libretro.dll
   TARGETOS = win32
   CC = g++
   LD = g++
   NATIVELD = $(LD)
   CC_AS = gcc
   SHARED := -shared -static-libgcc -static-libstdc++ -Wl,--version-script=src/osd/retro/link.T
ifneq ($(MDEBUG),1)
   SHARED += -s
endif
   CCOMFLAGS += -D__WIN32__ -D__WIN32_LIBRETRO__
ifeq ($(HW_RENDER), 1)
   LIBS += -lopengl32
endif
   LDFLAGS += $(SHARED)
   EXE = .exe
   DEFS = -DCRLF=3
   DEFS += -DX64_WINDOWS_ABI
endif

# Platform parameters finish

GIT_VERSION ?= " $(shell git rev-parse --short HEAD || echo unknown)"
ifneq ($(GIT_VERSION)," unknown")
	CCOMFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"
endif

ifeq ($(ALIGNED), 1)
	CCOMFLAGS += -DALIGN_INTS -DALIGN_SHORTS
endif

CCOMFLAGS += $(fpic)
LDFLAGS   += $(fpic)

###########################################################################
#################   BEGIN USER-CONFIGURABLE OPTIONS   #####################
###########################################################################

TARGET = mame
SUBTARGET = mame
NOWERROR = 1
OSD = retro
CROSS_BUILD_OSD = retro

#-------------------------------------------------
# specify architecture-specific optimizations
#-------------------------------------------------

# specify optimization level or leave commented to use the default
# (default is OPTIMIZE = 3 normally, or OPTIMIZE = 0 with symbols)
OPTIMIZE = 3

#-------------------------------------------------
# platform-specific definitions
#-------------------------------------------------

# utilities
MD = -mkdir$(EXE_EXT)
RM = @rm -f
OBJDUMP = @objdump

#-------------------------------------------------
# form the name of the executable
#-------------------------------------------------

# reset all internal prefixes/suffixes
SUFFIX64 =
SUFFIXDEBUG =
SUFFIXPROFILE =

# 64-bit builds get a '64' suffix
ifeq ($(PTR64), 1)
   SUFFIX64 = 64
endif

# add an EXE suffix to get the final emulator name
EMULATOR = $(TARGET)

#-------------------------------------------------
# source and object locations
#-------------------------------------------------

# build the targets in different object dirs, so they can co-exist
OBJ = obj/$(PREFIX)$(OSD)$(SUFFIX)$(SUFFIX64)$(SUFFIXDEBUG)$(SUFFIXPROFILE)

#-------------------------------------------------
# compile-time definitions
#-------------------------------------------------
# map the INLINE to something digestible by GCC
DEFS += -DINLINE="static inline"

# define MSB_FIRST if we are a big-endian target
ifeq ($(BIGENDIAN), 1)
   DEFS += -DMSB_FIRST
endif

# define PTR64 if we are a 64-bit target
ifeq ($(PTR64), 1)
   DEFS += -DPTR64
   ifeq ($(OPTFLAG), 1)
	CCOMFLAGS += -march=native -mtune=native
   endif
endif

# need to ensure FLAC functions are statically linked
DEFS += -DFLAC__NO_DLL

# CFLAGS is defined based on C or C++ targets
# (remember, expansion only happens when used, so doing it here is ok)
CFLAGS = $(CCOMFLAGS) $(CPPONLYFLAGS)

# we compile C-only to C89 standard with GNU extensions
# we compile C++ code to C++98 standard with GNU extensions
ifeq ($(platform), osx)
   CONLYFLAGS += -ansi
else
   CONLYFLAGS += -std=gnu89
endif

CPPONLYFLAGS += -x c++ -std=gnu++98
COBJFLAGS += -x objective-c++

# this speeds it up a bit by piping between the preprocessor/compiler/assembler
CCOMFLAGS += -pipe

# add the optimization flag and define MAME_DEBUG if we are a debugging build
# PS: void assert(int expression); - If NDEBUG is defined, do nothing.
ifeq ($(MDEBUG), 1)
   CCOMFLAGS +=  -O0 -g -DMAME_DEBUG
else
   CCOMFLAGS += -O$(OPTIMIZE) -DNDEBUG
endif

# add the error warning flag
ifndef NOWERROR
   CCOMFLAGS += -Werror
endif

# if we are optimizing, include optimization options
ifneq ($(OPTIMIZE), 0)
   CCOMFLAGS += -fno-strict-aliasing $(ARCHOPTS)
endif

# add a basic set of warnings
CCOMFLAGS += \
	-Wall \
	-Wundef \
	-Wformat-security \
	-Wwrite-strings \
	-Wno-sign-compare \
	-Wno-conversion 

# warnings only applicable to C compiles
CONLYFLAGS += \
	-Wpointer-arith \
	-Wbad-function-cast \
	-Wstrict-prototypes

# warnings only applicable to OBJ-C compiles
COBJFLAGS += \
	-Wpointer-arith 

# warnings only applicable to C++ compiles
CPPONLYFLAGS += \
	-Woverloaded-virtual

ifneq (,$(findstring clang,$(CC)))
CCOMFLAGS += \
	-Wno-cast-align \
	-Wno-tautological-compare \
	-Wno-constant-logical-operand \
	-Wno-format-security \
	-Wno-shift-count-overflow \
	-Wno-self-assign-field
endif

ifeq ($(ARM_ENABLED), 1)
   CFLAGS += -DARM_ENABLED
endif

ifeq ($(NEOGEO_BIOS), 1)
   CFLAGS += -DUSE_FULLY
endif


#-------------------------------------------------
# define the standard object directory; other
# projects can add their object directories to
# this variable
#-------------------------------------------------

OBJDIRS = $(OBJ) $(OBJ)/$(TARGET)/$(SUBTARGET)

#-------------------------------------------------
# define standard libarires for CPU and sounds
#-------------------------------------------------
LIBEMU   = $(OBJ)/libemu.a
LIBCPU   = $(OBJ)/libcpu.a
LIBDASM  = $(OBJ)/libdasm.a
LIBSOUND = $(OBJ)/libsound.a
LIBUTIL  = $(OBJ)/libutil.a
LIBOCORE = $(OBJ)/libocore.a
LIBOSD   = $(OSDOBJS)

#-------------------------------------------------
# 'default' target needs to go here, before the
# include files which define additional targets
#-------------------------------------------------

default: maketree buildtools emulator

all: default tools

tests: maketree jedutil$(EXE_EXT) chdman$(EXE_EXT)

7Z_LIB = $(OBJ)/lib7z.a

#-------------------------------------------------
# defines needed by multiple make files
#-------------------------------------------------

BUILDSRC = $(CORE_DIR)/src/build
BUILDOBJ = $(OBJ)/build
BUILDOUT = $(BUILDOBJ)

include makefile.libretro

# combine the various definitions to one
CCOMFLAGS += $(INCFLAGS) -fno-delete-null-pointer-checks
CDEFS = $(DEFS)

#-------------------------------------------------
# primary targets
#-------------------------------------------------

emulator: maketree $(EMULATOR)

buildtools: maketree

ifeq ($(NATIVE),1)
	mkdir prec-build
	cp -R $(OBJ)/build/* prec-build/
	$(RM) -r $(OBJ)/osd
	$(RM) -r $(OBJ)/lib/util
	$(RM) -r $(OBJ)/libutil.a
	$(RM) -r $(OBJ)/libocore.a
endif

tools: maketree $(TOOLS)

maketree: $(sort $(OBJDIRS))

clean: $(OSDCLEAN)
	@echo Deleting object tree $(OBJ)...
	$(RM) -r obj/*
	@echo Deleting $(EMULATOR)...
	$(RM) $(EMULATOR)
	@echo Deleting $(TOOLS)...
	$(RM) $(TOOLS)
	@echo Deleting dependencies...
	$(RM) depend_mame.mak
	$(RM) depend_mess.mak
	$(RM) depend_ume.mak

checkautodetect:
	@echo TARGETOS=$(TARGETOS)
	@echo PTR64=$(PTR64)
	@echo BIGENDIAN=$(BIGENDIAN)
	@echo UNAME="$(UNAME)"

tests: $(REGTESTS)


#-------------------------------------------------
# directory targets
#-------------------------------------------------

$(sort $(OBJDIRS)):
	$(MD) -p $@

ifeq ($(EXTRA_RULES), 1)
   ifeq ($(NATIVE), 0)
      $(OBJ)/build/makedep:
	 cp -R prec-build/makelist $(OBJ)/build
     $(OBJ)/build/makelist:
	 cp -R prec-build/makelist $(OBJ)/build
   endif
endif


#-------------------------------------------------
# executable targets and dependencies
#-------------------------------------------------
$(EMULATOR): $(OBJECTS)
	@echo Linking: $(TARGETLIB)
	@$(LD) $(LDFLAGS) $(LDFLAGSEMULATOR) $^ $(LIBS) -o $(TARGETLIB)

#-------------------------------------------------
# generic rules
#-------------------------------------------------

$(OBJ)/%.o: $(CORE_DIR)/src/%.c | $(OSPREBUILD)
	@$(shell echo echo Compiling:  $<)
	@$(CC) $(CDEFS) $(CFLAGS) -c $< -o $@

$(OBJ)/%.o: $(OBJ)/%.c | $(OSPREBUILD)
	@$(CC) $(CDEFS) $(CFLAGS) -c $< -o $@

$(OBJ)/%.pp: $(CORE_DIR)/src/%.c | $(OSPREBUILD)
	@$(CC) $(CDEFS) $(CFLAGS) -E $< -o $@

$(OBJ)/%.s: $(CORE_DIR)/src/%.c | $(OSPREBUILD)
	@$(CC) $(CDEFS) $(CFLAGS) -S $< -o $@

$(DRIVLISTOBJ): $(DRIVLISTSRC)
	@$(CC) $(CDEFS) $(CFLAGS) -c $< -o $@

$(DRIVLISTSRC): $(CORE_DIR)/src/$(TARGET)/$(SUBTARGET).lst $(MAKELIST_TARGET)
	@echo Building driver list $<...
	@$(MAKELIST) $< >$@

$(OBJ)/%.a:
	@echo Archiving: $@...
	@$(RM) $@
	@$(AR) $(ARFLAGS) $@ $^
