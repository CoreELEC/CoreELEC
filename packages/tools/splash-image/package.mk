# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020 present Team CoreELEC (https://coreelec.org)

PKG_NAME="splash-image"
PKG_VERSION="2807b667817208f803e5fd8ba96c65703faaf6ae"
PKG_SHA256="f2a5d0c7d4d8766fb0deb26864eb43058a70f05a9a31f992dde77b1a5bf3c76c"
PKG_LICENSE="GPL"
PKG_SITE="https://coreelec.org"
PKG_URL="https://github.com/CoreELEC/splash-image/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_INIT="toolchain gcc:init glibc libspng zlib"
PKG_LONGDESC="Boot splash screen supporting animation by single RGBA png files"

makeinstall_init() {
  mkdir -p $INSTALL/usr/bin
    cp splash-image $INSTALL/usr/bin

  mkdir -p $INSTALL/splash/progress
    find_file_path "splash/splash-*.png" && cp ${FOUND_PATH} $INSTALL/splash || :
    find_file_path "splash/progress/splash-*" && cp ${FOUND_PATH} $INSTALL/splash/progress || :
}
