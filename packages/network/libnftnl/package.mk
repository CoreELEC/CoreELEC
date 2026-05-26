# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libnftnl"
PKG_VERSION="1.3.1"
PKG_SHA256="607da28dba66fbdeccf8ef1395dded9077e8d19f2995f9a4d45a9c2f0bcffba8"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://netfilter.org/projects/libnftnl"
PKG_URL="https://netfilter.org/projects/libnftnl/files/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="autotools:host gcc:host libmnl"
PKG_LONGDESC="A userspace library providing a low-level netlink programming interface (API) to the in-kernel nf_tables subsystem."
