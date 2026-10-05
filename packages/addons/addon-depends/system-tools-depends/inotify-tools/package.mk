# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="inotify-tools"
PKG_VERSION="4.26.268"
PKG_SHA256="b59c1dd7546826b58459affb6e28ef386eb65b4b05d41435646de8417c02bb90"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://github.com/inotify-tools/inotify-tools"
PKG_URL="https://github.com/inotify-tools/inotify-tools/releases/download/${PKG_VERSION}/${PKG_NAME}-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain cargo:host"
PKG_LONGDESC="A library and a set of command-line programs for Linux providing a simple interface to inotify."
PKG_TOOLCHAIN="manual"

make_target() {
  cargo build \
    --target ${TARGET_NAME} \
    --release \
    --locked \
    --package inotify-tools \
    --bins
}

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/bin
  cp ${PKG_BUILD}/.${TARGET_NAME}/target/${TARGET_NAME}/release/{inotifywait,inotifywatch} ${INSTALL}/usr/bin
}
