# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="bcmstat"
PKG_VERSION="32d067b86fad8214f8b9a87a16241a89974dfb6a"
PKG_SHA256="8c40097e12aeabd3d57f20a3e8d2a1402ed338867bfa0a9e1a83fc225886c880"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://github.com/popcornmix/bcmstat"
PKG_URL="https://github.com/popcornmix/${PKG_NAME}/archive/${PKG_VERSION}.tar.gz"
PKG_LONGDESC="Raspberry Pi monitoring script"
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/bin
    cp -PRv bcmstat.sh ${INSTALL}/usr/bin
    chmod +x ${INSTALL}/usr/bin/*
}
