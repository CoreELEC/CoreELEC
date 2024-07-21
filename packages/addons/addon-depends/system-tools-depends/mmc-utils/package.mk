# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="mmc-utils"
PKG_VERSION="123fd8b2ac3933be1319486fb1f32236a4a86a7c"
PKG_SHA256="d718338740cc75c8b0b54647a0522baff1824a31d4f9ee7d0d022405d07284f6"
PKG_LICENSE="GPL"
PKG_SITE="https://www.kernel.org/doc/html/latest/driver-api/mmc/mmc-tools.html"
PKG_URL="https://git.kernel.org/pub/scm/utils/mmc/mmc-utils.git/snapshot/mmc-utils-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Configure MMC storage devices from userspace."
PKG_BUILD_FLAGS="-sysroot"

PKG_MAKE_OPTS_TARGET+=" C="
