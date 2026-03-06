# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="dbussy"
PKG_VERSION="bd4a5c3ddd2a59df2c10d84cfa7902102b68f050"
PKG_SHA256=""
PKG_LICENSE="LGPLv2.1+"
PKG_SITE="https://gitlab.com/ldo/dbussy"
PKG_URL="https://gitlab.com/ldo/${PKG_NAME}/-/archive/${PKG_VERSION}/${PKG_NAME}-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain Python3 dbus setuptools:host"
PKG_LONGDESC="DBussy is a wrapper around libdbus, written in pure Python"
PKG_TOOLCHAIN="python"

post_makeinstall_target() {
  python_remove_source
}
