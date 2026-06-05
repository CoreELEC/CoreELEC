# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2025-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="ipset"
PKG_VERSION="7.24"
PKG_SHA256="fbe3424dff222c1cb5e5c34d38b64524b2217ce80226c14fdcbb13b29ea36112"
PKG_LICENSE="GPL-2.0-only"
PKG_SITE="https://ipset.netfilter.org"
PKG_URL="https://ipset.netfilter.org/${PKG_NAME}-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_TARGET="libmnl"
PKG_LONGDESC="IP sets are a framework inside the Linux kernel, which can be administered by the ipset utility"
PKG_IS_KERNEL_PKG="yes"

PKG_CONFIGURE_OPTS_TARGET="--with-kbuild=$(kernel_path)"

post_configure_target() {
  libtool_remove_rpath libtool
}
