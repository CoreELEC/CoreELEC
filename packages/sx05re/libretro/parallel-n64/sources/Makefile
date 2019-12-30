DEBUG=0
PERF_TEST=0
HAVE_SHARED_CONTEXT=0
WITH_CRC=brumme
FORCE_GLES=1
HAVE_OPENGL=0
HAVE_VULKAN_DEBUG=0
GLIDEN64=0
GLIDEN64CORE=0
GLIDEN64ES=0
HAVE_RSP_DUMP=0
HAVE_RDP_DUMP=0
HAVE_RICE=1
HAVE_PARALLEL=1
HAVE_PARALLEL_RSP=0
STATIC_LINKING=0
HAVE_NEON=0

DYNAFLAGS :=
INCFLAGS  :=
COREFLAGS :=
CPUFLAGS  :=
GLFLAGS   :=

UNAME=$(shell uname -a)

SPACE :=
SPACE := $(SPACE) $(SPACE)
BACKSLASH :=
BACKSLASH := \$(BACKSLASH)
filter_out1 = $(filter-out $(firstword $1),$1)
filter_out2 = $(call filter_out1,$(call filter_out1,$1))

ifeq ($(HAVE_VULKAN_DEBUG),1)
HAVE_RSP_DUMP=1
HAVE_RDP_DUMP=1
endif

# Dirs
ROOT_DIR := .
LIBRETRO_DIR := $(ROOT_DIR)/libretro

ifeq ($(platform),)
   platform = unix
   ifeq ($(UNAME),)
      platform = win
   else ifneq ($(findstring MINGW,$(UNAME)),)
      platform = win
   else ifneq ($(findstring Darwin,$(UNAME)),)
      platform = osx
   else ifneq ($(findstring win,$(UNAME)),)
      platform = win
   endif
else ifneq (,$(findstring armv,$(platform)))
   override platform += unix
else ifneq (,$(findstring rpi,$(platform)))
   override platform += unix
else ifneq (,$(findstring odroid,$(platform)))
   override platform += unix
endif

# system platform
system_platform = unix
ifeq ($(shell uname -a),)
   EXE_EXT = .exe
   system_platform = win
else ifneq ($(findstring Darwin,$(shell uname -a)),)
   system_platform = osx
   arch = intel
ifeq ($(shell uname -p),powerpc)
   arch = ppc
endif
else ifneq ($(findstring MINGW,$(shell uname -a)),)
   system_platform = win
endif

# Cross compile ?

ifeq (,$(ARCH))
   ARCH = $(shell uname -m)
endif

# Target Dynarec
WITH_DYNAREC = $(ARCH)

ifeq ($(ARCH), $(filter $(ARCH), i386 i686))
   WITH_DYNAREC = x86
else ifeq ($(ARCH), $(filter $(ARCH), arm))
   WITH_DYNAREC = arm
else ifeq ($(ARCH), $(filter $(ARCH), aarch64))
   WITH_DYNAREC = aarch64
endif

ifeq ($(HAVE_VULKAN_DEBUG),1)
TARGET_NAME := parallel_n64_debug
else
TARGET_NAME := parallel_n64
endif

CC_AS ?= $(CC)

GIT_VERSION ?= " $(shell git rev-parse --short HEAD || echo unknown)"
ifneq ($(GIT_VERSION)," unknown")
	COREFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"
endif

# Unix
ifneq (,$(findstring unix,$(platform)))
   TARGET := $(TARGET_NAME)_libretro.so
   LDFLAGS += -shared -Wl,--no-undefined
	ifeq ($(DEBUG_JIT),)
	   LDFLAGS += -Wl,--version-script=$(LIBRETRO_DIR)/link.T
	endif
   fpic = -fPIC

	HAVE_THR_AL=1

#ifeq ($(WITH_DYNAREC), $(filter $(WITH_DYNAREC), x86_64 x64))
#ifeq ($(HAVE_PARALLEL), 1)
	#HAVE_PARALLEL_RSP=1
#endif
#endif

   ifeq ($(FORCE_GLES),1)
      GLES = 1
      GL_LIB := -lGLESv2
   else ifneq (,$(findstring gles,$(platform)))
      GLES = 1
      GL_LIB := -lGLESv2
   else ifeq ($(HAVE_OPENGL),1)
      GL_LIB := -lGL
   endif

   # Raspberry Pi
   ifneq (,$(findstring rpi,$(platform)))
      GLES = 1

      ifneq (,$(findstring mesa,$(platform)))
         GL_LIB := -lGLESv2
      else
         GL_LIB := -L/opt/vc/lib -lbrcmGLESv2
         INCFLAGS += -I/opt/vc/include -I/opt/vc/include/interface/vcos -I/opt/vc/include/interface/vcos/pthreads
      endif

      WITH_DYNAREC=arm
      ifneq (,$(findstring rpi2,$(platform)))
         CPUFLAGS += -DNO_ASM -DARM -D__arm__ -DARM_ASM -D__NEON_OPT -DNOSSE
         CPUFLAGS += -mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard
         HAVE_NEON = 1
      else ifneq (,$(findstring rpi3,$(platform)))
         CPUFLAGS += -DNO_ASM -DARM -D__arm__ -DARM_ASM -D__NEON_OPT -DNOSSE
         CPUFLAGS += -mcpu=cortex-a53 -mfpu=neon-fp-armv8 -mfloat-abi=hard
         HAVE_NEON = 1
      else
         CPUFLAGS += -DARMv5_ONLY -DNO_ASM
      endif
   endif

   # ODROIDs
   ifneq (,$(findstring odroid,$(platform)))
      BOARD := $(shell cat /proc/cpuinfo | grep -i odroid | awk '{print $$3}')
      GLES = 1
      GL_LIB := -lGLESv2
      CPUFLAGS += -DNO_ASM -DARM -D__arm__ -DARM_ASM -D__NEON_OPT -DNOSSE
      CPUFLAGS += -marm -mfloat-abi=hard
      HAVE_NEON = 1
      WITH_DYNAREC=arm
      ifneq (,$(findstring ODROIDC,$(BOARD)))
         # ODROID-C1
         CPUFLAGS += -mcpu=cortex-a5 -mfpu=neon
      else ifneq (,$(findstring ODROID-XU3,$(BOARD)))
         # ODROID-XU4 (on older kernel), XU3 & XU3 Lite
         ifeq "$(shell expr `gcc -dumpversion` \>= 4.9)" "1"
            CPUFLAGS += -march=armv7ve -mcpu=cortex-a15.cortex-a7 -mfpu=neon-vfpv4
         else
            CPUFLAGS += -mcpu=cortex-a9 -mfpu=neon
         endif
      else ifneq (,$(findstring ODROID-XU4,$(BOARD)))
         # ODROID-XU4 on newer kernels now identify as ODROID-XU4
         ifeq "$(shell expr `gcc -dumpversion` \>= 4.9)" "1"
            CPUFLAGS += -march=armv7ve -mcpu=cortex-a15.cortex-a7 -mfpu=neon-vfpv4
         else
            CPUFLAGS += -mcpu=cortex-a9 -mfpu=neon
         endif
      else
         # ODROID-U3, U2, X2 & X
         CPUFLAGS += -mcpu=cortex-a9 -mfpu=neon
      endif
   endif
   
   # Classic Platforms ####################
   # Platform affix = classic_<ISA>_<µARCH>
   # Help at https://modmyclassic.com/comp

   # (armv7 a7, hard point, neon based) ### 
   # NESC, SNESC, C64 mini 
   ifneq (,$(findstring classic_armv7_a7, $(platform)))
      GLES = 1
      GL_LIB := -lGLESv2
      HAVE_NEON = 1
      WITH_DYNAREC=arm
      ASFLAGS += -D__ARM_NEON__ -marm -mtune=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard
      CPUFLAGS += -DNO_ASM -DARM -D__arm__ -DARM_ASM -D__NEON_OPT -DNOSSE -DARM_FIX -DCLASSIC
      CPUFLAGS += -Ofast \
      -flto=4 -fwhole-program -fuse-linker-plugin \
      -fdata-sections -ffunction-sections -Wl,--gc-sections \
      -fno-stack-protector -fno-ident -fomit-frame-pointer \
      -falign-functions=1 -falign-jumps=1 -falign-loops=1 \
      -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-unroll-loops \
      -fmerge-all-constants -fno-math-errno \
      -marm -mtune=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard
      LDFLAGS += -marm -mtune=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard
      ifeq ($(shell echo `$(CC) -dumpversion` "< 4.9" | bc -l), 1)
        CPUFLAGS += -march=armv7-a
      else
        CPUFLAGS += -march=armv7ve
        # If gcc is 5.0 or later
        ifeq ($(shell echo `$(CC) -dumpversion` ">= 5" | bc -l), 1)
          LDFLAGS += -static-libgcc -static-libstdc++
        endif
      endif
	endif  
# (armv8 a35, hard point, neon based) ###
# PlayStation Classic
ifneq (,$(findstring classic_armv8_a35, $(platform)))
	GLES = 1
	GL_LIB := -lGLESv2
	HAVE_NEON = 1
	WITH_DYNAREC=arm
	ASFLAGS += -D__ARM_NEON__ -marm -mtune=cortex-a35 -mfpu=neon-fp-armv8 -mfloat-abi=hard
	CPUFLAGS += -DNO_ASM -DARM -D__arm__ -DARM_ASM -D__NEON_OPT -DNOSSE -DARM_FIX -DCLASSIC
	CPUFLAGS += -Ofast \
	-flto -fwhole-program -fuse-linker-plugin \
	-fdata-sections -ffunction-sections -Wl,--gc-sections \
	-fno-stack-protector -fno-ident -fomit-frame-pointer \
	-falign-functions=1 -falign-jumps=1 -falign-loops=1 \
	-fno-unwind-tables -fno-asynchronous-unwind-tables -fno-unroll-loops \
	-fmerge-all-constants -fno-math-errno \
	-marm -mtune=cortex-a35 -mfpu=neon-fp-armv8 -mfloat-abi=hard
	LDFLAGS += -marm -mtune=cortex-a35 -mfpu=neon-fp-armv8 -mfloat-abi=hard
        CPUFLAGS += -march=armv8-a
        LDFLAGS += -static-libgcc -static-libstdc++
	endif
 #######################################
   # Generic ARMV8 - cross - No GL 
   else ifneq (,$(findstring armv8,$(platform)))
      #CC = aarch64-linux-gnu-gcc
      #CXX = aarch64-linux-gnu-g++
      CPUFLAGS += -DNO_ASM -DARM -DARM_ASM -DDONT_WANT_ARM_OPTIMIZATIONS -DARM_FIX -DCLASSIC -DARM64
      LDFLAGS += -static-libgcc -static-libstdc++ -arch arm64 
      GLES = 1
      HAVE_NEON=1
      GL_LIB := -lGLESv2
      HAVE_OPENGL=1
      WITH_DYNAREC=aarch64
      ifneq (,$(findstring neon,$(platform)))
         CPUFLAGS += -D__NEON_OPT -mfpu=neon
         HAVE_NEON = 1
      endif
 #######################################
   
   # Generic ARM
   else ifneq (,$(findstring armv,$(platform)))
      CPUFLAGS += -DNO_ASM -DARM -D__arm__ -DARM_ASM -DNOSSE
      WITH_DYNAREC=arm
      ifneq (,$(findstring neon,$(platform)))
         CPUFLAGS += -D__NEON_OPT -mfpu=neon
         HAVE_NEON = 1
      endif

   PLATFORM_EXT := unix

# i.MX6
else ifneq (,$(findstring imx6,$(platform)))
   TARGET := $(TARGET_NAME)_libretro.so
   LDFLAGS += -shared -Wl,--version-script=$(LIBRETRO_DIR)/link.T
   fpic = -fPIC
   GLES = 1
   GL_LIB := -lGLESv2
   CPUFLAGS += -DNO_ASM
   PLATFORM_EXT := unix
   WITH_DYNAREC=arm
   HAVE_NEON=1

# OS X
else ifneq (,$(findstring osx,$(platform)))
   TARGET := $(TARGET_NAME)_libretro.dylib
   LDFLAGS += -dynamiclib
   OSXVER = `sw_vers -productVersion | cut -d. -f 2`
   OSX_LT_MAVERICKS = `(( $(OSXVER) <= 9)) && echo "YES"`
        LDFLAGS += -mmacosx-version-min=10.7
   LDFLAGS += -stdlib=libc++
   fpic = -fPIC

HAVE_THR_AL=1
   HAVE_PARALLEL=0
   PLATCFLAGS += -D__MACOSX__ -DOSX
   GL_LIB := -framework OpenGL
   PLATFORM_EXT := unix

   # Target Dynarec
   ifeq ($(ARCH), $(filter $(ARCH), ppc))
      WITH_DYNAREC =
   endif

# iOS
else ifneq (,$(findstring ios,$(platform)))
   ifeq ($(IOSSDK),)
      IOSSDK := $(shell xcodebuild -version -sdk iphoneos Path)
   endif

   TARGET := $(TARGET_NAME)_libretro_ios.dylib
   DEFINES += -DIOS
   GLES = 1
	WITH_DYNAREC=
   PLATFORM_EXT := unix

   HAVE_PARALLEL=0
   PLATCFLAGS += -DHAVE_POSIX_MEMALIGN -DNO_ASM
   PLATCFLAGS += -DIOS -marm
   CPUFLAGS += -DNO_ASM  -DARM -D__arm__ -DARM_ASM -D__NEON_OPT
   LDFLAGS += -dynamiclib

   fpic = -fPIC
   GL_LIB := -framework OpenGLES
   ifeq ($(platform),ios-arm64)
      CC = clang -arch arm64 -isysroot $(IOSSDK)
      CXX = clang++ -arch arm64 -isysroot $(IOSSDK)
      CFLAGS += -DDONT_WANT_ARM_OPTIMIZATIONS
      FORCE_GLES=1
      CXXFLAGS += -Wc++11-extensions -std=c++11 -stdlib=libc++ -Wc++11-long-long
      CPUFLAGS += -marm -mfpu=neon -mfloat-abi=softfp
      HAVE_NEON=0
   else
      CPUFLAGS += -marm -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=softfp
      CC = clang -arch armv7 -isysroot $(IOSSDK)
      CXX = clang++ -arch armv7 -isysroot $(IOSSDK)
      HAVE_NEON=1
   endif
   CC_AS = perl ./tools/gas-preprocessor.pl $(CC)
   ifeq ($(platform),ios9)
      CC         += -miphoneos-version-min=8.0
      CC_AS      += -miphoneos-version-min=8.0
      CXX        += -miphoneos-version-min=8.0
      PLATCFLAGS += -miphoneos-version-min=8.0
   else
      CC += -miphoneos-version-min=5.0
      CC_AS += -miphoneos-version-min=5.0
      CXX += -miphoneos-version-min=5.0
      PLATCFLAGS += -miphoneos-version-min=5.0
   endif

# Theos iOS
else ifneq (,$(findstring theos_ios,$(platform)))
   DEPLOYMENT_IOSVERSION = 5.0
   TARGET = iphone:latest:$(DEPLOYMENT_IOSVERSION)
   ARCHS = armv7
   TARGET_IPHONEOS_DEPLOYMENT_VERSION=$(DEPLOYMENT_IOSVERSION)
   THEOS_BUILD_DIR := objs
   include $(THEOS)/makefiles/common.mk

   LIBRARY_NAME = $(TARGET_NAME)_libretro_ios
   DEFINES += -DIOS
   GLES = 1
   WITH_DYNAREC=arm

   PLATCFLAGS += -DHAVE_POSIX_MEMALIGN -DNO_ASM
   PLATCFLAGS += -DIOS -marm
   CPUFLAGS += -DNO_ASM  -DARM -D__arm__ -DARM_ASM -D__NEON_OPT -DNOSSE
   HAVE_NEON=1


# Android
else ifneq (,$(findstring android,$(platform)))
   fpic = -fPIC
   TARGET := $(TARGET_NAME)_libretro_android.so
   LDFLAGS += -shared -Wl,--version-script=$(LIBRETRO_DIR)/link.T -Wl,--no-undefined -Wl,--warn-common
   GL_LIB := -lGLESv2

   CC = arm-linux-androideabi-gcc
   CXX = arm-linux-androideabi-g++
   WITH_DYNAREC=arm
   GLES = 1
   PLATCFLAGS += -DANDROID
   CPUCFLAGS  += -DNO_ASM
   HAVE_NEON = 1
   CPUFLAGS += -marm -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=softfp -D__arm__ -DARM_ASM -D__NEON_OPT
   CFLAGS += -DANDROID

   PLATFORM_EXT := unix

# QNX
else ifeq ($(platform), qnx)
   fpic = -fPIC
   TARGET := $(TARGET_NAME)_libretro_$(platform).so
   LDFLAGS += -shared -Wl,--version-script=$(LIBRETRO_DIR)/link.T -Wl,--no-undefined -Wl,--warn-common
   GL_LIB := -lGLESv2

   CC = qcc -Vgcc_ntoarmv7le
   CC_AS = qcc -Vgcc_ntoarmv7le
   CXX = QCC -Vgcc_ntoarmv7le
   AR = QCC -Vgcc_ntoarmv7le
   WITH_DYNAREC=arm
   GLES = 1
   PLATCFLAGS += -DNO_ASM -D__BLACKBERRY_QNX__
   CPUFLAGS += -DNOSSE
   HAVE_NEON = 1
   CPUFLAGS += -marm -mcpu=cortex-a9 -mfpu=neon -mfloat-abi=softfp -D__arm__ -DARM_ASM -D__NEON_OPT -DNOSSE
   CFLAGS += -D__QNX__

   PLATFORM_EXT := unix

# emscripten
else ifeq ($(platform), emscripten)
   TARGET := $(TARGET_NAME)_libretro_$(platform).bc
   GLES := 1
   WITH_DYNAREC :=

   HAVE_PARALLEL = 0
   CPUFLAGS += -DNOSSE -DEMSCRIPTEN -DNO_ASM -DNO_LIBCO -s USE_ZLIB=1 -s PRECISE_F32=1

   WITH_DYNAREC =
   CC = emcc
   CXX = em++
   HAVE_NEON = 0
   PLATFORM_EXT := unix
   STATIC_LINKING = 1
   SOURCES_C += $(CORE_DIR)/src/r4300/empty_dynarec.c

# PlayStation Vita
else ifneq (,$(findstring vita,$(platform)))
   TARGET:= $(TARGET_NAME)_libretro_$(platform).a
   CPUFLAGS += -DNO_ASM  -DARM -D__arm__ -DARM_ASM -D__NEON_OPT
   CPUFLAGS += -w -mthumb -mcpu=cortex-a9 -mfpu=neon -mfloat-abi=hard -D__arm__ -DARM_ASM -D__NEON_OPT -g
   HAVE_NEON = 1

   PREFIX = arm-vita-eabi
   CC = $(PREFIX)-gcc
   CXX = $(PREFIX)-g++
   AR  = $(PREFIX)-ar
   WITH_DYNAREC = arm
   DYNAREC_USED = 0
   GLES = 0
   HAVE_OPENGL = 0
   PLATCFLAGS += -DVITA
   CPUCFLAGS += -DNO_ASM
   CFLAGS += -DVITA -lm
   VITA = 1
   HAVE_PARALLEL=0
   SOURCES_C += $(CORE_DIR)/src/r4300/empty_dynarec.c

   PLATFORM_EXT := unix
   STATIC_LINKING=1

# Windows MSVC 2017 all architectures
else ifneq (,$(findstring windows_msvc2017,$(platform)))

	PlatformSuffix = $(subst windows_msvc2017_,,$(platform))
	ifneq (,$(findstring desktop,$(PlatformSuffix)))
		WinPartition = desktop
        MSVC2017CompileFlags = -DWINAPI_FAMILY=WINAPI_FAMILY_DESKTOP_APP
		LDFLAGS += -MANIFEST -LTCG:incremental -NXCOMPAT -DYNAMICBASE -DEBUG -OPT:REF -INCREMENTAL:NO -SUBSYSTEM:WINDOWS -MANIFESTUAC:"level='asInvoker' uiAccess='false'" -OPT:ICF -ERRORREPORT:PROMPT -NOLOGO -TLBID:1
		LIBS += kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib
	else ifneq (,$(findstring uwp,$(PlatformSuffix)))
		WinPartition = uwp
		MSVC2017CompileFlags = -DWINAPI_FAMILY=WINAPI_FAMILY_APP -D_WINDLL -D_UNICODE -DUNICODE -D__WRL_NO_DEFAULT_LIB__ -EHsc
		LDFLAGS += -APPCONTAINER -NXCOMPAT -DYNAMICBASE -MANIFEST:NO -LTCG -OPT:REF -SUBSYSTEM:CONSOLE -MANIFESTUAC:NO -OPT:ICF -ERRORREPORT:PROMPT -NOLOGO -TLBID:1 -DEBUG:FULL -WINMD:NO
		LIBS += WindowsApp.lib

        HAVE_OPENGL = 0
	endif

    WITH_DYNAREC :=
    SOURCES_C += $(CORE_DIR)/src/r4300/empty_dynarec.c

	CFLAGS += $(MSVC2017CompileFlags)
	CXXFLAGS += $(MSVC2017CompileFlags)

	TargetArchMoniker = $(subst $(WinPartition)_,,$(PlatformSuffix))

	CC  = cl.exe
	CXX = cl.exe
	CC_AS = nasm.exe
        ASFLAGS += -f win64
	GL_LIB = opengl32.lib
	HAVE_PARALLEL=0
	HAVE_PARALLEL_RSP=0
	HAVE_THR_AL=1

	reg_query = $(call filter_out2,$(subst $2,,$(shell reg query "$2" -v "$1" 2>nul)))
	fix_path = $(subst $(SPACE),\ ,$(subst \,/,$1))

	ProgramFiles86w := $(shell cmd /c "echo %PROGRAMFILES(x86)%")
	ProgramFiles86 := $(shell cygpath "$(ProgramFiles86w)")

	WindowsSdkDir ?= $(call reg_query,InstallationFolder,HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Microsoft SDKs\Windows\v10.0)
	WindowsSdkDir ?= $(call reg_query,InstallationFolder,HKEY_CURRENT_USER\SOFTWARE\Wow6432Node\Microsoft\Microsoft SDKs\Windows\v10.0)
	WindowsSdkDir ?= $(call reg_query,InstallationFolder,HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Microsoft SDKs\Windows\v10.0)
	WindowsSdkDir ?= $(call reg_query,InstallationFolder,HKEY_CURRENT_USER\SOFTWARE\Microsoft\Microsoft SDKs\Windows\v10.0)
	WindowsSdkDir := $(WindowsSdkDir)

	WindowsSDKVersion ?= $(firstword $(foreach folder,$(subst $(subst \,/,$(WindowsSdkDir)Include/),,$(wildcard $(call fix_path,$(WindowsSdkDir)Include\*))),$(if $(wildcard $(call fix_path,$(WindowsSdkDir)Include/$(folder)/um/Windows.h)),$(folder),)))$(BACKSLASH)
	WindowsSDKVersion := $(WindowsSDKVersion)

	VsInstallBuildTools = $(ProgramFiles86)/Microsoft Visual Studio/2017/BuildTools
	VsInstallEnterprise = $(ProgramFiles86)/Microsoft Visual Studio/2017/Enterprise
	VsInstallProfessional = $(ProgramFiles86)/Microsoft Visual Studio/2017/Professional
	VsInstallCommunity = $(ProgramFiles86)/Microsoft Visual Studio/2017/Community

	VsInstallRoot ?= $(shell if [ -d "$(VsInstallBuildTools)" ]; then echo "$(VsInstallBuildTools)"; fi)
	ifeq ($(VsInstallRoot), )
		VsInstallRoot = $(shell if [ -d "$(VsInstallEnterprise)" ]; then echo "$(VsInstallEnterprise)"; fi)
	endif
	ifeq ($(VsInstallRoot), )
		VsInstallRoot = $(shell if [ -d "$(VsInstallProfessional)" ]; then echo "$(VsInstallProfessional)"; fi)
	endif
	ifeq ($(VsInstallRoot), )
		VsInstallRoot = $(shell if [ -d "$(VsInstallCommunity)" ]; then echo "$(VsInstallCommunity)"; fi)
	endif
	VsInstallRoot := $(VsInstallRoot)

	VcCompilerToolsVer := $(shell cat "$(VsInstallRoot)/VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt" | grep -o '[0-9\.]*')
	VcCompilerToolsDir := $(VsInstallRoot)/VC/Tools/MSVC/$(VcCompilerToolsVer)

	WindowsSDKSharedIncludeDir := $(shell cygpath -w "$(WindowsSdkDir)\Include\$(WindowsSDKVersion)\shared")
	WindowsSDKUCRTIncludeDir := $(shell cygpath -w "$(WindowsSdkDir)\Include\$(WindowsSDKVersion)\ucrt")
	WindowsSDKUMIncludeDir := $(shell cygpath -w "$(WindowsSdkDir)\Include\$(WindowsSDKVersion)\um")
	WindowsSDKUCRTLibDir := $(shell cygpath -w "$(WindowsSdkDir)\Lib\$(WindowsSDKVersion)\ucrt\$(TargetArchMoniker)")
	WindowsSDKUMLibDir := $(shell cygpath -w "$(WindowsSdkDir)\Lib\$(WindowsSDKVersion)\um\$(TargetArchMoniker)")

	# For some reason the HostX86 compiler doesn't like compiling for x64
	# ("no such file" opening a shared library), and vice-versa.
	# Work around it for now by using the strictly x86 compiler for x86, and x64 for x64.
	# NOTE: What about ARM?
	ifneq (,$(findstring x64,$(TargetArchMoniker)))
		VCCompilerToolsBinDir := $(VcCompilerToolsDir)\bin\HostX64
		LDFLAGS += -MACHINE:X64
	else
		VCCompilerToolsBinDir := $(VcCompilerToolsDir)\bin\HostX86
		LDFLAGS += -MACHINE:X86
	endif

	PATH := $(shell IFS=$$'\n'; cygpath "$(VCCompilerToolsBinDir)/$(TargetArchMoniker)"):$(PATH)
	PATH := $(PATH):$(shell IFS=$$'\n'; cygpath "$(VsInstallRoot)/Common7/IDE")
	INCLUDE := $(shell IFS=$$'\n'; cygpath -w "$(VcCompilerToolsDir)/include")
	LIB := $(shell IFS=$$'\n'; cygpath -w "$(VcCompilerToolsDir)/lib/$(TargetArchMoniker)")
	ifneq (,$(findstring uwp,$(PlatformSuffix)))
		LIB := $(shell IFS=$$'\n'; cygpath -w "$(LIB)/store")
	endif
    
	export INCLUDE := $(INCLUDE);$(WindowsSDKSharedIncludeDir);$(WindowsSDKUCRTIncludeDir);$(WindowsSDKUMIncludeDir)
	export LIB := $(LIB);$(WindowsSDKUCRTLibDir);$(WindowsSDKUMLibDir)
	TARGET := $(TARGET_NAME)_libretro.dll
	PSS_STYLE :=2
	LDFLAGS += -DLL

# Windows MSVC 2013 x86
else ifeq ($(platform), windows_msvc2013_x86)
	CC  = cl.exe
	CXX = cl.exe
	CC_AS = nasm.exe

PATH := $(shell IFS=$$'\n'; cygpath "$(VS120COMNTOOLS)../../VC/bin"):$(PATH)
PATH := $(PATH):$(shell IFS=$$'\n'; cygpath "$(VS120COMNTOOLS)../IDE")
LIB := $(shell IFS=$$'\n'; cygpath -w "$(VS120COMNTOOLS)../../VC/lib")
INCLUDE := $(shell IFS=$$'\n'; cygpath -w "$(VS120COMNTOOLS)../../VC/include")

WindowsSdkDir := $(shell reg query "HKLM\SOFTWARE\Microsoft\Windows Kits\Installed Roots" -v "KitsRoot81" | grep -o '[A-Z]:\\.*')

WindowsSdkDirInc := $(WindowsSdkDir)Include

INCFLAGS_PLATFORM = -I"$(WindowsSdkDirInc)\um" -I"$(WindowsSdkDirInc)\shared"
export INCLUDE := $(INCLUDE)
export LIB := $(LIB);$(WindowsSdkDir)\Lib;$(WindowsSdkDir)\Lib\winv6.3\um\x86
TARGET := $(TARGET_NAME)_libretro.dll
PSS_STYLE :=2
ASFLAGS += -f win32
LDFLAGS += -DLL -MACHINE:X86
GL_LIB = opengl32.lib
HAVE_PARALLEL=0
HAVE_PARALLEL_RSP=0
WITH_DYNAREC=x86

# Windows MSVC 2013 x64
else ifeq ($(platform), windows_msvc2013_x64)
	CC  = cl.exe
	CXX = cl.exe
	CC_AS = nasm.exe

PATH := $(shell IFS=$$'\n'; cygpath "$(VS120COMNTOOLS)../../VC/bin/amd64"):$(PATH)
PATH := $(PATH):$(shell IFS=$$'\n'; cygpath "$(VS120COMNTOOLS)../IDE")
LIB := $(shell IFS=$$'\n'; cygpath -w "$(VS120COMNTOOLS)../../VC/lib/amd64")
INCLUDE := $(shell IFS=$$'\n'; cygpath -w "$(VS120COMNTOOLS)../../VC/include")

WindowsSdkDir := $(shell reg query "HKLM\SOFTWARE\Microsoft\Windows Kits\Installed Roots" -v "KitsRoot81" | grep -o '[A-Z]:\\.*')

WindowsSdkDirInc := $(WindowsSdkDir)Include

INCFLAGS_PLATFORM = -I"$(WindowsSdkDirInc)\um" -I"$(WindowsSdkDirInc)\shared"
export INCLUDE := $(INCLUDE)
export LIB := $(LIB);$(WindowsSdkDir)\Lib;$(WindowsSdkDir)\Lib\winv6.3\um\x64
TARGET := $(TARGET_NAME)_libretro.dll
PSS_STYLE :=2
ASFLAGS += -f win64
LDFLAGS += -DLL -MACHINE:X64
GL_LIB = opengl32.lib
HAVE_PARALLEL=0
HAVE_PARALLEL_RSP=0

# Windows MSVC 2015 x64
else ifeq ($(platform), windows_msvc2015_x64)
	CC  = cl.exe
	CXX = cl.exe
	CC_AS = nasm.exe

PATH := $(shell IFS=$$'\n'; cygpath "$(VS140COMNTOOLS)../../VC/bin/amd64"):$(PATH)
PATH := $(PATH):$(shell IFS=$$'\n'; cygpath "$(VS140COMNTOOLS)../IDE")
LIB := $(shell IFS=$$'\n'; cygpath -w "$(VS140COMNTOOLS)../../VC/lib/amd64")
INCLUDE := $(shell IFS=$$'\n'; cygpath -w "$(VS140COMNTOOLS)../../VC/include")
ASFLAGS += -f win64

reg_query = $(call filter_out2,$(subst $2,,$(shell reg query "$2" -v "$1" 2>nul)))
fix_path = $(subst $(SPACE),\ ,$(subst \,/,$1))
WindowsSdkDir ?= $(call reg_query,InstallationFolder,HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Microsoft SDKs\Windows\v10.0)
WindowsSdkDir ?= $(call reg_query,InstallationFolder,HKEY_CURRENT_USER\SOFTWARE\Wow6432Node\Microsoft\Microsoft SDKs\Windows\v10.0)
WindowsSdkDir ?= $(call reg_query,InstallationFolder,HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Microsoft SDKs\Windows\v10.0)
WindowsSdkDir ?= $(call reg_query,InstallationFolder,HKEY_CURRENT_USER\SOFTWARE\Microsoft\Microsoft SDKs\Windows\v10.0)
WindowsSdkDir := $(WindowsSdkDir)

WindowsSDKVersion ?= $(firstword $(foreach folder,$(subst $(subst \,/,$(WindowsSdkDir)Include/),,$(wildcard $(call fix_path,$(WindowsSdkDir)Include\*))),$(if $(wildcard $(call fix_path,$(WindowsSdkDir)Include/$(folder)/um/Windows.h)),$(folder),)))$(BACKSLASH)
WindowsSDKVersion := $(WindowsSDKVersion)

export INCLUDE := $(INCLUDE);$(VCINSTALLDIR)INCLUDE;$(VCINSTALLDIR)ATLMFC\INCLUDE;$(WindowsSdkDir)include\$(WindowsSDKVersion)ucrt;$(WindowsSdkDir)include\$(WindowsSDKVersion)shared;$(WindowsSdkDir)include\$(WindowsSDKVersion)um
export LIB := $(LIB);$(VCINSTALLDIR)LIB\amd64;$(VCINSTALLDIR)ATLMFC\LIB\amd64;$(WindowsSdkDir)lib\$(WindowsSDKVersion)ucrt\x64;$(WindowsSdkDir)lib\$(WindowsSDKVersion)um\x64

INCFLAGS_PLATFORM = -I"$(WindowsSDKVersion)um" -I"$(WindowsSDKVersion)shared"
TARGET := $(TARGET_NAME)_libretro.dll
PSS_STYLE :=2
LDFLAGS += -DLL -MACHINE:X64
GL_LIB = opengl32.lib
HAVE_PARALLEL=0
HAVE_PARALLEL_RSP=0

# Windows MSVC 2015 x86
else ifeq ($(platform), windows_msvc2015_x86)
	CC  = cl.exe
	CXX = cl.exe
	CC_AS = nasm.exe

PATH := $(shell IFS=$$'\n'; cygpath "$(VS140COMNTOOLS)../../VC/bin"):$(PATH)
PATH := $(PATH):$(shell IFS=$$'\n'; cygpath "$(VS140COMNTOOLS)../IDE")
LIB := $(shell IFS=$$'\n'; cygpath -w "$(VS140COMNTOOLS)../../VC/lib")
INCLUDE := $(shell IFS=$$'\n'; cygpath -w "$(VS140COMNTOOLS)../../VC/include")
ASFLAGS += -f win32

reg_query = $(call filter_out2,$(subst $2,,$(shell reg query "$2" -v "$1" 2>nul)))
fix_path = $(subst $(SPACE),\ ,$(subst \,/,$1))
WindowsSdkDir ?= $(call reg_query,InstallationFolder,HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Microsoft SDKs\Windows\v10.0)
WindowsSdkDir ?= $(call reg_query,InstallationFolder,HKEY_CURRENT_USER\SOFTWARE\Wow6432Node\Microsoft\Microsoft SDKs\Windows\v10.0)
WindowsSdkDir ?= $(call reg_query,InstallationFolder,HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Microsoft SDKs\Windows\v10.0)
WindowsSdkDir ?= $(call reg_query,InstallationFolder,HKEY_CURRENT_USER\SOFTWARE\Microsoft\Microsoft SDKs\Windows\v10.0)
WindowsSdkDir := $(WindowsSdkDir)

WindowsSDKVersion ?= $(firstword $(foreach folder,$(subst $(subst \,/,$(WindowsSdkDir)Include/),,$(wildcard $(call fix_path,$(WindowsSdkDir)Include\*))),$(if $(wildcard $(call fix_path,$(WindowsSdkDir)Include/$(folder)/um/Windows.h)),$(folder),)))$(BACKSLASH)
WindowsSDKVersion := $(WindowsSDKVersion)

export INCLUDE := $(INCLUDE);$(VCINSTALLDIR)INCLUDE;$(VCINSTALLDIR)ATLMFC\INCLUDE;$(WindowsSdkDir)include\$(WindowsSDKVersion)ucrt;$(WindowsSdkDir)include\$(WindowsSDKVersion)shared;$(WindowsSdkDir)include\$(WindowsSDKVersion)um
export LIB := $(LIB);$(VCINSTALLDIR)LIB;$(VCINSTALLDIR)ATLMFC\LIB;$(WindowsSdkDir)lib\$(WindowsSDKVersion)ucrt\x86;$(WindowsSdkDir)lib\$(WindowsSDKVersion)um\x86

INCFLAGS_PLATFORM = -I"$(WindowsSDKVersion)um" -I"$(WindowsSDKVersion)shared"
TARGET := $(TARGET_NAME)_libretro.dll
PSS_STYLE :=2
LDFLAGS += -DLL -MACHINE:X86
GL_LIB = opengl32.lib
HAVE_PARALLEL=0
HAVE_PARALLEL_RSP=0

# Windows MSVC 2010 x64
else ifeq ($(platform), windows_msvc2010_x64)
	CC  = cl.exe
	CXX = cl.exe
	CC_AS = nasm.exe

PATH := $(shell IFS=$$'\n'; cygpath "$(VS100COMNTOOLS)../../VC/bin/amd64"):$(PATH)
PATH := $(PATH):$(shell IFS=$$'\n'; cygpath "$(VS100COMNTOOLS)../IDE")
LIB := $(shell IFS=$$'\n'; cygpath "$(VS100COMNTOOLS)../../VC/lib/amd64")
INCLUDE := $(shell IFS=$$'\n'; cygpath -w "$(VS100COMNTOOLS)../../VC/include")

WindowsSdkDir := $(shell reg query "HKLM\SOFTWARE\Microsoft\Microsoft SDKs\Windows\v7.0A" -v "InstallationFolder" | grep -o '[A-Z]:\\.*')lib/x64
WindowsSdkDir ?= $(shell reg query "HKLM\SOFTWARE\Microsoft\Microsoft SDKs\Windows\v7.1A" -v "InstallationFolder" | grep -o '[A-Z]:\\.*')lib/x64

WindowsSdkDirInc := $(shell reg query "HKLM\SOFTWARE\Microsoft\Microsoft SDKs\Windows\v7.0A" -v "InstallationFolder" | grep -o '[A-Z]:\\.*')Include
WindowsSdkDirInc ?= $(shell reg query "HKLM\SOFTWARE\Microsoft\Microsoft SDKs\Windows\v7.1A" -v "InstallationFolder" | grep -o '[A-Z]:\\.*')Include


INCFLAGS_PLATFORM = -I"$(WindowsSdkDirInc)"
export INCLUDE := $(INCLUDE)
export LIB := $(LIB);$(WindowsSdkDir)
TARGET := $(TARGET_NAME)_libretro.dll
PSS_STYLE :=2
ASFLAGS += -f win64
LDFLAGS += -DLL -MACHINE:X64
GL_LIB = opengl32.lib
HAVE_PARALLEL=0
HAVE_PARALLEL_RSP=0

# Windows MSVC 2010 x86
else ifeq ($(platform), windows_msvc2010_x86)
	CC  = cl.exe
	CXX = cl.exe
	CC_AS = nasm.exe

PATH := $(shell IFS=$$'\n'; cygpath "$(VS100COMNTOOLS)../../VC/bin"):$(PATH)
PATH := $(PATH):$(shell IFS=$$'\n'; cygpath "$(VS100COMNTOOLS)../IDE")
LIB := $(shell IFS=$$'\n'; cygpath -w "$(VS100COMNTOOLS)../../VC/lib")
INCLUDE := $(shell IFS=$$'\n'; cygpath -w "$(VS100COMNTOOLS)../../VC/include")

WindowsSdkDir := $(shell reg query "HKLM\SOFTWARE\Microsoft\Microsoft SDKs\Windows\v7.0A" -v "InstallationFolder" | grep -o '[A-Z]:\\.*')lib
WindowsSdkDir ?= $(shell reg query "HKLM\SOFTWARE\Microsoft\Microsoft SDKs\Windows\v7.1A" -v "InstallationFolder" | grep -o '[A-Z]:\\.*')lib

WindowsSdkDirInc := $(shell reg query "HKLM\SOFTWARE\Microsoft\Microsoft SDKs\Windows\v7.0A" -v "InstallationFolder" | grep -o '[A-Z]:\\.*')Include
WindowsSdkDirInc ?= $(shell reg query "HKLM\SOFTWARE\Microsoft\Microsoft SDKs\Windows\v7.1A" -v "InstallationFolder" | grep -o '[A-Z]:\\.*')Include


INCFLAGS_PLATFORM = -I"$(WindowsSdkDirInc)"
export INCLUDE := $(INCLUDE)
export LIB := $(LIB);$(WindowsSdkDir)
TARGET := $(TARGET_NAME)_libretro.dll
PSS_STYLE :=2
ASFLAGS += -f win32
LDFLAGS += -DLL -MACHINE:X86
GL_LIB = opengl32.lib
HAVE_PARALLEL=0
HAVE_PARALLEL_RSP=0

# Windows MSVC 2005 x86
else ifeq ($(platform), windows_msvc2005_x86)
	CC  = cl.exe
	CXX = cl.exe
	CC_AS = nasm.exe
	HAS_GCC := 0

PATH := $(shell IFS=$$'\n'; cygpath "$(VS80COMNTOOLS)../../VC/bin"):$(PATH)
PATH := $(PATH):$(shell IFS=$$'\n'; cygpath "$(VS80COMNTOOLS)../IDE")
INCLUDE := $(shell IFS=$$'\n'; cygpath -w "$(VS80COMNTOOLS)../../VC/include")
LIB := $(shell IFS=$$'\n'; cygpath -w "$(VS80COMNTOOLS)../../VC/lib")
BIN := $(shell IFS=$$'\n'; cygpath "$(VS80COMNTOOLS)../../VC/bin")

WindowsSdkDir := $(INETSDK)

export INCLUDE := $(INCLUDE);$(INETSDK)/Include;libretro-common/include/compat/msvc
export LIB := $(LIB);$(WindowsSdkDir);$(INETSDK)/Lib
TARGET := $(TARGET_NAME)_libretro.dll
PSS_STYLE :=2
ASFLAGS += -f win32
LDFLAGS += -DLL -MACHINE:X86
CFLAGS += -D_CRT_SECURE_NO_DEPRECATE
LIBS =
GL_LIB = opengl32.lib
HAVE_PARALLEL=0
HAVE_PARALLEL_RSP=0
NOSSE=1

# Windows
else ifneq (,$(findstring win,$(platform)))
   TARGET := $(TARGET_NAME)_libretro.dll
   LDFLAGS += -shared -static-libgcc -static-libstdc++ -Wl,--version-script=$(LIBRETRO_DIR)/link.T -lwinmm -lgdi32 
   GL_LIB := -lopengl32 -lversion
   PLATFORM_EXT := win32
   CC = gcc
   CXX = g++
   HAVE_THR_AL=1

#ifeq ($(WITH_DYNAREC), $(filter $(WITH_DYNAREC), x86_64 x64))
#ifeq ($(HAVE_PARALLEL), 1)
	#HAVE_PARALLEL_RSP=1
   #CPUFLAGS += -D_POSIX_SOURCE
#endif
#endif

endif

ifneq ($(SANITIZER),)
    CFLAGS   += -fsanitize=$(SANITIZER)
    CXXFLAGS += -fsanitize=$(SANITIZER)
    LDFLAGS  += -fsanitize=$(SANITIZER)
endif

ifeq ($(HAVE_OPENGL),0)
	HAVE_GLIDE64=0
	HAVE_GLN64=0
	HAVE_GLIDEN64=0
	HAVE_RICE=0
endif

include Makefile.common

ifeq ($(HAVE_NEON), 1)
   COREFLAGS += -DHAVE_NEON
endif

ifeq ($(PERF_TEST), 1)
   COREFLAGS += -DPERF_TEST
endif

ifeq ($(HAVE_SHARED_CONTEXT), 1)
   COREFLAGS += -DHAVE_SHARED_CONTEXT
endif

COREFLAGS += -D__LIBRETRO__ -DM64P_PLUGIN_API -DM64P_CORE_PROTOTYPES -D_ENDUSER_RELEASE -DSINC_LOWER_QUALITY

OBJOUT   = -o 
LINKOUT  = -o 

ifneq (,$(findstring msvc,$(platform)))
	OBJOUT = -Fo
	LINKOUT = -out:
	LD = link.exe
else
	LD = $(CXX)
endif

ifeq ($(DEBUG), 1)
   CPUOPTS += -DOPENGL_DEBUG

   ifneq (,$(findstring msvc,$(platform)))
      ifeq (,$(findstring msvc2005,$(platform)))
      ifeq (,$(findstring msvc2010,$(platform)))
				 # /FS was added in 2013
         CPUOPTS += -FS
      endif
      endif

      ifeq ($(STATIC_LINKING),1)
      CPUOPTS += -Od -MTd -Zi
      else
      CPUOPTS += -Od -MDd -Zi
      endif

      LDFLAGS += -DEBUG
   else
      CPUOPTS += -O0 -g
   endif
else
ifneq (,$(findstring msvc,$(platform)))
   CPUOPTS += -O2
else
	CPUOPTS += -O3
endif
   CPUOPTS += -DNDEBUG
ifneq ($(findstring Darwin,$(UNAME)),)
   CPUOPTS += -flto
else ifeq ($(findstring msvc,$(platform)),)
ifneq ($(platform), emscripten)
ifneq ($(shell $(CC) -v 2>&1 | grep -c "clang"),1)
   CPUOPTS += -fipa-pta
endif
endif
endif


   ifneq (,$(findstring msvc,$(platform)))
      ifeq (,$(findstring msvc2005,$(platform)))
      ifeq (,$(findstring msvc2010,$(platform)))
				 # /FS was added in 2013
         CPUOPTS += -FS
      endif
      endif

      ifeq ($(STATIC_LINKING),1)
      CPUOPTS += -MT -Zi
      else
      CPUOPTS += -MD -Zi
      endif

      ifeq (,$(findstring msvc2017,$(platform)))
      CPUOPTS += -EHsc -D_CRT_SECURE_NO_WARNINGS -D_ENDUSER_RELEASE -D__LIBRETRO_WIN64__ -D__SSE2__ -DUNICODE -D_UNICODE -D_USRDLL -DWIN32 -D_WINDLL -D_WINDOWS -WX- -Zc:forScope -Zc:wchar_t -Zi -wd4996 -W0 -fp:precise -Gd -GL -Gm- -GS- -Gy -DMSVC2010_EXPORTS -Oi -Ot
      LDFLAGS += -LTCG -DYNAMICBASE -ERRORREPORT:QUEUE -INCREMENTAL:NO -MANIFEST:NO -NXCOMPAT -OPT:ICF -OPT:REF -SUBSYSTEM:WINDOWS,"5.02" -TLBID:1 advapi32.lib comdlg32.lib gdi32.lib kernel32.lib odbc32.lib odbccp32.lib ole32.lib oleaut32.lib shell32.lib user32.lib uuid.lib winspool.lib
      else
      CPUOPTS += -EHsc -D_CRT_SECURE_NO_WARNINGS -D_ENDUSER_RELEASE -D__LIBRETRO_WIN64__ -D__SSE2__ -DUNICODE -D_UNICODE -D_USRDLL -DWIN32 -D_WINDLL -D_WINDOWS -WX- -Zc:forScope -Zc:wchar_t -Zi -wd4996 -W0 -fp:precise -Gd -GL -Gm- -GS- -Gy -DMSVC2010_EXPORTS -Oi -Ot
      LDFLAGS += -LTCG -DYNAMICBASE -ERRORREPORT:QUEUE -INCREMENTAL:NO -NXCOMPAT -OPT:ICF -OPT:REF
      endif
   endif
endif

WANT_CXX11=0

ifeq ($(platform), qnx)
   CFLAGS   += -Wp,-MMD
   CXXFLAGS += -Wp,-MMD
else
ifeq ($(GLIDEN64),1)
   CFLAGS   += -DGLIDEN64
   CXXFLAGS += -DGLIDEN64
   WANT_CXX11=1
   CFLAGS   += -MMD
   CXXFLAGS += -MMD
else ifeq ($(HAVE_PARALLEL), 1)
   WANT_CXX11=1
   ifeq (,$(findstring msvc,$(platform)))
     CFLAGS   += -MMD
     CXXFLAGS += -MMD
   else
     CFLAGS   += -MT
     CXXFLAGS += -MT
   endif
else ifeq (,$(findstring msvc,$(platform)))
    CFLAGS   += -MMD
    ifneq ($(platform),ios-arm64)
        CXXFLAGS += -std=c++98 -MMD
    endif
endif
ifeq ($(GLIDEN64ES),1)
   CFLAGS   += -DGLIDEN64ES
   CXXFLAGS += -DGLIDEN64ES
endif
ifeq ($(GLIDEN64CORE),1)
   CFLAGS += -DCORE
   CXXFLAGS += -DCORE
endif
endif

ifeq ($(HAVE_THR_AL), 1)
WANT_CXX11=1
endif

ifeq ($(WANT_CXX11),1)
ifeq (,$(findstring msvc,$(platform)))
CXXFLAGS += -std=c++0x
endif
endif

ifneq (,$(findstring msvc,$(platform)))
CFLAGS += -DINLINE="_inline"
CXXFLAGS += -DINLINE="_inline"
else
CFLAGS += -DINLINE="inline"
CXXFLAGS += -DINLINE="inline"
endif

ASFLAGS     := $(ASFLAGS) $(CFLAGS)

### Finalize ###
OBJECTS     += $(SOURCES_CXX:.cpp=.o) $(SOURCES_C:.c=.o) $(SOURCES_ASM:.S=.o)
CXXFLAGS    += $(CPUOPTS) $(COREFLAGS) $(INCFLAGS) $(INCFLAGS_PLATFORM) $(PLATCFLAGS) $(fpic) $(PLATCFLAGS) $(CPUFLAGS) $(GLFLAGS) $(DYNAFLAGS)
CFLAGS      += $(CPUOPTS) $(COREFLAGS) $(INCFLAGS) $(INCFLAGS_PLATFORM) $(PLATCFLAGS) $(fpic) $(PLATCFLAGS) $(CPUFLAGS) $(GLFLAGS) $(DYNAFLAGS)

ifeq ($(findstring Haiku,$(UNAME)),)
ifeq (,$(findstring msvc,$(platform)))
   LDFLAGS += -lm
endif
endif

LDFLAGS    += $(fpic)

ifeq ($(platform), theos_ios)
COMMON_FLAGS := -DIOS $(COMMON_DEFINES) $(INCFLAGS) $(INCFLAGS_PLATFORM) -I$(THEOS_INCLUDE_PATH) -Wno-error
$(LIBRARY_NAME)_ASFLAGS += $(CFLAGS) $(COMMON_FLAGS)
$(LIBRARY_NAME)_CFLAGS += $(CFLAGS) $(COMMON_FLAGS)
$(LIBRARY_NAME)_CXXFLAGS += $(CXXFLAGS) $(COMMON_FLAGS)
${LIBRARY_NAME}_FILES = $(SOURCES_CXX) $(SOURCES_C) $(SOURCES_ASM)
${LIBRARY_NAME}_FRAMEWORKS = OpenGLES
${LIBRARY_NAME}_LIBRARIES = z
include $(THEOS_MAKE_PATH)/library.mk
else
all: $(TARGET)
$(TARGET): $(OBJECTS)
ifeq ($(STATIC_LINKING), 1)
	$(AR) rcs $@ $(OBJECTS)
else
	$(LD) $(LINKOUT)$@ $(OBJECTS) $(LDFLAGS) $(GL_LIB) $(LIBS)
endif


%.o: %.S
ifneq (,$(findstring msvc,$(platform)))
	$(CC_AS) $(ASFLAGS) -o$@ $<
else
	$(CC_AS) $(ASFLAGS) -c $< $(OBJOUT)$@
endif

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< $(OBJOUT)$@

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< $(OBJOUT)$@


clean:
	rm -f $(OBJECTS) $(TARGET) $(OBJECTS:.o=.d)

.PHONY: clean
-include $(OBJECTS:.o=.d)
endif

print-%:
	@echo '$*=$($*)'
