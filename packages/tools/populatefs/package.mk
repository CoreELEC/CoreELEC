# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="populatefs"
PKG_VERSION="fa7279f8e6afd4ab82c153ba048d09ee6e156fcf"
PKG_SHA256="bfcdd86bdeea0eebbaacbae4651ed40554e0e2067cd6b4cc94a741e1ae5c23e0"
PKG_LICENSE="GPL-2.0-only"
PKG_SITE="https://github.com/kfix/populatefs"
PKG_URL="https://github.com/kfix/${PKG_NAME}/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="e2fsprogs:host"
PKG_LONGDESC="populatefs: Tool for replacing genext2fs when creating ext4 images"
PKG_BUILD_FLAGS="+pic:host"

make_host() {
  make EXTRA_LIBS="-lcom_err -lpthread"
}

makeinstall_host() {
  ${STRIP} src/populatefs

  mkdir -p ${TOOLCHAIN}/sbin
  cp src/populatefs ${TOOLCHAIN}/sbin
}
