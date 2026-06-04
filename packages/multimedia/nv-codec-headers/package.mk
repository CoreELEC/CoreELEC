# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="nv-codec-headers"
PKG_VERSION="13.0.19.0"
PKG_SHA256="86d15d1a7c0ac73a0eafdfc57bebfeba7da8264595bf531cf4d8db1c22940116"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/FFmpeg/nv-codec-headers"
PKG_URL="https://github.com/FFmpeg/nv-codec-headers/archive/n${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="FFmpeg version of headers required to interface with Nvidias codec APIs."

makeinstall_target() {
  make DESTDIR=${SYSROOT_PREFIX} PREFIX=/usr install
}
