# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="nvidia-vaapi-driver"
PKG_VERSION="0.0.17"
PKG_SHA256="4e4bed9acef9881de8705c8e0286901f13f6b43c2df1b0c8b6a2f8ba2a186ac9"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/elFarto/nvidia-vaapi-driver"
PKG_URL="https://github.com/elFarto/nvidia-vaapi-driver/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libva nv-codec-headers gst-plugins-bad"
PKG_LONGDESC="A VA-API implementation using NVIDIA's NVDEC"
