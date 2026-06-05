# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2025-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="nftables"
PKG_VERSION="1.1.6"
PKG_SHA256="372931bda8556b310636a2f9020adc710f9bab66f47efe0ce90bff800ac2530c"
PKG_LICENSE="GPL-2.0-only"
PKG_SITE="https://netfilter.org/projects/${PKG_NAME}"
PKG_URL="https://netfilter.org/projects/${PKG_NAME}/files/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="autotools:host gcc:host libnftnl readline"
PKG_LONGDESC="A userspace library providing a low-level netlink programming interface (API) to the in-kernel nf_tables subsystem."
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_TARGET="--without-cli --with-mini-gmp"

post_configure_target() {
  libtool_remove_rpath libtool
}
