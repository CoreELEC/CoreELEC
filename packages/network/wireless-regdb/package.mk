# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="wireless-regdb"
PKG_VERSION="2024.10.07"
PKG_SHA256="f76f2bd79a653e9f9dd50548d99d03a4a4eb157da056dfd5892f403ec28fb3d5"
PKG_LICENSE="GPL"
PKG_SITE="https://wireless.wiki.kernel.org/en/developers/regulatory/wireless-regdb"
PKG_URL="https://www.kernel.org/pub/software/network/${PKG_NAME}/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_LONGDESC="wireless-regdb is a regulatory database"
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  FW_TARGET_DIR=${INSTALL}/$(get_full_firmware_dir)

  mkdir -p ${FW_TARGET_DIR}
    cp ${PKG_BUILD}/regulatory.db ${PKG_BUILD}/regulatory.db.p7s ${FW_TARGET_DIR}
}
