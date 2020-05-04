# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="VVVVVV"
PKG_VERSION="f78603ef87128c72d3ae433ce86c76b739096f00"
PKG_SHA256="44afbde789514c2a431572551995f9021cdd4b831f86f0654cc2a8a01b90fbdf"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="CUSTOM"
PKG_SITE="https://github.com/TerryCavanagh/VVVVVV"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain SDL2-git"
PKG_SHORTDESC="VVVVVV License: https://github.com/TerryCavanagh/VVVVVV/blob/master/LICENSE.md"
PKG_LONGDESC="VVVVVV is a platform game all about exploring one simple mechanical idea - what if you reversed gravity instead of jumping?"
PKG_TOOLCHAIN="cmake"

if [ "$DEVICE" == "OdroidGoAdvance" ]; then
PKG_PATCH_DIRS="OdroidGoAdvance"
fi

PKG_CMAKE_OPTS_TARGET=" desktop_version"

pre_configure_target() {
sed -i "s/fullscreen = false/fullscreen = true/" "$PKG_BUILD/desktop_version/src/Game.cpp"
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/config/emuelec/bin
  cp VVVVVV $INSTALL/usr/config/emuelec/bin
}
