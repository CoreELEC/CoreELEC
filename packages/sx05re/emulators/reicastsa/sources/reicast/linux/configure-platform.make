
# Generic 32 bit x86 (a.k.a. i386/i486/i686)
ifneq (,$(findstring x86,$(platform)))
    PLATFORM_LINUX := 1
    X86_REC := 1
    USE_X11 := 1
    SUPPORT_GLX := 1

    MFLAGS += -m32
    ASFLAGS += -m32
    LDFLAGS += -m32
    CFLAGS += -m32 -D TARGET_LINUX_x86 -D TARGET_NO_AREC -fno-builtin-sqrtf
    CXXFLAGS += -fexceptions

    ifneq (,$(findstring sse4_1,$(platform)))
        HAS_SOFTREND := 1
    endif

# Generic 64 bit x86 (a.k.a. x64/AMD64/x86_64/Intel64/EM64T)
else ifneq (,$(findstring x64,$(platform)))
    PLATFORM_LINUX := 1
    X64_REC := 1
    USE_X11 := 1
    SUPPORT_GLX := 1
    #SUPPORT_EGL := 1
    
    CFLAGS += -D TARGET_LINUX_x64 -D TARGET_NO_AREC -fno-builtin-sqrtf
    CXXFLAGS += -fexceptions

    ifneq (,$(findstring sse4_1,$(platform)))
        HAS_SOFTREND := 1
    endif
# odroidn2
else ifneq (,$(findstring odroidn2,$(platform)))
    PLATFORM_LINUX := 1
	ARM64_REC := 1
	USE_X11 := 1
	SUPPORT_EGL := 1
    CFLAGS += -D TARGET_LINUX_ARMv8 -march=armv8-a+crc -mtune=cortex-a72 -O2 -pipe
    MFLAGS += -march=armv8-a+crc -mtune=cortex-a72 -mfpu=neon-fp-armv8 -mfloat-abi=hard
    ASFLAGS += -march=armv8-a+crc -mtune=cortex-a72 -mfpu=neon-fp-armv8 -mfloat-abi=hard

# Generic 64 bit ARM (armv8) Linux
else ifneq (,$(findstring arm64,$(platform)))
    PLATFORM_LINUX := 1
    ARM64_REC = 1
    USE_X11 = 1
    SUPPORT_GLX = 1
    SUPPORT_EGL = 1
    USE_GLES = 1
    CFLAGS += -march=armv8-a -D TARGET_LINUX_ARMv8 -D TARGET_NO_AREC -fno-builtin-sqrtf -mtune=cortex-a53
    CXXFLAGS += -fexceptions
    
# Generic 32 bit ARMhf (a.k.a. ARMv7h)
else ifneq (,$(findstring armv7h,$(platform)))
    PLATFORM_LINUX := 1
    ARM32_REC := 1
    USE_X11 := 1
    SUPPORT_GLX := 1

    MFLAGS += -marm -mfloat-abi=hard -march=armv7-a -funroll-loops -mfpu=neon
    ASFLAGS += -mfloat-abi=hard -march=armv7-a -mfpu=neon
    ifneq (,$(findstring neon,$(platform)))
        MFLAGS += -mfpu=neon
        ASFLAGS += -mfpu=neon
    endif
    CFLAGS += -D TARGET_LINUX_ARMELv7 -DARM_HARDFP -fsingle-precision-constant

# sun8i Allwinner H2+ / H3 
# like Orange PI, Nano PI, Banana PI, Tritium
else ifneq (,$(findstring sun8i,$(platform)))

    MFLAGS += -marm -mfloat-abi=hard -march=armv7-a -mtune=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard
    ASFLAGS += -marm -mfloat-abi=hard -march=armv7-a -mtune=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard
    CFLAGS +=  -D TARGET_LINUX_ARMELv7 -DARM_HARDFP -funsafe-math-optimizations
    CC = gcc -std=gnu99
    CXX = g++ -std=gnu++14
    ifneq (,$(findstring sdl,$(platform)))
    	CFLAGS += -D GL_GLEXT_PROTOTYPES
        USE_SDL := 1
        USE_GLES := 1
    else
        USE_GLES := 1
    endif

# LinCPP
else ifneq (,$(findstring lincpp,$(platform)))
    CPP_REC := 1
    NOT_ARM := 1
    USE_X11 := 1
    CFLAGS += -D TARGET_LINUX_x64 -D TARGET_NO_JIT
    CXXFLAGS += -fexceptions -std=gnu++14

# Raspberry Pi 4
else ifneq (,$(findstring rpi4,$(platform)))
    PLATFORM_LINUX := 1
	ARM64_REC := 1
	USE_X11 := 1
	SUPPORT_EGL := 1
    CFLAGS += -D TARGET_LINUX_ARMv8 -march=armv8-a+crc -mtune=cortex-a72 -O2 -pipe
    MFLAGS += -march=armv8-a+crc -mtune=cortex-a72 -mfpu=neon-fp-armv8 -mfloat-abi=hard
    ASFLAGS += -march=armv8-a+crc -mtune=cortex-a72 -mfpu=neon-fp-armv8 -mfloat-abi=hard

# Raspberry Pi 2/3
else ifneq (,$(findstring rpi,$(platform)))
    CFLAGS +=  -D TARGET_LINUX_ARMELv7 -DARM_HARDFP -fsingle-precision-constant

    ifneq (,$(findstring rpi2,$(platform)))
        MFLAGS += -mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard
        ASFLAGS += -mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard
    else ifneq (,$(findstring rpi3,$(platform)))
        MFLAGS += -march=armv8-a+crc -mtune=cortex-a53 -mfpu=neon-fp-armv8 -mfloat-abi=hard
        ASFLAGS += -march=armv8-a+crc -mtune=cortex-a53 -mfpu=neon-fp-armv8 -mfloat-abi=hard
    endif

    USE_GLES := 1

    ifneq (,$(findstring mesa,$(platform)))
        USE_SDL := 1
    else
        INCS += -I/opt/vc/include/ -I/opt/vc/include/interface/vmcs_host/linux -I/opt/vc/include/interface/vcos/pthreads
        LIBS += -L/opt/vc/lib/ -lbcm_host -ldl
        LIBS += -lbrcmEGL
        CFLAGS += -D TARGET_VIDEOCORE
        USE_OMX := 1
        USE_DISPMANX := 1
    endif

# BeagleBone Black
else ifneq (,$(findstring beagle,$(platform)))
    CC_PREFIX ?= arm-none-linux-gnueabi-
    MFLAGS += -marm -march=armv7-a -mtune=cortex-a9 -mfpu=neon -mfloat-abi=softfp -funroll-loops
    ASFLAGS := -march=armv7-a -mfpu=neon -mfloat-abi=softfp
    CFLAGS += -fsingle-precision-constant
    USE_GLES := 1

# Pandora
else ifneq (,$(findstring pandora,$(platform)))
    FOR_PANDORA := 1
    USE_X11 := 1
    USE_SDL := 1
    PGO_USE := 1
    USE_GLES := 1
    MFLAGS +== -marm -march=armv7-a -mtune=cortex-a8 -mfpu=neon -mfloat-abi=softfp -funroll-loops -fpermissive
    ASFLAGS += -march=armv7-a -mfpu=neon -mfloat-abi=softfp
    CFLAGS += -D TARGET_PANDORA  -D WEIRD_SLOWNESS -fsingle-precision-constant

# ODROIDs
else ifneq (,$(findstring odroid,$(platform)))
    MFLAGS += -marm -mfpu=neon -mfloat-abi=hard -funroll-loops
    ASFLAGS += -mfpu=neon -mfloat-abi=hard
    CFLAGS += -D TARGET_LINUX_ARMELv7 -DARM_HARDFP -fsingle-precision-constant
    USE_GLES := 1

    # ODROID-XU3, -XU3 Lite & -XU4
    ifneq (,$(findstring odroidxu3,$(platform)))
        MFLAGS += -march=armv7ve -mtune=cortex-a15.cortex-a7
        ASFLAGS += -march=armv7ve
    # Other ODROIDs
    else
        MFLAGS += -march=armv7-a
        ASFLAGS += -march=armv7-a

        # ODROID-C1 & -C1+
        ifneq (,$(findstring odroidc1,$(platform)))
            MFLAGS += -mtune=cortex-a5

        # ODROID-U2, -U3, -X & -X2
        else
            MFLAGS += -mtune=cortex-a9

        endif
    endif

# GCW Zero
else ifneq (,$(findstring gcwz,$(platform)))
    NOT_ARM := 1
    NO_REC := 1
    CC_PREFIX ?= /opt/gcw0-toolchain/usr/bin/mipsel-gcw0-linux-uclibc-
    CFLAGS += -D TARGET_GCW0 -D TARGET_NO_REC -fsingle-precision-constant
    LIBS += -L../linux-deps/lib -L./enta_viv -lglapi
    GCWZ_PKG = reicast-gcwz.opk
    GCWZ_PKG_FILES = gcwz/default.gcw0.desktop gcwz/icon-32.png

# Vero4K
else ifneq (,$(findstring vero4k,$(platform)))
    MFLAGS += -marm -march=armv8-a+crc -mtune=cortex-a53 -mfloat-abi=hard -funsafe-math-optimizations -funroll-loops
    ASFLAGS += -mfloat-abi=hard
    ifneq (,$(findstring neon,$(platform)))
        MFLAGS += -mfpu=neon
        ASFLAGS += -mfpu=neon
    endif
    CFLAGS += -D TARGET_LINUX_ARMELv7 -DARM_HARDFP -fsingle-precision-constant
    INCS += -I/opt/vero3/include/
    LIBS += -L/opt/vero3/lib/ -lEGL
    USE_GLES := 1
    USE_SDL := 1

# Windows
else ifneq (,$(findstring win32,$(platform)))
    PLATFORM_WINDOWS := 1
    SUPPORT_WGL := 1
    
    CFLAGS += -DNOMINMAX -DTARGET_NO_WEBUI -fno-builtin-sqrtf -funroll-loops -DHAVE_FSEEKO -D TARGET_NO_AREC
    LDFLAGS += -static-libgcc -static-libstdc++ -Wl,-subsystem,windows
    LIBS := -lopengl32 -lwinmm -lole32 -lgdi32 -lwsock32 -ldsound -lcomctl32 -lcomdlg32 -lxinput -liphlpapi
    PLATFORM_EXT := exe
    ifeq ($(WITH_DYNAREC), x86)
        X86_REC := 1
        LDFLAGS += -m32
        CFLAGS += -m32
    else
        X64_REC := 1
    endif

    undefine USE_X11
    undefine USE_ALSA
    undefine USE_OSS
    undefine USE_EVDEV
    undefine USE_UDEV
    undefine FOR_LINUX
    undefine WEBUI
    NO_NIXPROF := 1
else
    $(error Unknown platform)
endif
