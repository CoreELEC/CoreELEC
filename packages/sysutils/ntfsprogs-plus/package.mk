# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2025-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="ntfsprogs-plus"
PKG_VERSION="53943daef955cc2a4a405c8e37c62478e4272fec"
PKG_SHA256="44b35d37c74259bd88c94a0cc21fc8b0560bc938eb0f400a9bc74b8e317b17d0"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://github.com/ntfsprogs-plus/ntfsprogs-plus"
PKG_URL="https://github.com/ntfsprogs-plus/ntfsprogs-plus/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libgcrypt"
PKG_LONGDESC="NTFS filesystem userspace utilities"
PKG_TOOLCHAIN="autotools"

post_makeinstall_target() {
  rm ${INSTALL}/usr/lib/libntfs.so
  mv ${INSTALL}/lib/libntfs.so* ${INSTALL}/usr/lib/
  rmdir ${INSTALL}/lib
}
