# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2021-present Frank Hartung (supervisedthinking (@) gmail.com)
# Copyright (C) 2021-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="spirv-headers"
# The SPIRV-Headers pkg_version needs to match the compatible (known_good) glslang pkg_version.
# https://raw.githubusercontent.com/KhronosGroup/glslang/${PKG_VERSION}/known_good.json
# When updating glslang pkg_version please update to the known_good spirv-headers pkg_version.
# When updating spirv-llvm-translator pkg_version validate the minimum githash from
# https://github.com/KhronosGroup/SPIRV-LLVM-Translator/blob/main/spirv-headers-tag.conf
PKG_VERSION="29981f65241605e08b0ede4cfeb999fe3b723c6a"
PKG_SHA256="232899f1ad4104fb5bc377b94596c7621575eee62ad9a9e8f929b63a7dd8a7ad"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/KhronosGroup/SPIRV-headers"
PKG_URL="https://github.com/KhronosGroup/SPIRV-headers/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST=""
PKG_LONGDESC="SPIRV-Headers"
PKG_TOOLCHAIN="manual"
