# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="emuelec-32bit-libs"
PKG_VERSION="0127ed6c0db21fdc5a655a36611e682a877ea934"
PKG_SHA256="fa6bff6521ed69080af6fed8961c5e4494ccd8b120f434fa8f6ef793f28d0efa"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/emuelec/emuelec-32bit-libs"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="EmuELEC 32-bit libraries, binaries and cores to use with EmuELEC aarch64"
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  mkdir -p $INSTALL
if [[ "$DEVICE" == "OdroidGoAdvance" ]] || [[ "$DEVICE" == "GameForce" ]]; then
	cp "$(get_build_dir mali-bifrost)/lib/arm-linux-gnueabihf/libmali-bifrost-g31-rxp0-gbm.so" $PKG_BUILD/OdroidGoAdvance/usr/config/emuelec/lib32/libmali.so
	cp -rf $PKG_BUILD/OdroidGoAdvance/* $INSTALL/
	
	if [[ "$DEVICE" == "GameForce" ]]; then
	   cp -rf $PKG_BUILD/GameForce/* $INSTALL/
	fi
	
elif [[ "$PROJECT" == "Amlogic-ng" ]]; then
    cp -p "$(get_build_dir opengl-meson)/lib/eabihf/gondul/r12p0/fbdev/libMali.so" $PKG_BUILD/Amlogic-ng/usr/config/emuelec/lib32/libMali.gondul.so
    cp -p "$(get_build_dir opengl-meson)/lib/eabihf/dvalin/r12p0/fbdev/libMali.so" $PKG_BUILD/Amlogic-ng/usr/config/emuelec/lib32/libMali.dvalin.so
    cp -rf $PKG_BUILD/Amlogic-ng/* $INSTALL/
elif [[ "$PROJECT" == "Amlogic" ]]; then
	cp -rf $PKG_BUILD/Amlogic/* $INSTALL/
fi

mkdir -p $INSTALL/usr/lib
ln -sf /emuelec/lib32 $INSTALL/usr/lib/arm-linux-gnueabihf
ln -sf /emuelec/lib32/ld-2.29.so $INSTALL/usr/lib/ld-linux-armhf.so.3
}
