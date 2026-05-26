# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2025-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="plymouth-lite"
PKG_VERSION="d41ce34de554a1cae3250e6a945822ed12717eb5"
PKG_SHA256="ee914d57ac8e8c9b4ab238dc3dd1d7e461b62896408e55b8ac737b5edaa8eaca"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://github.com/sailfishos/plymouth-lite"
PKG_URL="https://github.com/sailfishos/plymouth-lite/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_INIT="toolchain gcc:init libpng"
PKG_LONGDESC="Boot splash screen based on Fedora's Plymouth code"

pre_configure_init() {
  # plymouth-lite dont support to build in subdirs
  cd ${PKG_BUILD}
    rm -rf .${TARGET_NAME}-init
}

makeinstall_init() {
  mkdir -p ${INSTALL}/usr/bin
    cp ply-image ${INSTALL}/usr/bin

  mkdir -p ${INSTALL}/splash
    find_file_path splash/splash.conf && cp ${FOUND_PATH} ${INSTALL}/splash
    find_file_path "splash/splash-*.png" && cp ${FOUND_PATH} ${INSTALL}/splash
}
