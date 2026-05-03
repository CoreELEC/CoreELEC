# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="ethtool"
PKG_VERSION="7.0"
PKG_SHA256="660bf9725a7871343a0d232068a7634fbcfb69b6c2f8eff455827faefb0cd162"
PKG_LICENSE="GPL"
PKG_SITE="https://www.kernel.org/pub/software/network/ethtool/"
PKG_URL="https://www.kernel.org/pub/software/network/ethtool/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain libmnl"
PKG_LONGDESC="Ethtool is used for querying settings of an ethernet device and changing them."
PKG_BUILD_FLAGS="-cfg-libs"
