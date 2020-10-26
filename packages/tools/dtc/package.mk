# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="dtc"
PKG_VERSION="1.5.0"
PKG_SHA256="14343cb204aaff386206ea27e39e93d6e35d9a797222e8426f95e57828ca6b94"
PKG_LICENSE="GPL"
PKG_SITE="https://git.kernel.org/pub/scm/utils/dtc/dtc.git/"
PKG_URL="https://git.kernel.org/pub/scm/utils/dtc/dtc.git/snapshot/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain zlib"
PKG_LONGDESC="The Device Tree Compiler"

PKG_MAKE_OPTS_TARGET="dtc fdtput fdtget libfdt"

makeinstall_target() {
  mkdir -p $INSTALL/usr/bin
    cp -P $PKG_BUILD/dtc $INSTALL/usr/bin
    cp -P $PKG_BUILD/fdtput $INSTALL/usr/bin/
    cp -P $PKG_BUILD/fdtget $INSTALL/usr/bin/
  mkdir -p $INSTALL/usr/lib
    cp -P $PKG_BUILD/libfdt/libfdt.a $INSTALL/usr/lib/

  # copy to toolchain
  mkdir -p $SYSROOT_PREFIX/usr/{include,lib}
    cp -P $PKG_BUILD/libfdt/libfdt.a $SYSROOT_PREFIX/usr/lib
    cp -P $PKG_BUILD/libfdt/fdt.h $SYSROOT_PREFIX/usr/include
    cp -P $PKG_BUILD/libfdt/libfdt.h $SYSROOT_PREFIX/usr/include
    cp -P $PKG_BUILD/libfdt/libfdt_env.h $SYSROOT_PREFIX/usr/include
}
