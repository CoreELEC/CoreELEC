# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libxshmfence"
PKG_VERSION="1.3.3"
PKG_SHA256="d4a4df096aba96fea02c029ee3a44e11a47eb7f7213c1a729be83e85ec3fde10"
PKG_LICENSE="HPND-sell-variant"
PKG_SITE="https://www.X.org"
PKG_URL="https://xorg.freedesktop.org/archive/individual/lib/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain util-macros xorgproto"
PKG_LONGDESC="libxshmfence is the Shared memory 'SyncFence' synchronization primitive."
PKG_TOOLCHAIN="autotools"
PKG_BUILD_FLAGS="+pic"
