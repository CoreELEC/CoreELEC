# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present CoreELEC (https://coreelec.org)

PKG_NAME="wetekdvb"
PKG_VERSION="1.0"
PKG_ARCH="arm aarch64"
PKG_LICENSE="GPL"
PKG_SITE=""
PKG_URL=""
PKG_DEPENDS_TARGET="toolchain"
PKG_SECTION="virtual"
PKG_SHORTDESC="wetekdvb: Wetek DVB driver"
PKG_LONGDESC="wetekdvb: Wetek DVB driver"

post_install() {
  enable_service wetekdvb.service
}
