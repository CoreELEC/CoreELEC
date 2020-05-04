# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="librga"
PKG_VERSION="72e7764a9fe358e6ad50eb1b21176cc95802c7fb"
PKG_SHA256="3d4caa9513d12f9533b0e35dad1801e1454258b4ab6a75c30f0c42904b9c54da"
PKG_ARCH="arm aarch64"
PKG_LICENSE="GNU"
PKG_DEPENDS_TARGET="toolchain"
PKG_SITE="https://github.com/rockchip-linux/linux-rga"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_LONGDESC="The RGA driver userspace "
PKG_TOOLCHAIN="auto"

PKG_MAKE_OPTS_TARGET=" CFLAGS=-fPIC PROJECT_DIR=./build"

pre_make_target() { 
mkdir -p $PKG_BUILD/build/include
cd $PKG_BUILD
sed -i "s|DEBUG       	:= y|DEBUG       	:= n|g" $PKG_BUILD/Makefile
}

makeinstall_target() { 
mkdir -p $INSTALL/usr/lib
cp build/lib/librga.so $INSTALL/usr/lib

cd $PKG_BUILD
mkdir -p $SYSROOT_PREFIX/usr/include/rga/
# Library
cp build/lib/librga.so $SYSROOT_PREFIX/usr/lib/

# Headers
cp drmrga.h $SYSROOT_PREFIX/usr/include/rga/
cp rga.h $SYSROOT_PREFIX/usr/include/rga/
cp RgaApi.h $SYSROOT_PREFIX/usr//include/rga/
cp RockchipRgaMacro.h $SYSROOT_PREFIX/usr/include/rga/
}
