# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2021-present Frank Hartung (supervisedthinking (@) gmail.com)
# Copyright (C) 2021-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="spirv-tools"
# The SPIRV-Tools pkg_version needs to match the compatible (known_good) glslang pkg_version.
# https://raw.githubusercontent.com/KhronosGroup/glslang/${PKG_VERSION}/known_good.json
# When updating glslang pkg_version please update to the known_good spirv-tools pkg_version.
PKG_VERSION="b707790a898e44038547df54580022fc1cf89c3d"
PKG_SHA256="05d8af89737bde57571c48dbd36714c9f520a69623e14de72c3be6b600e277d6"
PKG_LICENSE="Apache-2.0"
PKG_SITE="https://github.com/KhronosGroup/SPIRV-Tools"
PKG_URL="https://github.com/KhronosGroup/SPIRV-Tools/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="toolchain:host"
PKG_DEPENDS_TARGET="toolchain"
PKG_DEPENDS_UNPACK="spirv-headers"
PKG_LONGDESC="The SPIR-V Tools project provides an API and commands for processing SPIR-V modules."

PKG_CMAKE_OPTS_HOST="-DSPIRV_SKIP_TESTS=ON"

post_unpack() {
  mkdir -p ${PKG_BUILD}/external/spirv-headers
    tar --strip-components=1 \
      -xf "${SOURCES}/spirv-headers/spirv-headers-$(get_pkg_version spirv-headers).tar.gz" \
      -C "${PKG_BUILD}/external/spirv-headers"
}
