# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="mtools"
PKG_VERSION="4.0.49"
PKG_SHA256="6fe5193583d6e7c59da75e63d7234f76c0b07caf33b103894f46f66a871ffc9f"
PKG_LICENSE="GPL-3.0-or-later"
PKG_SITE="http://www.gnu.org/software/mtools/"
PKG_URL="https://ftpmirror.gnu.org/${PKG_NAME}/${PKG_NAME}-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_HOST="autotools:host"
PKG_LONGDESC="mtools: A collection of utilities to access MS-DOS disks"
PKG_TOOLCHAIN="autotools"
PKG_BUILD_FLAGS="-cfg-libs:host"
