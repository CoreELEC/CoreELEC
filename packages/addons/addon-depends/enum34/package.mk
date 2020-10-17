# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="enum34"
PKG_VERSION="4cfedc426c4e2fc52e3f5c2b4297e15ed8d6b8c7"
PKG_SHA256="4fe3f5640686512dfa125dca7f8201a0318b6739fe3e9a516c37d9d7740af566"
PKG_LICENSE="BSD"
PKG_SITE="https://github.com/PeachPy/enum34.git"
PKG_URL="https://github.com/PeachPy/enum34/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain Python2 distutilscross:host"
PKG_LONGDESC="Python 3.4 Enum backported to 3.3, 3.2, 3.1, 2.7, 2.6, 2.5, and 2.4."
PKG_TOOLCHAIN="manual"

make_target() {
  python setup.py build
}
