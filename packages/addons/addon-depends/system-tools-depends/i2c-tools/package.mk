# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="i2c-tools"
PKG_VERSION="4.4"
PKG_SHA256="8b15f0a880ab87280c40cfd7235cfff28134bf14d5646c07518b1ff6642a2473"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://i2c.wiki.kernel.org/index.php/I2C_Tools"
PKG_URL="https://www.kernel.org/pub/software/utils/i2c-tools/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain Python3 setuptools:host"
PKG_LONGDESC="A heterogeneous set of I2C tools for Linux."
PKG_BUILD_FLAGS="-sysroot"

make_target() {
  make  CC="${CC}" \
        AR="${AR}" \
        CFLAGS="${TARGET_CFLAGS}" \
        CPPFLAGS="${TARGET_CPPFLAGS}" \
        all

  (
    cd py-smbus
    python_target_env python3 -m build -n -w -x
  )
}

makeinstall_target() {
  make  DESTDIR=${INSTALL} \
        PREFIX="/usr" \
        prefix="/usr" \
        install
  (
    cd py-smbus
    python3 -m installer --overwrite-existing dist/*.whl -d ${INSTALL} -p /usr
  )
}
