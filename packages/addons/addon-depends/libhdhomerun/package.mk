# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libhdhomerun"
PKG_VERSION="20260326"
PKG_SHA256="792d43b98bdc146fa8872f8205f4f8feb1c1d5557e3c77edda6b6b1ede9b9db0"
PKG_LICENSE="LGPL"
PKG_SITE="http://www.silicondust.com"
PKG_URL="https://download.silicondust.com/hdhomerun/libhdhomerun_${PKG_VERSION}.tgz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="The library provides functionality to setup the HDHomeRun."

PKG_MAKE_OPTS_TARGET="CROSS_COMPILE=${TARGET_PREFIX}"

makeinstall_target() {
  mkdir -p ${SYSROOT_PREFIX}/usr/include/hdhomerun
    cp *.h ${SYSROOT_PREFIX}/usr/include/hdhomerun

  mkdir -p ${SYSROOT_PREFIX}/usr/lib
    cp libhdhomerun.a ${SYSROOT_PREFIX}/usr/lib
}
