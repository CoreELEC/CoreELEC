# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020 present Team CoreELEC (https://coreelec.org)

PKG_NAME="splash-image"
PKG_VERSION="87fc0f2bba09f51a7669d5ec1f62bbc4f2673c5b"
PKG_SHA256=""
PKG_LICENSE="GPL"
PKG_SITE="https://coreelec.org"
PKG_URL="https://github.com/CoreELEC/splash-image/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_INIT="toolchain gcc:init glibc libspng zlib"
PKG_LONGDESC="Boot splash screen supporting animation by single RGBA png files"

makeinstall_init() {
  mkdir -p $INSTALL/usr/bin
    cp splash-image $INSTALL/usr/bin

  mkdir -p $INSTALL/splash/progress
    find_file_path "splash/${DEVICE}/splash-*.png" && cp ${FOUND_PATH} $INSTALL/splash || :
    find_file_path "splash/${DEVICE}/progress/splash-*" && cp ${FOUND_PATH} $INSTALL/splash/progress || :
}
