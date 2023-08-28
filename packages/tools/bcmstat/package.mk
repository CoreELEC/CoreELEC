# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="bcmstat"
PKG_VERSION="1698ec66f7dd38b21d92958dfe08a79dd07300a5"
PKG_SHA256="6a4c30778abb80c47be984b01cf9a5cfce83bcc7c673303693945c9a0f5c876a"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/popcornmix/bcmstat"
PKG_URL="https://github.com/popcornmix/${PKG_NAME}/archive/${PKG_VERSION}.tar.gz"
PKG_LONGDESC="Raspberry Pi monitoring script"
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/bin
    cp -PRv bcmstat.sh ${INSTALL}/usr/bin
    chmod +x ${INSTALL}/usr/bin/*
}
