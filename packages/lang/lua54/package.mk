# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="lua54"
PKG_VERSION="5.4.8"
PKG_SHA256="4f18ddae154e793e46eeab727c59ef1c0c0c2b744e7b94219710d76f530629ae"
PKG_LICENSE="MIT"
PKG_SITE="https://www.lua.org"
PKG_URL="http://www.lua.org/ftp/lua-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Lua is a powerful, efficient, lightweight, embeddable scripting language."
PKG_BUILD_FLAGS="+pic"

make_target() {
  make CC=${CC} AR="${AR} rcu" MYCFLAGS="-fPIC" posix
}

makeinstall_target() {
  mkdir -p ${SYSROOT_PREFIX}/usr/include/lua$(get_pkg_version_maj_min)
  cp src/lua.h src/luaconf.h src/lualib.h src/lauxlib.h ${SYSROOT_PREFIX}/usr/include/lua$(get_pkg_version_maj_min)

  mkdir -p ${SYSROOT_PREFIX}/usr/lib
  cp src/liblua.a ${SYSROOT_PREFIX}/usr/lib/liblua$(get_pkg_version_maj_min).a

  mkdir -p ${SYSROOT_PREFIX}/usr/lib/pkgconfig
  cp ${PKG_DIR}/config/lua54.pc ${SYSROOT_PREFIX}/usr/lib/pkgconfig
  sed -e "s/@@VERSION@@/${PKG_VERSION}/g" \
      -e "s/@@VERSION_MM@@/$(get_pkg_version_maj_min)/g" \
      -i ${SYSROOT_PREFIX}/usr/lib/pkgconfig/lua54.pc
}
