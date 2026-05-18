# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="gbm"
PKG_VERSION=""
PKG_LICENSE="OSS"
PKG_URL=""
PKG_DEPENDS_TARGET="toolchain"
PKG_SECTION="virtual"
PKG_LONGDESC="Base packages for running on GBM"

# NVIDIA drivers for Linux
if listcontains "${GRAPHIC_DRIVERS}" "nvidia-ng"; then
  PKG_DEPENDS_TARGET+=" nvidia"
fi
