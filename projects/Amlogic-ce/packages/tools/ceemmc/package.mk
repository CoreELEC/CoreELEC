# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Team CoreELEC (https://coreelec.org)

PKG_NAME="ceemmc"
PKG_VERSION="d8058180fef88a8391f1f3410954922ee60bfa32"
PKG_SHA256="5d2826f77eda075d4a24d11dc64111378615aa15baf651638ac638031cebe0ae"
PKG_LICENSE="proprietary"
PKG_SITE="https://coreelec.org"
PKG_SOURCE_NAME="ceemmc-aarch64-${PKG_VERSION}.tar.xz"
PKG_URL="https://sources.coreelec.org/${PKG_SOURCE_NAME}"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Tool to install CoreELEC on internal eMMC"
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/sbin
    install -m 0755 ceemmc ${INSTALL}/usr/sbin/ceemmc
}
