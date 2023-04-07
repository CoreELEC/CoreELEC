# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Team CoreELEC (https://coreelec.org)

PKG_NAME="ceemmc"
PKG_LICENSE="proprietary"
PKG_SITE="https://coreelec.org"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Tool to install CoreELEC on internal eMMC"
PKG_TOOLCHAIN="manual"
PKG_VERSION="3143cf2ab3dec9f47f822a3c5adbaf8640bc65da"

case "${ARCH}" in
  arm)
    PKG_SHA256="493a809c95c3021c87597fb77ea9e9c39d174b5d5c4f81c40810ceebc71ce1ba"
    ;;
  aarch64)
    PKG_SHA256="0665182ad9a5ab2de330fc55559c7b1dd23554000507d7a532b5b000ba2cbac1"
    ;;
esac

PKG_SOURCE_NAME="${PKG_NAME}-${ARCH}-${PKG_VERSION}.tar.xz"
PKG_URL="https://sources.coreelec.org/${PKG_SOURCE_NAME}"

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/sbin
    install -m 0755 ceemmc ${INSTALL}/usr/sbin/ceemmc
}
