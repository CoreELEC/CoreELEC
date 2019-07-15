# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present 5schatten (https://github.com/5schatten)

PKG_NAME="mupen64plus-rsp-hle"
PKG_VERSION="2.5.9"
PKG_SHA256="9a47fd48e499ced84ce5578e0746022074bc298c3dc943307481226c65e4bd02"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/mupen64plus/mupen64plus-rsp-hle"
PKG_URL="https://github.com/mupen64plus/mupen64plus-rsp-hle/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain linux glibc SDL2-git mupen64plus-core"
PKG_LONGDESC="RSP processor plugin for the Mupen64Plus v2.0 project. This plugin is based on the Mupen64 HLE RSP plugin v0.2 with Azimers code by Hacktarux"
PKG_TOOLCHAIN="make"

PKG_MAKE_OPTS_TARGET="-f projects/unix/Makefile SRCDIR=src APIDIR=$(get_build_dir mupen64plus-core)/src/api all"

pre_configure_target() {
  export SYSROOT_PREFIX=$SYSROOT_PREFIX

  # ARCH arm
  if [ "${ARCH}" = "arm" ]; then
    PKG_MAKE_OPTS_TARGET+=" DYNAREC=arm HOST_CPU=arm"
  fi
}

makeinstall_target() {
 :
}
