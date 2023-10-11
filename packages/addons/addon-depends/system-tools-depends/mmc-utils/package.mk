# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="mmc-utils"
PKG_VERSION="613495ecaca97a19fa7f8f3ea23306472b36453c"
PKG_SHA256="1c76924aa3f636af70bd841bc1dcd85d5728ef1d4326921da30cbab7d643e2a7"
PKG_LICENSE="GPL"
PKG_SITE="https://www.kernel.org/doc/html/latest/driver-api/mmc/mmc-tools.html"
PKG_URL="https://git.kernel.org/pub/scm/utils/mmc/mmc-utils.git/snapshot/mmc-utils-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Configure MMC storage devices from userspace."
PKG_BUILD_FLAGS="-sysroot"
