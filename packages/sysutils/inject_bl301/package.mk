# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Team CoreELEC (https://coreelec.org)

PKG_NAME="inject_bl301"
PKG_VERSION="c495cd23b6930c5bb9d9beb5c64a5d389900ec2c"
PKG_SHA256="77f867b6317b5ec24298e592f048fe94529f41d4c62d39e7db4838f35540f178"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/CoreELEC/inject_bl301"
PKG_URL="https://github.com/CoreELEC/inject_bl301/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_HOST="gcc:host openssl"
PKG_DEPENDS_TARGET="toolchain"
PKG_DEPENDS_INIT="toolchain"
PKG_LONGDESC="Tool to inject bootloader blob BL301.bin on internal eMMC"

make_host() {
  cd ${PKG_BUILD}
	make clean
  make
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/sbin
    cp ${PKG_NAME} $INSTALL/usr/sbin/${PKG_NAME}
}
