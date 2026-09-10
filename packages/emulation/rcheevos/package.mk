# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="rcheevos"
PKG_VERSION="12.5.0"
PKG_SHA256="e6df83de4e18f0a19206e711e1e589624dcfcf6bf529c14f6dd2bb2d1ced983f"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/RetroAchievements/rcheevos"
PKG_URL="https://github.com/RetroAchievements/rcheevos/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libretro-common"
PKG_LONGDESC="Library to parse and evaluate achievements and leaderboards for RetroAchievements"
PKG_BUILD_FLAGS="+pic"

post_unpack() {
  # rcheevos doesn't come with any build files, use a copy of the cmake file in
  # game.libretro (depends/common/rcheevos/CMakeLists.txt)
  cp "${PKG_DIR}/source/CMakeLists.txt" "${PKG_BUILD}"
}
