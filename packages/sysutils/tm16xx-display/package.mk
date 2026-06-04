# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2025-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="tm16xx-display"
PKG_VERSION="e990a0cc2377fb957973dd406db6ab6fdf9c2a5a" #v4-feedback
PKG_SHA256="2b9c53f87641c7498d384d97b4e3605eda8a6a0b76e2b6ad4c916551829e6207"
PKG_LICENSE="GPL-2.0-only"
PKG_SITE="https://github.com/jefflessard/tm16xx-display/"
PKG_URL="https://github.com/jefflessard/tm16xx-display/archive/${PKG_VERSION}.tar.gz"
PKG_LONGDESC="Linux kernel driver utilities for TM16XX compatible controllers"
PKG_IS_KERNEL_PKG="yes"
PKG_TOOLCHAIN="manual"

post_makeinstall_target() {
  mkdir -p ${INSTALL}/usr/sbin
    cp display-service ${INSTALL}/usr/sbin
  mkdir -p ${INSTALL}/usr/lib/systemd/system
    cp display.service ${INSTALL}/usr/lib/systemd/system
}

post_install() {
  enable_service display.service
}
