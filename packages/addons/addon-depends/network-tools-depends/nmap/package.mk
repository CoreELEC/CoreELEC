# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="nmap"
PKG_VERSION="7.991"
PKG_SHA256="a5d507f29437bef3bedd4771ff9aaa8fc1c2a109ddba1f5b1cf12027456929be"
PKG_LICENSE="LicenseRef-Nmap"
PKG_SITE="https://nmap.org/"
PKG_URL="https://nmap.org/dist/${PKG_NAME}-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_TARGET="toolchain openssl dbus libnl"
PKG_LONGDESC="Free Security Scanned for Network."
PKG_BUILD_FLAGS="-sysroot"

PKG_CONFIGURE_OPTS_TARGET="--enable-static \
                           --with-pcap=linux \
                           --with-libpcap=included \
                           --with-libpcre=included \
                           --with-libdnet=included \
                           --with-liblua=included \
                           --with-liblinear=included \
                           --with-openssl=${SYSROOT_PREFIX} \
                           --with-ndiff=no \
                           --with-zenmap=no"

pre_configure_target() {
  # nmap fails to build in subdirs
  cd ${PKG_BUILD}
    rm -rf .${TARGET_NAME}

  export CPPFLAGS="${CPPFLAGS} -Iliblua"
  export LIBS="${LIBS} -ldbus-1 -lnl-3 -lnl-genl-3"
}
