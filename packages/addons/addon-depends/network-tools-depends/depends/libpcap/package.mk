# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libpcap"
PKG_VERSION="1.7.4"
PKG_SHA256="7ad3112187e88328b85e46dce7a9b949632af18ee74d97ffc3f2b41fe7f448b0"
PKG_LICENSE="GPL"
PKG_SITE="http://www.tcpdump.org/"
PKG_URL="http://www.tcpdump.org/release/libpcap-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="A portable framework for low-level network monitoring."

PKG_CONFIGURE_OPTS_TARGET="LIBS=-lpthread \
                           ac_cv_header_libusb_1_0_libusb_h=no \
                           --disable-shared \
                           --with-pcap=linux \
                           --disable-bluetooth \
                           --disable-can \
                           --without-libnl \
                           --disable-dbus \
                           --disable-canusb"

pre_configure_target() {
# When cross-compiling, configure can't set linux version
# forcing it
  sed -i -e 's/ac_cv_linux_vers=unknown/ac_cv_linux_vers=2/' ../configure
}

post_makeinstall_target() {
  rm -rf $INSTALL/usr/bin
}
