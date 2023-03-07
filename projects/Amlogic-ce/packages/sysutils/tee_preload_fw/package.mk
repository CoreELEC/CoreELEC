# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2022-present Team CoreELEC (https://coreelec.org)

PKG_NAME="tee_preload_fw"
PKG_VERSION="0.1"
PKG_SHA256=""
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/CoreELEC"
PKG_URL=""
PKG_DEPENDS_TARGET="opentee_linuxdriver"
PKG_LONGDESC="SECPU FW Loader"
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/lib/ta
    ln -sf /var/lib/teetz ${INSTALL}/usr/lib/teetz

    for soc in ${TEE_SOC}; do
      cp -rP $(get_pkg_directory ${PKG_NAME})/filesystem/${ARCH}/ta/v3.8/dev/${soc} ${INSTALL}/usr/lib/ta
    done

  mkdir -p ${INSTALL}/usr/sbin
    cp ${PKG_DIR}/scripts/trusted-application-setup ${INSTALL}/usr/sbin

  cp -rP $(get_pkg_directory ${PKG_NAME})/filesystem/${ARCH}/usr/. ${INSTALL}/usr
}

post_install() {
  enable_service video-firmware-preload.service
}
