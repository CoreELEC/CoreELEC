# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="nlohmann-json"
PKG_VERSION="3.11.3"
PKG_SHA256="0d8ef5af7f9794e3263480193c491549b2ba6cc74bb018906202ada498a79406"
PKG_LICENSE="MIT"
PKG_SITE="https://nlohmann.github.io/json/"
PKG_URL="https://github.com/nlohmann/json/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="JSON for Modern C++"
# Meson does not provide nlohmann_json*.cmake files which some projects rely on
PKG_TOOLCHAIN="cmake"

PKG_CMAKE_OPTS_TARGET="-DJSON_BuildTests=OFF"
