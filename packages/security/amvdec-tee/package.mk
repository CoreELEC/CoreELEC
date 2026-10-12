# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="amvdec-tee"
PKG_VERSION="1d7e8009cce1692fef04a729e859e8602b8eec9e"
PKG_SHA256="a6915ce0f8b1937798ade4b6d00041282c136e416921a360a0ab6b5028bd9bc7"
PKG_LICENSE="GPL-2.0-only"
PKG_SITE="https://github.com/LibreELEC/amvdec-tee"
PKG_URL="https://github.com/LibreELEC/amvdec-tee/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain optee_client"
PKG_LONGDESC="Secure World (TEE) AMRISC firmware pre-load service"
PKG_TOOLCHAIN="make"

makeinstall_target() {
  make DESTDIR=${INSTALL} install
}

post_install() {
  enable_service amvdec-tee.service
}
