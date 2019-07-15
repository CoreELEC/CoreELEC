# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="openbor"
PKG_VERSION="db5f0836f74a90091037da456f0985c1b822f5bf"
PKG_SHA256="42f20119cfa18105c9ae1c4109bf27ada9db9f36ee60808a0ba7ccdedb7bd828"
PKG_ARCH="any"
PKG_SITE="https://github.com/DCurrent/openbor"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain SDL2-git libogg libvorbisidec libvpx gl4es libpng16"
PKG_SHORTDESC="OpenBOR is the ultimate 2D side scrolling engine for beat em' ups, shooters, and more! "
PKG_LONGDESC="OpenBOR is the ultimate 2D side scrolling engine for beat em' ups, shooters, and more! "
PKG_TOOLCHAIN="manual"

pre_configure_target() {
 cd ..
 rm -rf .$TARGET_NAME
}

make_target() {
cd $PKG_BUILD/engine
./version.sh
  make CC=$CC CXX=$CXX AS=$CC BUILD_PANDORA=1
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/bin
    cp `find . -name "OpenBOR.elf" | xargs echo` $INSTALL/usr/bin/OpenBOR
    cp $PKG_DIR/scripts/*.sh $INSTALL/usr/bin
    chmod +x $INSTALL/usr/bin/*
    mkdir -p $INSTALL/usr/config/openbor  
	cp $PKG_DIR/config/* $INSTALL/usr/config/openbor
   } 
