# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present 5schatten (https://github.com/5schatten)

PKG_NAME="mupen64plus-rsp-cxd4"
PKG_VERSION="87c1c9a89730c0522cb66380adcb8b39075dda4d"
PKG_SHA256="49de602b33428fffce20af7d1320d370c3a529251961239a9768e8bdc7489fd6"
PKG_LICENSE="CC0-1.0"
PKG_SITE="https://github.com/mupen64plus/mupen64plus-rsp-cxd4"
PKG_URL="https://github.com/mupen64plus/mupen64plus-rsp-cxd4/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain linux glibc SDL2-git mupen64plus-core"
PKG_LONGDESC="Exemplary MSP communications simulator using a normalized VU."
PKG_TOOLCHAIN="make"

PKG_MAKE_OPTS_TARGET="-f projects/unix/Makefile SRCDIR=. APIDIR=$(get_build_dir mupen64plus-core)/src/api all HLEVIDEO=1"

pre_configure_target() {
  export SYSROOT_PREFIX=$SYSROOT_PREFIX

  # ARCH arm
  if [ "${ARCH}" = "arm" ]; then
    PKG_MAKE_OPTS_TARGET+=" HOST_CPU=arm"
  fi
}

makeinstall_target() {
 :
}
