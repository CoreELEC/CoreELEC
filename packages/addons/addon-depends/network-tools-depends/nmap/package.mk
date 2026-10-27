# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="nmap"
PKG_VERSION="7.993"
PKG_SHA256="270e55f04915bfcecb5db8feb959d4c5ba86a48c094520a4f1c07571e1c79c59"
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
