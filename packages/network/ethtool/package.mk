# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="ethtool"
PKG_VERSION="7.1"
PKG_SHA256="4d78c26edc0255bc92f4b995b5fd66108d75ff966ed4694f6025a6d370bc2496"
PKG_LICENSE="GPL-2.0-only"
PKG_SITE="https://www.kernel.org/pub/software/network/ethtool/"
PKG_URL="https://www.kernel.org/pub/software/network/ethtool/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain libmnl"
PKG_LONGDESC="Ethtool is used for querying settings of an ethernet device and changing them."
PKG_BUILD_FLAGS="-cfg-libs"
