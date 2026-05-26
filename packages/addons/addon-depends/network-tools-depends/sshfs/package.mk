# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="sshfs"
PKG_VERSION="3.7.5"
PKG_SHA256="0e45db63c2d00919db3174134fa234c6e0682d6fe573c46312d1d53d1d61a8bb"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://github.com/libfuse/sshfs"
PKG_URL="https://github.com/libfuse/sshfs/releases/download/sshfs-${PKG_VERSION}/sshfs-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain fuse3 glib"
PKG_LONGDESC="A filesystem client based on the SSH File Transfer Protocol."
PKG_BUILD_FLAGS="-sysroot"
