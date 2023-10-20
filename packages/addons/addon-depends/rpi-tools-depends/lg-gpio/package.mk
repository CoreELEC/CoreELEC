# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="lg-gpio"
PKG_VERSION="0.2.2"
#PKG_SHA256="cd61c4b03c37b62bba4a5acfea9862749c33c618e0295e7e90aa4713fb373b70"
PKG_LICENSE="Unlicense"
PKG_SITE="http://abyz.me.uk/lg/"
PKG_URL="https://files.pythonhosted.org/packages/source/${PKG_NAME:0:1}/${PKG_NAME}/${PKG_NAME}-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain Python3 swig:host distutilscross:host"
PKG_LONGDESC="A library for Linux Single Board Computers (SBC) which allows control of the General Purpose Input Outputs (GPIO)"
PKG_TOOLCHAIN="manual"

pre_configure_target() {
  export PYTHONXCPREFIX="${SYSROOT_PREFIX}/usr"
  export CFLAGS="${CFLAGS} -I${PKG_BUILD}"
  export LDFLAGS="${CFLAGS} -L${PKG_BUILD}"
  export LDSHARED="${CC} -shared"
}

make_target() {
  make liblgpio.so CROSS_PREFIX=${TARGET_KERNEL_PREFIX}
  (
    cd PY_LGPIO
    swig -python lgpio.i
    python setup.py build
  )
}

