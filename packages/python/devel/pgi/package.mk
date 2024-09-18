# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Team CoreELEC (https://coreelec.org)

PKG_NAME="pgi"
PKG_VERSION="d88848581c68543f6d761205e689a2757fd1ffdd"
PKG_SHA256="9db3fcf5215ea19f31bf790c935bda497769cf38fca0ee8a05cb0300b41b8d76"
PKG_ARCH="any"
PKG_LICENSE="LGPL"
PKG_SITE="https://github.com/pygobject/pgi"
PKG_URL="https://github.com/pygobject/pgi/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain Python3"
PKG_SECTION="python/devel"
PKG_SHORTDESC="PGI - Pure Python GObject Introspection Bindings"
PKG_LONGDESC="GObject Introspection bindings written in pure Python using ctypes and cffi (optional). API compatible with PyGObject."
PKG_TOOLCHAIN="manual"

make_target() {
  python_target_env python3 -m build -n -w -x
}

makeinstall_target() {
  python3 -m installer --overwrite-existing dist/*.whl -d ${INSTALL} -p /usr
}

post_makeinstall_target() {
  find ${INSTALL}/usr/lib -name "*.py" -exec rm -rf "{}" ";"
}
