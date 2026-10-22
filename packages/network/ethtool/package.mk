# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="ethtool"
PKG_VERSION="7.2"
PKG_SHA256="44256ddf4631ed08d816b7b8725f4b2e0f05e96b8c7ca64ee5c81449da2b6507"
PKG_LICENSE="GPL-2.0-only"
PKG_SITE="https://www.kernel.org/pub/software/network/ethtool/"
PKG_URL="https://www.kernel.org/pub/software/network/ethtool/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain libmnl"
PKG_LONGDESC="Ethtool is used for querying settings of an ethernet device and changing them."
PKG_BUILD_FLAGS="-cfg-libs"
