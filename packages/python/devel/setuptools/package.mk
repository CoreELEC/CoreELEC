# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="setuptools"
PKG_VERSION="44.0.0"
PKG_SHA256="e5baf7723e5bb8382fc146e33032b241efc63314211a3a120aaa55d62d2bb008"
PKG_LICENSE="OSS"
PKG_SITE="https://pypi.org/project/setuptools"
PKG_URL="https://files.pythonhosted.org/packages/b0/f3/44da7482ac6da3f36f68e253cb04de37365b3dba9036a3c70773b778b485/setuptools-44.0.0.zip"
PKG_DEPENDS_HOST="Python2:host Python3:host"
PKG_LONGDESC="Replaces Setuptools as the standard method for working with Python module distributions."
PKG_TOOLCHAIN="manual"

make_host() {
  python3 bootstrap.py build
  python2 bootstrap.py build
}

makeinstall_host() {
  exec_thread_safe python3 setup.py install --prefix=$TOOLCHAIN
  exec_thread_safe python2 setup.py install --prefix=$TOOLCHAIN
}

pre_make_target() {
  export PYTHONXCPREFIX="$SYSROOT_PREFIX/usr"
  export LDFLAGS="$LDFLAGS -L$SYSROOT_PREFIX/usr/lib -L$SYSROOT_PREFIX/lib"
  export LDSHARED="$CC -shared"
}

make_target() {
  python3 bootstrap.py build
  python2 bootstrap.py build build --cross-compile
}

makeinstall_target() {
  python3 setup.py install --root=$INSTALL --prefix=/usr
  python2 setup.py install --root=$INSTALL --prefix=/usr
}
