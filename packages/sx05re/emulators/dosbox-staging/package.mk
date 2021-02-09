# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="dosbox-staging"
PKG_VERSION="d6ed9a9ef9f3058a7415b02e9236988563263da4"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/dosbox-staging/dosbox-staging"
PKG_URL="$PKG_SITE.git"
PKG_DEPENDS_TARGET="toolchain linux glibc glib systemd dbus alsa-lib SDL2-git SDL2_net SDL_sound libpng zlib libvorbis flac libogg fluidsynth-git munt opusfile"
PKG_LONGDESC="DOS/x86 emulator focusing on ease of use "
PKG_TOOLCHAIN="autotools"
PKG_BUILD_FLAGS="+lto"

pre_configure_target() {

# Clean up build directory
rm -rf .${TARGET_NAME}

  PKG_CONFIGURE_OPTS_TARGET="--prefix=/usr \
                             --enable-core-inline \
                             --enable-dynrec \
                             --enable-unaligned_memory \
                             --with-sdl-prefix=${SYSROOT_PREFIX}/usr \
                             --host=$ARCH"
cd ${PKG_BUILD}
./autogen.sh
}

post_makeinstall_target() {
  mkdir -p ${INSTALL}/usr/config/dosbox
  cp -a ${PKG_DIR}/scripts/* ${INSTALL}/usr/bin/
  cp -a ${PKG_DIR}/config/*  ${INSTALL}/usr/config/dosbox/
}
