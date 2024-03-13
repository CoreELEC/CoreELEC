# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pcsc-lite"
PKG_VERSION="2.0.3"
PKG_SHA256="f42ee9efa489e9ff5d328baefa26f9c515be65021856e78d99ad1f0ead9ec85d"
PKG_LICENSE="GPL"
PKG_SITE="https://pcsclite.apdu.fr"
PKG_URL="https://pcsclite.apdu.fr/files/pcsc-lite-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_TARGET="toolchain libusb polkit"
PKG_LONGDESC="Middleware to access a smart card using SCard API (PC/SC)."

PKG_CONFIGURE_OPTS_TARGET="--disable-shared \
            --enable-static \
            --disable-libudev \
            --enable-libusb \
            --enable-usbdropdir=/storage/.kodi/addons/service.pcscd/drivers"

pre_configure_target() {
  export PKG_CONFIG_PATH="$(get_install_dir polkit)/usr/lib/pkgconfig:${PKG_CONFIG_PATH}"
}

post_configure_target() {
  libtool_remove_rpath libtool
}
