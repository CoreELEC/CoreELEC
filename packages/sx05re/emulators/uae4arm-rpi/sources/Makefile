ifeq ($(PLATFORM),)
	PLATFORM = rpi2
endif

ifeq ($(PLATFORM),rpi2)
	CPU_FLAGS += -mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard
	MORE_CFLAGS += -DRASPBERRY -DCAPSLOCK_DEBIAN_WORKAROUND -DARMV6T2 -DUSE_JIT_FPU -DARM_HAS_DIV
	LDFLAGS += -lbcm_host
	HAVE_NEON = 1
	HAVE_DISPMANX = 1
	USE_PICASSO96 = 1
else ifeq ($(PLATFORM),rpi1)
	CPU_FLAGS += -mcpu=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard
	MORE_CFLAGS += -DRASPBERRY -DCAPSLOCK_DEBIAN_WORKAROUND
	LDFLAGS += -lbcm_host
	HAVE_DISPMANX = 1
	USE_PICASSO96 = 1
else ifeq ($(PLATFORM),generic-sdl)
#	ifneq ($(findstring raspberrypi,$(shell uname -a)),)
	CPU_FLAGS= -mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard
#	endif
	MORE_CFLAGS += -DARMV6T2
	HAVE_SDL_DISPLAY = 1
else ifeq ($(PLATFORM),gles)
	CPU_FLAGS= -mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard
	# Uncomment below line to activate shader support. It's very slown on Allwinner.
#	MORE_CFLAGS += -DSHADER_SUPPORT
	# Uncomment below line to activate threading. This is buggy on Allwinner.
   # MORE_CFLAGS += -DUSE_RENDER_THREAD
	MORE_CFLAGS += -DARMV6T2 -DUSE_JIT_FPU
	HAVE_GLES_DISPLAY = 1
	HAVE_NEON = 1
	USE_PICASSO96 = 1
endif

GIT_VERSION := $(shell git rev-parse --short HEAD 2>/dev/null)
ifneq ($(GIT_VERSION),)
   MORE_CFLAGS += -DGIT_VERSION=$(GIT_VERSION)
endif

NAME   = uae4arm
RM     = rm -f
CXX    = g++
STRIP  = strip

PROG   = $(NAME)

all: $(PROG)

DEBUG=1
TRACER=1

PANDORA=1
#GEN_PROFILE=1
#USE_PROFILE=1

SDL_CFLAGS = `sdl-config --cflags`

DEFS +=  `xml2-config --cflags`
DEFS += -DCPU_arm -DARMV6_ASSEMBLY -DPANDORA
DEFS += -DWITH_INGAME_WARNING
DEFS += -DROM_PATH_PREFIX=\"./\" -DDATA_PREFIX=\"./data/\" -DSAVE_PREFIX=\"./saves/\"
DEFS += -DUSE_SDL

ifeq ($(USE_PICASSO96), 1)
	DEFS += -DPICASSO96
endif

ifeq ($(HAVE_NEON), 1)
	DEFS += -DUSE_ARMNEON
endif

MORE_CFLAGS += -I/opt/vc/include -I/opt/vc/include/interface/vmcs_host/linux -I/opt/vc/include/interface/vcos/pthreads

MORE_CFLAGS += -Isrc -Isrc/od-pandora  -Isrc/threaddep -Isrc/menu -Isrc/include -Isrc/archivers -Isrc/od-pandora -Wno-unused -Wno-format  -DGCCCONSTFUNC="__attribute__((const))"
MORE_CFLAGS += -fexceptions -fpermissive -std=gnu++11 -marm

LDFLAGS += -lSDL -lpthread -lm -lz -lSDL_image -lpng -lrt -lxml2 -lFLAC -lmpg123 -ldl -lmpeg2convert -lmpeg2
LDFLAGS += -lSDL_ttf -lguichan_sdl -lguichan -L/opt/vc/lib 

ifndef DEBUG
MORE_CFLAGS += -Ofast -fomit-frame-pointer
MORE_CFLAGS += -finline -fno-builtin
else
MORE_CFLAGS += -g -DDEBUG -Wl,--export-dynamic -DWITH_LOGGING

#MORE_CFLAGS += -fsanitize=address -fsanitize=bounds
#LDFLAGS += -static-libasan -fsanitize=address -fsanitize=bounds

ifdef TRACER
TRACE_CFLAGS = -DTRACER -finstrument-functions -Wall -rdynamic
endif

endif

ASFLAGS += $(CPU_FLAGS)

CXXFLAGS += $(SDL_CFLAGS) $(CPU_FLAGS) $(DEFS) $(MORE_CFLAGS)

ifdef GEN_PROFILE
MORE_CFLAGS += -fprofile-generate=/media/MAINSD/pandora/test -fprofile-arcs -fvpt
endif
ifdef USE_PROFILE
MORE_CFLAGS += -fprofile-use -fbranch-probabilities -fvpt
endif


CAPS = capsimg.so

capsimg.so: capsimg
	(cd capsimg ; ./bootstrap ; ./configure --host=arm; $(MAKE) ; cp capsimg.so ../ )

OBJS =	\
	src/akiko.o \
	src/ar.o \
	src/aros.rom.o \
	src/audio.o \
	src/autoconf.o \
	src/blitfunc.o \
	src/blittable.o \
	src/blitter.o \
	src/blkdev.o \
	src/blkdev_cdimage.o \
	src/bsdsocket.o \
	src/calc.o \
	src/cd32_fmv.o \
	src/cd32_fmv_genlock.o \
	src/cdrom.o \
	src/cfgfile.o \
	src/cia.o \
	src/crc32.o \
	src/custom.o \
	src/def_icons.o \
	src/devices.o \
	src/disk.o \
	src/diskutil.o \
	src/dlopen.o \
	src/drawing.o \
	src/events.o \
	src/expansion.o \
	src/fdi2raw.o \
	src/filesys.o \
	src/flashrom.o \
	src/fpp.o \
	src/fsdb.o \
	src/fsdb_unix.o \
	src/fsusage.o \
	src/gayle.o \
	src/gfxboard.o \
	src/gfxutil.o \
	src/hardfile.o \
	src/hrtmon.rom.o \
	src/ide.o \
	src/inputdevice.o \
	src/keybuf.o \
	src/main.o \
	src/memory.o \
	src/native2amiga.o \
	src/rommgr.o \
	src/rtc.o \
	src/savestate.o \
	src/scsi.o \
	src/statusline.o \
	src/traps.o \
	src/uaelib.o \
	src/uaeresource.o \
	src/zfile.o \
	src/zfile_archive.o \
	src/archivers/7z/7zAlloc.o \
	src/archivers/7z/7zBuf.o \
	src/archivers/7z/7zCrc.o \
	src/archivers/7z/7zCrcOpt.o \
	src/archivers/7z/7zDec.o \
	src/archivers/7z/7zIn.o \
	src/archivers/7z/7zStream.o \
	src/archivers/7z/Bcj2.o \
	src/archivers/7z/Bra.o \
	src/archivers/7z/Bra86.o \
	src/archivers/7z/LzmaDec.o \
	src/archivers/7z/Lzma2Dec.o \
	src/archivers/7z/BraIA64.o \
	src/archivers/7z/Delta.o \
	src/archivers/7z/Sha256.o \
	src/archivers/7z/Xz.o \
	src/archivers/7z/XzCrc64.o \
	src/archivers/7z/XzDec.o \
	src/archivers/dms/crc_csum.o \
	src/archivers/dms/getbits.o \
	src/archivers/dms/maketbl.o \
	src/archivers/dms/pfile.o \
	src/archivers/dms/tables.o \
	src/archivers/dms/u_deep.o \
	src/archivers/dms/u_heavy.o \
	src/archivers/dms/u_init.o \
	src/archivers/dms/u_medium.o \
	src/archivers/dms/u_quick.o \
	src/archivers/dms/u_rle.o \
	src/archivers/lha/crcio.o \
	src/archivers/lha/dhuf.o \
	src/archivers/lha/header.o \
	src/archivers/lha/huf.o \
	src/archivers/lha/larc.o \
	src/archivers/lha/lhamaketbl.o \
	src/archivers/lha/lharc.o \
	src/archivers/lha/shuf.o \
	src/archivers/lha/slide.o \
	src/archivers/lha/uae_lha.o \
	src/archivers/lha/util.o \
	src/archivers/lzx/unlzx.o \
	src/archivers/mp2/kjmp2.o \
	src/archivers/wrp/warp.o \
	src/archivers/zip/unzip.o \
	src/caps/caps_win32.o \
	src/md-pandora/support.o \
	src/od-pandora/bsdsocket_host.o \
	src/od-pandora/cda_play.o \
	src/od-pandora/charset.o \
	src/od-pandora/fsdb_host.o \
	src/od-pandora/hardfile_pandora.o \
	src/od-pandora/keyboard.o \
	src/od-pandora/mp3decoder.o \
	src/od-pandora/writelog.o \
	src/od-pandora/pandora.o \
	src/od-pandora/pandora_filesys.o \
	src/od-pandora/pandora_input.o \
	src/od-pandora/pandora_gui.o \
	src/od-pandora/pandora_rp9.o \
	src/od-pandora/pandora_mem.o \
	src/od-pandora/sigsegv_handler.o \
	src/sd-sdl/sound_sdl_new.o \
	src/od-pandora/gui/UaeRadioButton.o \
	src/od-pandora/gui/UaeDropDown.o \
	src/od-pandora/gui/UaeCheckBox.o \
	src/od-pandora/gui/UaeListBox.o \
	src/od-pandora/gui/SelectorEntry.o \
	src/od-pandora/gui/ShowHelp.o \
	src/od-pandora/gui/ShowMessage.o \
	src/od-pandora/gui/SelectFolder.o \
	src/od-pandora/gui/SelectFile.o \
	src/od-pandora/gui/CreateFilesysHardfile.o \
	src/od-pandora/gui/EditFilesysVirtual.o \
	src/od-pandora/gui/EditFilesysHardfile.o \
	src/od-pandora/gui/PanelPaths.o \
	src/od-pandora/gui/PanelQuickstart.o \
	src/od-pandora/gui/PanelConfig.o \
	src/od-pandora/gui/PanelCPU.o \
	src/od-pandora/gui/PanelChipset.o \
	src/od-pandora/gui/PanelROM.o \
	src/od-pandora/gui/PanelRAM.o \
	src/od-pandora/gui/PanelFloppy.o \
	src/od-pandora/gui/PanelHD.o \
	src/od-pandora/gui/PanelDisplay.o \
	src/od-pandora/gui/PanelSound.o \
	src/od-pandora/gui/PanelInput.o \
	src/od-pandora/gui/PanelMisc.o \
	src/od-pandora/gui/PanelSavestate.o \
	src/od-pandora/gui/main_window.o \
	src/od-pandora/gui/Navigation.o

ifeq ($(HAVE_DISPMANX), 1)
OBJS += src/od-rasp/rasp_gfx.o
endif

ifeq ($(HAVE_SDL_DISPLAY), 1)
OBJS += src/od-pandora/pandora_gfx.o
endif

ifeq ($(HAVE_GLES_DISPLAY), 1)
OBJS += src/od-gles/gl.o
OBJS += src/od-gles/shader_stuff.o
OBJS += src/od-gles/gl_platform.o
OBJS += src/od-gles/gles_gfx.o
MORE_CFLAGS += -I/opt/vc/include/
MORE_CFLAGS += -DHAVE_GLES
LDFLAGS +=  -ldl
ifneq (,$(wildcard /opt/vc/lib/libbrcmGLESv2.so))
LDFLAGS += -lbrcmEGL -lbrcmGLESv2
else
#LDFLAGS += -L/usr/lib/arm-linux-gnueabihf/mali-egl
LDFLAGS += -lEGL -lGL -lGLESv2
endif
endif


ifdef PANDORA
OBJS += src/od-pandora/gui/sdltruetypefont.o
endif

ifeq ($(USE_PICASSO96), 1)
	OBJS += src/od-pandora/picasso96.o
endif

ifeq ($(HAVE_NEON), 1)
	OBJS += src/od-pandora/neon_helper.o
else
	OBJS += src/od-pandora/arm_helper.o
endif


OBJS += src/newcpu.o
OBJS += src/newcpu_common.o
OBJS += src/readcpu.o
OBJS += src/cpudefs.o
OBJS += src/cpustbl.o
OBJS += src/cpuemu_0.o
OBJS += src/cpuemu_4.o
OBJS += src/cpuemu_11.o
OBJS += src/cpuemu_40.o
OBJS += src/cpuemu_44.o
OBJS += src/jit/compemu.o
OBJS += src/jit/compstbl.o
OBJS += src/jit/compemu_fpp.o
OBJS += src/jit/compemu_support.o

src/od-pandora/neon_helper.o: src/od-pandora/neon_helper.s
	$(CXX) -march=armv7-a -mfpu=neon -Wall -o src/od-pandora/neon_helper.o -c src/od-pandora/neon_helper.s

src/od-pandora/arm_helper.o: src/od-pandora/arm_helper.s
	$(CXX) -Wall -o src/od-pandora/arm_helper.o -c src/od-pandora/arm_helper.s



src/trace.o: src/trace.c
	$(CC) $(MORE_CFLAGS) -c src/trace.c -o src/trace.o

$(PROG): $(OBJS) $(CAPS)
	$(CXX) -o $(PROG) $(OBJS) $(LDFLAGS)
#ifndef DEBUG
#	$(STRIP) $(PROG)
#endif

clean:
	$(RM) $(PROG) $(OBJS) $(CAPS)
	(cd capsimg ; make clean )
