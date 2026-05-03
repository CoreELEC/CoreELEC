# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2021-present Frank Hartung (supervisedthinking (@) gmail.com)
# Copyright (C) 2021-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="spirv-headers"
# The SPIRV-Headers pkg_version needs to match the compatible (known_good) glslang pkg_version.
# https://raw.githubusercontent.com/KhronosGroup/glslang/${PKG_VERSION}/known_good.json
# When updating glslang pkg_version please update to the known_good spirv-headers pkg_version.
# When updating spirv-llvm-translator pkg_version validate the minimum githash from
# https://github.com/KhronosGroup/SPIRV-LLVM-Translator/blob/main/spirv-headers-tag.conf
PKG_VERSION="1a22b167081842915a1c78a0b5b5a353a23284aa"
PKG_SHA256="25ffb47c375650264547b3a5c42a0ad5b03378a5a7c10efcd3ae99ba9fe75cb7"
PKG_LICENSE="Apache-2.0"
PKG_SITE="https://github.com/KhronosGroup/SPIRV-headers"
PKG_URL="https://github.com/KhronosGroup/SPIRV-headers/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST=""
PKG_LONGDESC="SPIRV-Headers"
PKG_TOOLCHAIN="manual"
