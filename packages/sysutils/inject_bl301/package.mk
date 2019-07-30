# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Team CoreELEC (https://coreelec.org)

PKG_NAME="inject_bl301"
PKG_VERSION="v0.1"
PKG_LICENSE="GPLv2"
PKG_TOOLCHAIN="manual"
PKG_LONGDESC="Tool to inject bootloader blob BL301.bin on internal eMMC"

makeinstall_target() {
  mkdir -p $INSTALL/usr/sbin
	cd ${PKG_BUILD}
	mv ${PKG_NAME} $INSTALL/usr/sbin/${PKG_NAME}
}
