# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2021-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="gl4es"
PKG_VERSION="83b074dcb0028e239da2ad9789db80cc1d5f2544"
PKG_SHA256="2031be77b49398cdf945c110e59530a48113c35fd69c0a7381be21fc9eaccc79"
#PKG_SITE="https://github.com/ptitSeb/gl4es"
PKG_GIT_CLONE_BRANCH="sk_hacks"
PKG_SITE="https://github.com/JohnnyonFlame/gl4es"
PKG_LICENSE="GPL"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain ${OPENGLES}"
PKG_LONGDESC=" GL4ES is a OpenGL 2.1/1.5 to GL ES 2.0/1.1 translation library, with support for Pandora, ODroid, OrangePI, CHIP, Raspberry PI, Android, Emscripten and AmigaOS4. "
PKG_TOOLCHAIN="cmake-make"

pre_configure_target() {

if [[ "${PROJECT}" == "Amlogic"* ]]; then
	PKG_CMAKE_OPTS_TARGET=" -DNOX11=1 -DODROID=1 -DGBM=OFF -DCMAKE_BUILD_TYPE=Release "
else
	PKG_CMAKE_OPTS_TARGET=" -DNOX11=1 -DODROID=1 -DGBM=ON -DCMAKE_BUILD_TYPE=Release "
fi

}
