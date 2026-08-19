# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="librelp"
PKG_VERSION="1.13.0"
PKG_SHA256="58e976e31795d7309cf2d9f37fdc93522daa4a322f5012d7879fed9ebfa40068"
PKG_LICENSE="GPL-3.0-or-later"
PKG_SITE="https://www.rsyslog.com/category/librelp/"
PKG_URL="https://download.rsyslog.com/librelp/${PKG_NAME}-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Provides reliable event logging over the network."
PKG_TOOLCHAIN="autotools"
PKG_BUILD_FLAGS="+pic"

PKG_CONFIGURE_OPTS_TARGET="--enable-static \
                           --disable-shared \
                           --disable-tls \
                           ac_cv_func_malloc_0_nonnull=yes \
                           ac_cv_func_realloc_0_nonnull=yes"
