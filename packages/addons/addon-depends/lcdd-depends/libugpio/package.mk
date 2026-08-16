# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libugpio"
PKG_VERSION="0.0.8"
PKG_SHA256="d69fa71b5f51a9296b5e4f95f03c0f191b056a7aac565db951c420e2276bbf94"
PKG_LICENSE="LGPL-2.1-or-later"
PKG_SITE="https://github.com/mhei/libugpio"
PKG_URL="https://github.com/mhei/${PKG_NAME}/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain linux:host"
PKG_LONGDESC="A software library to ease the use of linux kernel's sysfs gpio interface from C programs and/or other libraries."
PKG_TOOLCHAIN="autotools"
PKG_BUILD_FLAGS="+pic"

PKG_CONFIGURE_OPTS_TARGET="--enable-static --disable-shared"
