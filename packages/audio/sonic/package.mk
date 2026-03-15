# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="sonic"
PKG_VERSION="b93885dcb70aae50c6f76b0fe4e0868f029a077e"
PKG_SHA256="b76d832649306b53e716c271014c0b8f89f6bcefc43e5068a2eca3cf1946324c"
PKG_LICENSE="Apache-2.0"
PKG_SITE="https://github.com/waywardgeek/sonic"
PKG_URL="https://github.com/waywardgeek/sonic/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="ccache:host"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Simple library to speed up or slow down speech"

pre_build_host() {
  mkdir -p ${PKG_BUILD}/.${HOST_NAME}
  cp -r ${PKG_BUILD}/* ${PKG_BUILD}/.${HOST_NAME}
}

configure_host() {
  cd ${PKG_BUILD}/.${HOST_NAME}
}

makeinstall_host() {
  make install PREFIX="${TOOLCHAIN}"
}

post_makeinstall_host() {
  safe_remove ${TOOLCHAIN}/bin/sonic
  safe_remove ${TOOLCHAIN}/lib/libsonic.so
  safe_remove ${TOOLCHAIN}/lib/libsonic.so.0
  safe_remove ${TOOLCHAIN}/lib/libsonic.so.0.3.0
}

pre_build_target() {
  mkdir -p ${PKG_BUILD}/.${TARGET_NAME}
  cp -r ${PKG_BUILD}/* ${PKG_BUILD}/.${TARGET_NAME}
}

configure_target() {
  cd ${PKG_BUILD}/.${TARGET_NAME}
}

post_makeinstall_target() {
  safe_remove ${INSTALL}/usr/bin/sonic
  safe_remove ${INSTALL}/usr/lib/libsonic.so
  safe_remove ${INSTALL}/usr/lib/libsonic.so.0
  safe_remove ${INSTALL}/usr/lib/libsonic.so.0.3.0
  rm ${SYSROOT_PREFIX}/usr/bin/sonic
  rm ${SYSROOT_PREFIX}/usr/lib/libsonic.so
  rm ${SYSROOT_PREFIX}/usr/lib/libsonic.so.0
  rm ${SYSROOT_PREFIX}/usr/lib/libsonic.so.0.3.0
}
