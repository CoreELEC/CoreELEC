# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="advancemame"
PKG_VERSION="fa0e6e536b50c1ea81c31089e85b0afb5465fe92"
PKG_SHA256="a689c78f95a812f0df0b2523e245d93c912422ec5763c6a765f493269400758b"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="MAME"
PKG_SITE="https://github.com/amadvance/advancemame"
PKG_URL="https://github.com/amadvance/advancemame/archive/$PKG_VERSION.tar.gz"
PKG_SOURCE_DIR="advancemame-$PKG_VERSION*"
PKG_DEPENDS_TARGET="toolchain freetype slang alsa retroarch-joypad-autoconfig"
PKG_SECTION="emuelec/mod"
PKG_SHORTDESC="A MAME and MESS port with an advanced video support for Arcade Monitors, TVs, and PC Monitors "
PKG_LONGDESC="A MAME and MESS port with an advanced video support for Arcade Monitors, TVs, and PC Monitors "
PKG_IS_ADDON="no"
PKG_AUTORECONF="no"
PKG_TOOLCHAIN="make"
PKG_NEED_UNPACK="retroarch-joypad-autoconfig"

pre_configure_target() {
export CFLAGS=`echo $CFLAGS | sed -e "s|-O.|-O3|g"`
}

pre_make_target() {
VERSION="EmuELEC-v$(cat $ROOT/packages/sx05re/emuelec/config/EE_VERSION)-${PKG_VERSION:0:7}"
echo $VERSION > $PKG_BUILD/.version
cd $PKG_DIR/joverride/
./convert.sh $(get_build_dir retroarch-joypad-autoconfig)/udev
cp -r $PKG_DIR/joverride/joverride.dat $PKG_BUILD/advance/linux/joverride.dat
rm $PKG_DIR/joverride/joverride.dat
}

make_target() {
cd $PKG_BUILD
./autogen.sh
./configure --prefix=/usr --datadir=/usr/share/ --datarootdir=/usr/share/ --host=armv8a-libreelec-linux --enable-fb --enable-freetype --with-freetype-prefix=$SYSROOT_PREFIX/usr/ --enable-slang
make mame
}

makeinstall_target() {
 : not
}

post_make_target() { 
mkdir -p $INSTALL/usr/share/advance
if [ "$DEVICE" == "OdroidGoAdvance" ]; then
   cp -r $PKG_DIR/config/advmame.rc_oga $INSTALL/usr/share/advance/advmame.rc
else
   cp -r $PKG_DIR/config/advmame.rc $INSTALL/usr/share/advance/advmame.rc
fi
   
mkdir -p $INSTALL/usr/bin
   cp -r $PKG_DIR/bin/* $INSTALL/usr/bin
chmod +x $INSTALL/usr/bin/advmame.sh

cp -r $PKG_BUILD/obj/mame/linux/blend/advmame $INSTALL/usr/bin
cp -r $PKG_BUILD/support/category.ini $INSTALL/usr/share/advance
cp -r $PKG_BUILD/support/sysinfo.dat $INSTALL/usr/share/advance
cp -r $PKG_BUILD/support/history.dat $INSTALL/usr/share/advance
cp -r $PKG_BUILD/support/hiscore.dat $INSTALL/usr/share/advance
cp -r $PKG_BUILD/support/event.dat $INSTALL/usr/share/advance
CFLAGS=$OLDCFLAGS
}
