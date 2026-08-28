# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="lua54"
PKG_VERSION="5.4.9"
PKG_SHA256="2335b6c582a52654f94612bf10d2f4672805d05329aa6568b1d8cd9e5c6fb8e6"
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
