# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="volk"
PKG_VERSION="1.3.295"
PKG_SHA256="aea9f09c49f8a4e36738003c7aa5f08f99d68b96e4028ad9fa9039d2ee9fb251"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/zeux/volk"
PKG_URL="https://github.com/zeux/volk/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain vulkan-headers"
PKG_LONGDESC="Meta loader for Vulkan API"

PKG_CMAKE_OPTS_TARGET="-DVOLK_INSTALL=on"
