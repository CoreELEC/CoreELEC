# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2021-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="soniccd"
PKG_VERSION="ce80f3599ea9fe8ef06bf2aef2876246d7d904a0"
PKG_ARCH="any"
PKG_SITE="https://github.com/Rubberduckycooly/Sonic-CD-11-Decompilation"
PKG_URL="$PKG_SITE.git"
PKG_DEPENDS_TARGET="toolchain SDL2-git libtheora"
PKG_SHORTDESC="A Full Decompilation of Sonic CD 2011"
PKG_TOOLCHAIN="make"

makeinstall_target() {
mkdir -p $INSTALL/usr/bin
cp soniccd $INSTALL/usr/bin

mkdir -p $INSTALL/usr/config/emuelec/configs/sonic
cp $PKG_DIR/config/* $INSTALL/usr/config/emuelec/configs/sonic
} 
