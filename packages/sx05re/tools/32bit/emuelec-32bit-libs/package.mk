# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="emuelec-32bit-libs"
PKG_VERSION="7b99aab2e1e7155f2a44630c758908be10d8c6f8"
PKG_SHA256="959413aa14b1af6056fd8fc4f7791f81ac1f300820fc887b14e73c08b176eafe"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/emuelec/emuelec-32bit-libs"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain $OPENGLES"
PKG_LONGDESC="EmuELEC 32-bit libraries, binaries and cores to use with EmuELEC aarch64"
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  mkdir -p $INSTALL
if [[ "$DEVICE" == "OdroidGoAdvance" ]] || [[ "$DEVICE" == "GameForce" ]]; then
	cp "$(get_build_dir mali-bifrost)/libmali.so_rk3326_gbm_arm32_r13p0_with_vulkan_and_cl" $PKG_BUILD/OdroidGoAdvance/usr/config/emuelec/lib32/libMali.so
	cp -rf $PKG_BUILD/OdroidGoAdvance/* $INSTALL/
	
	if [[ "${DEVICE}" == "GameForce" ]]; then
	   cp -rf $PKG_BUILD/GameForce/* $INSTALL/
	fi
	
elif [[ "${DEVICE}" == "Amlogic-ng" ]]; then
    cp -p "$(get_build_dir opengl-meson)/lib/eabihf/gondul/r12p0/fbdev/libMali.so" $PKG_BUILD/Amlogic-ng/usr/config/emuelec/lib32/libMali.gondul.so
    cp -p "$(get_build_dir opengl-meson)/lib/eabihf/dvalin/r12p0/fbdev/libMali.so" $PKG_BUILD/Amlogic-ng/usr/config/emuelec/lib32/libMali.dvalin.so
    cp -rf $PKG_BUILD/Amlogic-ng/* $INSTALL/
elif [[ "${DEVICE}" == "Amlogic" ]]; then
	cp -rf $PKG_BUILD/Amlogic/* $INSTALL/
fi

mkdir -p $INSTALL/usr/lib
ln -sf /emuelec/lib32 $INSTALL/usr/lib/arm-linux-gnueabihf
ln -sf /emuelec/lib32/ld-2.32.so $INSTALL/usr/lib/ld-linux-armhf.so.3

mkdir -p ${INSTALL}/usr/lib/libretro
cp ${PKG_DIR}/infos/*.info ${INSTALL}/usr/lib/libretro/
}
