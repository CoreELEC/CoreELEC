# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="retroarch_32"
PKG_VERSION="a308be6e87f305bcb219d7599bf956406d3857c2"
PKG_SITE="https://github.com/libretro/RetroArch"
PKG_URL="$PKG_SITE.git"
PKG_LICENSE="GPLv3"
PKG_DEPENDS_TARGET="toolchain SDL2-git alsa-lib openssl freetype zlib ffmpeg libass $OPENGLES avahi nss-mdns freetype openal-soft libusb"
PKG_LONGDESC="Reference frontend for the libretro API."
GET_HANDLER_SUPPORT="git"

if [ ${PROJECT} = "Amlogic-ng" ]; then
  PKG_PATCH_DIRS="${PROJECT}"
fi

if [ "$DEVICE" == "OdroidGoAdvance" ]; then
PKG_DEPENDS_TARGET+=" libdrm librga"
fi

# Pulseaudio Support
  if [ "${PULSEAUDIO_SUPPORT}" = yes ]; then
    PKG_DEPENDS_TARGET+=" pulseaudio"
fi

pre_configure_target() {
TARGET_CONFIGURE_OPTS=""
PKG_CONFIGURE_OPTS_TARGET="--disable-qt \
                           --enable-alsa \
                           --enable-udev \
                           --disable-opengl1 \
                           --disable-opengl \
                           --enable-egl \
                           --enable-opengles \
                           --disable-wayland \
                           --disable-x11 \
                           --enable-zlib \
                           --enable-freetype \
                           --disable-discord \
                           --disable-vg \
                           --disable-sdl \
                           --enable-sdl2 \
                           --enable-ffmpeg"

if [ "$DEVICE" == "OdroidGoAdvance" ]; then
PKG_CONFIGURE_OPTS_TARGET+=" --enable-opengles3 \
                           --enable-kms \
                           --disable-mali_fbdev \
                           --enable-odroidgo2"
else
PKG_CONFIGURE_OPTS_TARGET+=" --disable-kms \
                           --enable-mali_fbdev"
fi

if [ $ARCH == "arm" ]; then
PKG_CONFIGURE_OPTS_TARGET+=" --enable-neon"
fi

cd $PKG_BUILD
}

make_target() {
  make HAVE_UPDATE_ASSETS=1 HAVE_LIBRETRODB=1 HAVE_NETWORKING=1 HAVE_LAKKA=1 HAVE_ZARCH=1 HAVE_QT=0 HAVE_LANGEXTRA=1
  [ $? -eq 0 ] && echo "(retroarch ok)" || { echo "(retroarch failed)" ; exit 1 ; }
  make -C gfx/video_filters compiler=$CC extra_flags="$CFLAGS"
[ $? -eq 0 ] && echo "(video filters ok)" || { echo "(video filters failed)" ; exit 1 ; }
  make -C libretro-common/audio/dsp_filters compiler=$CC extra_flags="$CFLAGS"
[ $? -eq 0 ] && echo "(audio filters ok)" || { echo "(audio filters failed)" ; exit 1 ; }
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/bin
  mkdir -p $INSTALL/etc
    cp $PKG_BUILD/retroarch $INSTALL/usr/bin/
    cp $PKG_BUILD/retroarch.cfg $INSTALL/etc
  mkdir -p $INSTALL/usr/share/video_filters
    cp $PKG_BUILD/gfx/video_filters/*.so $INSTALL/usr/share/video_filters
    cp $PKG_BUILD/gfx/video_filters/*.filt $INSTALL/usr/share/video_filters
  mkdir -p $INSTALL/usr/share/audio_filters
    cp $PKG_BUILD/libretro-common/audio/dsp_filters/*.so $INSTALL/usr/share/audio_filters
    cp $PKG_BUILD/libretro-common/audio/dsp_filters/*.dsp $INSTALL/usr/share/audio_filters
  
}
