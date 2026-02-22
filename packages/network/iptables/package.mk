# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="iptables"
PKG_VERSION="1.8.12"
PKG_SHA256="8e7ee962601492de6503d171d4a948092ab18f89f111de72e3037c1f40cfb846"
PKG_LICENSE="GPL"
PKG_SITE="https://www.netfilter.org/"
PKG_URL="https://www.netfilter.org/projects/iptables/files/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="autotools:host gcc:host linux:host libmnl libnftnl"
PKG_LONGDESC="IP packet filter administration."
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_TARGET+=" --enable-nftables --disable-ipv4 --disable-ipv6"

post_configure_target() {
  libtool_remove_rpath libtool
}

post_makeinstall_target() {
  mkdir -p ${INSTALL}/usr/config/iptables/
    cp -PR ${PKG_DIR}/config/README ${INSTALL}/usr/config/iptables/

  mkdir -p ${INSTALL}/etc/iptables/
    cp -PR ${PKG_DIR}/config/* ${INSTALL}/etc/iptables/

  mkdir -p ${INSTALL}/usr/lib/libreelec
    cp ${PKG_DIR}/scripts/iptables_helper ${INSTALL}/usr/lib/libreelec

  ln -sf xtables-nft-multi ${INSTALL}/usr/sbin/iptables
  ln -sf xtables-nft-multi ${INSTALL}/usr/sbin/iptables-restore
  ln -sf xtables-nft-multi ${INSTALL}/usr/sbin/iptables-save
  ln -sf xtables-nft-multi ${INSTALL}/usr/sbin/ip6tables
  ln -sf xtables-nft-multi ${INSTALL}/usr/sbin/ip6tables-restore
  ln -sf xtables-nft-multi ${INSTALL}/usr/sbin/ip6tables-save
}

post_install() {
  enable_service iptables.service
}
