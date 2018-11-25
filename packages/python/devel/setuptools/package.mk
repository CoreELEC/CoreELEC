# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)

PKG_NAME="setuptools"
PKG_VERSION="39.2.0"
PKG_SHA256="ca8119dd5c2764a7d290518817de0b880d23d790913fcd797c02ad2aa39b8c41"
PKG_LICENSE="OSS"
PKG_SITE="https://pypi.org/project/setuptools"
PKG_URL="https://github.com/pypa/setuptools/archive/v$PKG_VERSION.tar.gz"
PKG_DEPENDS_HOST="Python2:host"
PKG_LONGDESC="Replaces Setuptools as the standard method for working with Python module distributions."
PKG_TOOLCHAIN="manual"

make_host() {
  python bootstrap.py
}

makeinstall_host() {
  python setup.py install --prefix=$TOOLCHAIN
}
