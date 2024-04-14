# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="tinc"
PKG_VERSION="4c6a9a9611442f958c3049a566ac4369653978e9"
PKG_SHA256="409829f454fdfe87f7222f131de88006640709ff046e2241bfdb44a49a146059"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPLv2"
PKG_SITE="http://www.tinc-vpn.org/"
PKG_URL="https://github.com/gsliepen/tinc/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain lz4 lzo miniupnpc ncurses openssl readline zlib"
PKG_SECTION="service/system"
PKG_SHORTDESC="tinc: a Virtual Private Network daemon"
PKG_LONGDESC="tinc (${PKG_VERSION}) is a Virtual Private Network (VPN) daemon that uses tunnelling and encryption to create a secure private network between hosts on the Internet. Because the VPN appears to the IP level network code as a normal network device, there is no need to adapt any existing software. This allows VPN sites to share information with each other over the Internet without exposing any information to others."
PKG_BUILD_FLAGS="+pic"

PKG_IS_ADDON="yes"
PKG_ADDON_NAME="tinc"
PKG_ADDON_TYPE="xbmc.service"
PKG_MAINTAINER="Anton Voyl (awiouy)"

PKG_MESON_OPTS_TARGET="-Dminiupnpc=enabled \
                       -Ddocs=disabled \
                       -Dsystemd=disabled \
                       -Dtests=disabled \
                       -Drunstatedir=/run"

addon() {
  mkdir -p ${ADDON_BUILD}/${PKG_ADDON_ID}/bin
  cp ${PKG_INSTALL}/usr/sbin/* \
     ${ADDON_BUILD}/${PKG_ADDON_ID}/bin
}
