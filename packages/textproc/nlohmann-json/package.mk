# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="nlohmann-json"
PKG_VERSION="3.12.0"
PKG_SHA256="4b92eb0c06d10683f7447ce9406cb97cd4b453be18d7279320f7b2f025c10187"
PKG_LICENSE="MIT"
PKG_SITE="https://nlohmann.github.io/json/"
PKG_URL="https://github.com/nlohmann/json/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="JSON for Modern C++"
# Meson does not provide nlohmann_json*.cmake files which some projects rely on
PKG_TOOLCHAIN="cmake"

PKG_CMAKE_OPTS_TARGET="-DJSON_BuildTests=OFF"
