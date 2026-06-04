# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="sshfs"
PKG_VERSION="3.7.6"
PKG_SHA256="6a1bcb31450a077e9cb1b7ff158c71de34db697c3c0da6cb362502131e495893"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://github.com/libfuse/sshfs"
PKG_URL="https://github.com/libfuse/sshfs/releases/download/sshfs-${PKG_VERSION}/sshfs-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain fuse3 glib"
PKG_LONGDESC="A filesystem client based on the SSH File Transfer Protocol."
PKG_BUILD_FLAGS="-sysroot"
