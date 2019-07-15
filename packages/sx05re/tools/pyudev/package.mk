# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 0riginally created by Escalade (https://github.com/escalade)
# Copyright (C) 2018-present 5schatten (https://github.com/5schatten)

PKG_NAME="pyudev"
PKG_VERSION="0.21.0"
PKG_SHA256="5f4625f89347e465731866ddbe042a055bbc5092577356aa3d089ac5fb8efd94"
PKG_LICENSE="OSS"
PKG_SITE="https://github.com/pyudev/pyudev"
PKG_URL="https://github.com/pyudev/pyudev/archive/v$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain Python2 distutilscross:host"
PKG_LONGDESC="pyudev is a LGPL licenced, pure Python 2/3 binding to libudev, the device and hardware management and information library of Linux."

pre_make_target() {
  export PYTHONXCPREFIX="$SYSROOT_PREFIX/usr"
  export LDFLAGS="$LDFLAGS -L$SYSROOT_PREFIX/usr/lib -L$SYSROOT_PREFIX/lib"
  export LDSHARED="$CC -shared"
}

make_target() {
  python setup.py build --cross-compile
}

makeinstall_target() {
  python setup.py install --root=$INSTALL --prefix=/usr
}

post_makeinstall_target() {
  find $INSTALL/usr/lib/python*/site-packages/  -name "*.py" -exec rm -rf {} ";"
}
