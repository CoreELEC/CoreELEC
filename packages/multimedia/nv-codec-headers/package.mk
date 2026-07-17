# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="nv-codec-headers"
PKG_VERSION="13.1.15.0"
PKG_SHA256="2255bc74d038b95aa4be30f5f66322c2176acbdb90ada1851db6993536fbeaf7"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/FFmpeg/nv-codec-headers"
PKG_URL="https://github.com/FFmpeg/nv-codec-headers/archive/n${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="FFmpeg version of headers required to interface with Nvidias codec APIs."

makeinstall_target() {
  make DESTDIR=${SYSROOT_PREFIX} PREFIX=/usr install
}
