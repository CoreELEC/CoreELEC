# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2021-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="supertuxkart"
PKG_VERSION="6c2c5131b07a422e27e16609ca67712dad8efa43"
PKG_SHA256="0d0afbb8189160b18410d3e0c437e18cfaeb1e19cd6958f09d3fb2cec6cf2638"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPLv3"
PKG_SITE="https://github.com/supertuxkart/stk-code"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain SDL2-git harfbuzz"
PKG_LONGDESC="SuperTuxKart is a free kart racing game. It focuses on fun and not on realistic kart physics. Instructions can be found on the in-game help page."

pre_configure_target() {
PKG_CMAKE_OPTS_TARGET+=" -DUSE_WIIUSE=OFF -DCHECK_ASSETS=off -DCMAKE_BUILD_TYPE=Release"
}

makeinstall_target() {
mkdir -p $INSTALL/usr/bin
cp $PKG_BUILD/.${TARGET_NAME}/bin/supertuxkart $INSTALL/usr/bin/
cp $PKG_DIR/scripts/* $INSTALL/usr/bin
}
