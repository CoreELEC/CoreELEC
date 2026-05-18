# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libusb-compat"
PKG_VERSION="0.1.9"
PKG_SHA256="179d934676ad3bd172a5a37cf5c6351b8e9d7816feb3029252e79b30a518a3be"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/libusb/libusb-compat-0.1"
PKG_URL="https://github.com/libusb/libusb-compat-0.1/releases/download/v${PKG_VERSION}/${PKG_NAME}-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_TARGET="toolchain libusb"
PKG_LONGDESC="The libusb project's aim is to create a Library for use by user level applications to USB devices."
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_TARGET="--disable-log \
                           --disable-debug-log \
                           --disable-examples-build"

post_configure_target() {
  libtool_remove_rpath libtool
}

post_makeinstall_target() {
  rm -rf ${INSTALL}/usr/bin
  sed -e "s:\(['= ]\)/usr:\\1${SYSROOT_PREFIX}/usr:g" -i ${SYSROOT_PREFIX}/usr/bin/libusb-config
}
