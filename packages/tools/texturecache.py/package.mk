# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="texturecache.py"
PKG_VERSION="2.5.7"
PKG_SHA256="94b26eb3bd9e532305a895acd23155e87be4763c612c0404e1f854bf2a4abb11"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://github.com/xbmc/texturecache"
PKG_URL="https://github.com/xbmc/texturecache/archive/${PKG_VERSION}.tar.gz"
PKG_LONGDESC="The Swiss Army knife for Kodi"
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/bin
    cp -PRv texturecache.py ${INSTALL}/usr/bin
    cp -PRv tools/mklocal.py ${INSTALL}/usr/bin
    chmod +x ${INSTALL}/usr/bin/*
}
