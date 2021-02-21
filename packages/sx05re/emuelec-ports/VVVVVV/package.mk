# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="VVVVVV"
PKG_VERSION="979c5e3aa414dc1687e509ee586ac6dd46c3db2e"
PKG_SHA256="fedc6f14f3e4cd1b7ab5f0112b295ea02d12b892662edc78499cf038330884ae"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="CUSTOM"
PKG_SITE="https://github.com/TerryCavanagh/VVVVVV"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain SDL2-git"
PKG_SHORTDESC="VVVVVV License: https://github.com/TerryCavanagh/VVVVVV/blob/master/LICENSE.md"
PKG_LONGDESC="VVVVVV is a platform game all about exploring one simple mechanical idea - what if you reversed gravity instead of jumping?"
PKG_TOOLCHAIN="cmake"

if [ "$DEVICE" == "OdroidGoAdvance" ] || [ "$DEVICE" == "GameForce" ]; then
PKG_PATCH_DIRS="OdroidGoAdvance"
fi

PKG_CMAKE_OPTS_TARGET=" desktop_version"

pre_configure_target() {
sed -i "s/fullscreen = false/fullscreen = true/" "$PKG_BUILD/desktop_version/src/Screen.cpp"
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/bin
  cp VVVVVV $INSTALL/usr/bin
}
