# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="rcheevos"
PKG_VERSION="12.3.0"
PKG_SHA256="bc7ab9985aee7b6a29e3d65492de9371c866236c5873a1c4493ec9cb6d2603d2"
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
