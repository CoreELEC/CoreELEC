# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="bitstream"
PKG_VERSION="1.6"
PKG_SHA256="dea67a9dca7eda0d72017359c8d649bd5a9d249f9f9a691b8daf739d16798029"
PKG_LICENSE="MIT"
PKG_SITE="http://www.videolan.org"
PKG_URL="http://download.videolan.org/pub/videolan/bitstream/${PKG_VERSION}/${PKG_NAME}-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="biTStream is a set of C headers allowing a simpler access to binary structures such as specified by MPEG, DVB, IETF."

PKG_MAKEINSTALL_OPTS_TARGET="PREFIX=/usr"
