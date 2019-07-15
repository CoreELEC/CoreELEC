# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 0riginally created by Escalade (https://github.com/escalade)
# Copyright (C) 2018-present 5schatten (https://github.com/5schatten)

PKG_NAME="dosbox-sdl2"
PKG_VERSION="d4380b09810f5d07bd86328b9da6b6a82d8e583b"
PKG_SHA256="ceda5ea24ee42dad1867236144d2e3a3f242ee7fc2aef634f4ac9bce9008a809"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/duganchen/dosbox"
PKG_URL="https://github.com/duganchen/dosbox/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain linux glibc glib systemd dbus alsa-lib SDL2-git SDL2_net SDL_sound libpng zlib libvorbis flac libogg fluidsynth-git munt"
PKG_LONGDESC="This is an enhanced SDL2 fork of DOSBox emulator by duganchen. It is currently in sync with revision 4006."
PKG_TOOLCHAIN="autotools"

configure_package() {
  # Displayserver Support
  if [ "${DISPLAYSERVER}" = "x11" ]; then
    PKG_DEPENDS_TARGET+=" xorg-server"
  fi

  if [ "${OPENGL_SUPPORT}" = "yes" ]; then
    PKG_DEPENDS_TARGET+="  ${OPENGL} dosbox-sdl2-shaders glew"
  fi
}

pre_configure_target() {
  PKG_CONFIGURE_OPTS_TARGET="--prefix=/usr \
                             --enable-core-inline \
                             --enable-dynrec \
                             --enable-unaligned_memory \
                             --with-sdl-prefix=$SYSROOT_PREFIX/usr"
  cd ..
  rm -rf .$TARGET_NAME
}

pre_make_target() {
  # Enable DynaRec for ARMv7 cpus
  if [ "$TARGET_CPU" = "cortex-a7" ] || [ "$TARGET_CPU" = "cortex-a53" ] && [ ! $TARGET_CPU = "arm1176jzf-s" ]; then
    sed -i 's|/\* #undef C_DYNREC \*/|#define C_DYNREC 1|' config.h
    sed -i s/C_TARGETCPU.*/C_TARGETCPU\ ARMV7LE/g config.h
    sed -i 's|/\* #undef C_UNALIGNED_MEMORY \*/|#define C_UNALIGNED_MEMORY 1|' config.h
  fi
  # Change version SVN to SDL2
  sed -i s/SVN/SDL2/g config.h
}

post_makeinstall_target() {
  # Create config directory & install config
  mkdir -p $INSTALL/usr/config/dosbox
  cp $PKG_DIR/scripts/* $INSTALL/usr/bin/
  cp $PKG_DIR/config/* $INSTALL/usr/config/dosbox/

  # Enable OpenGL output if supported
  if [ "$OPENGL_SUPPORT" = "yes" ]; then
    sed -i s/output=texture/output=opengl/g $INSTALL/usr/config/dosbox/dosbox-SDL2.conf
  fi
}
