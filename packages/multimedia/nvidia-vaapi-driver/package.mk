# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="nvidia-vaapi-driver"
PKG_VERSION="0.0.10"
PKG_SHA256="df63b0832ccf9f74a90ff8b26d8e069606e0614d1748e35a857d2e53745ab95c"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/elFarto/nvidia-vaapi-driver"
PKG_URL="https://github.com/elFarto/nvidia-vaapi-driver/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libva nv-codec-headers gst-plugins-bad"
PKG_LONGDESC="A VA-API implemention using NVIDIA's NVDEC"
