# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libnl"
PKG_VERSION="3.12.0"
PKG_SHA256="fc51ca7196f1a3f5fdf6ffd3864b50f4f9c02333be28be4eeca057e103c0dd18"
PKG_LICENSE="LGPL-2.1-only"
PKG_SITE="https://github.com/thom311/libnl"
PKG_URL="https://github.com/thom311/libnl/releases/download/libnl${PKG_VERSION//./_}/libnl-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="A library for applications dealing with netlink socket."
PKG_BUILD_FLAGS="+pic"

PKG_CONFIGURE_OPTS_TARGET="--enable-static \
                           --disable-shared \
                           --disable-cli"
