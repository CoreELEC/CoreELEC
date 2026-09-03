# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2025-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="nftables"
PKG_VERSION="1.1.7"
PKG_SHA256="a6fbf060d8d4fff001517a2b94f356bb4366bfbf0ba366366f9d27cc38caa58f"
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
