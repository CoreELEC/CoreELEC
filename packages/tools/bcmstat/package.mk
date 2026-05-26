# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="bcmstat"
PKG_VERSION="7880c777d6a2f631a9d017a371911c79b874744f"
PKG_SHA256="7b0230e87299f40f6f52aeedff9c2a1761bab1d99cb9c958aa5aa476641b9ce8"
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
