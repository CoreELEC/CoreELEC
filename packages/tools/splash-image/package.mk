# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020 present Team CoreELEC (https://coreelec.org)

PKG_NAME="splash-image"
PKG_VERSION="76925fc7781dcffa5a4517f251ebd004816a3de6"
PKG_SHA256="7e972aada5f5aa1f12e4add0757970c9dfddc53b01a209ce38581af791b7e75a"
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
