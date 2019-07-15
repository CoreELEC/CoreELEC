# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present 5schatten (https://github.com/5schatten)

PKG_NAME="mupen64plus-ui-console"
PKG_VERSION="2.5.9"
PKG_SHA256="65a294b94dfb4defe0c2f802c3cd2bff23c8cbb2dae5a76fcf04d2745b5e594f"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/mupen64plus/mupen64plus-ui-console"
PKG_URL="https://github.com/mupen64plus/mupen64plus-ui-console/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain linux glibc SDL2-git mupen64plus-core"
PKG_LONGDESC="Console (command-line) front-end user interface for Mupen64Plus v2.0 project"
PKG_TOOLCHAIN="make"

PKG_MAKE_OPTS_TARGET="-f projects/unix/Makefile \
                      SRCDIR=src \
                      APIDIR=$(get_build_dir mupen64plus-core)/src/api \
                      SHAREDIR=/usr/config/mupen64plus \
                      COREDIR=/usr/lib/mupen64plus/ \
                      PLUGINDIR=/usr/lib/mupen64plus/ \
                      all"

pre_configure_target() {
  export SYSROOT_PREFIX=$SYSROOT_PREFIX
}

makeinstall_target() {
 :
}
