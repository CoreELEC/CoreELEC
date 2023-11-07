# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="nvidia-vaapi-driver"
PKG_VERSION="0.0.11"
PKG_SHA256="3e1ed95d7e2b2b1377c9cb59c3e7caaf960134694e9441e1d91c38f224d1d5d9"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/elFarto/nvidia-vaapi-driver"
PKG_URL="https://github.com/elFarto/nvidia-vaapi-driver/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libva nv-codec-headers gst-plugins-bad"
PKG_LONGDESC="A VA-API implemention using NVIDIA's NVDEC"
