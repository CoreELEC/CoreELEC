# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2021-present Frank Hartung (supervisedthinking (@) gmail.com)
# Copyright (C) 2021-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="spirv-tools"
# The SPIRV-Tools pkg_version needs to match the compatible (known_good) glslang pkg_version.
# https://raw.githubusercontent.com/KhronosGroup/glslang/${PKG_VERSION}/known_good.json
# When updating glslang pkg_version please update to the known_good spirv-tools pkg_version.
PKG_VERSION="4b6bd5a665ba0529747af3a0bc808732b7e78f48"
PKG_SHA256="890d9e33b8bb79a8799a2887b442e55c93db0a4db94252339c915a1d90857505"
PKG_LICENSE="Apache-2.0"
PKG_SITE="https://github.com/KhronosGroup/SPIRV-Tools"
PKG_URL="https://github.com/KhronosGroup/SPIRV-Tools/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST=""
PKG_LONGDESC="The SPIR-V Tools project provides an API and commands for processing SPIR-V modules."
PKG_TOOLCHAIN="manual"
