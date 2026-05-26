# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="brcmfmac_sdio-firmware-rpi"
PKG_VERSION="6596d332eaadd63e2413e3937715a084f61c39e2"
PKG_SHA256="d9835b33bea65cf15f41753e215a11e5eca818a6e3bfacaa750aeac12456c24f"
PKG_LICENSE="LicenseRef-firmware-Broadcom"
PKG_SITE="https://github.com/LibreELEC/LibreELEC.tv"
PKG_URL="https://github.com/LibreELEC/${PKG_NAME}/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Firmware for brcm bluetooth chips used on RaspberryPi devices."
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  DESTDIR=${INSTALL}/$(get_kernel_overlay_dir) ./install
}
