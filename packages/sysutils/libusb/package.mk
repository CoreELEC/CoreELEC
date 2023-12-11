# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libusb"
PKG_VERSION="1.0.27"
PKG_SHA256="ffaa41d741a8a3bee244ac8e54a72ea05bf2879663c098c82fc5757853441575"
PKG_LICENSE="LGPLv2.1"
PKG_SITE="http://libusb.info/"
PKG_URL="https://github.com/libusb/libusb/releases/download/v${PKG_VERSION}/libusb-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_TARGET="toolchain systemd"
PKG_LONGDESC="The libusb project's aim is to create a Library for use by user level applications to USB devices."

PKG_CONFIGURE_OPTS_TARGET="--enable-shared \
            --enable-static \
            --disable-log \
            --disable-debug-log \
            --enable-udev \
            --disable-examples-build"
