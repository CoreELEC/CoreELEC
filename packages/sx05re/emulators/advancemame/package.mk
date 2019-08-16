# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="advancemame"
PKG_VERSION="0038b8d3fc0976576b550eebad5295033d306ab5"
PKG_SHA256="0107bfd13d98cd030f5226d094f639f0bab883e9924a27f943c573720e56d727"
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

post_unpack() {
VERSION="EmuELEC-v$(cat $ROOT/packages/sx05re/emuelec/config/EE_VERSION)-${PKG_VERSION:0:7}"
echo $VERSION > $PKG_BUILD/.version
cd $PKG_DIR/joverride/
./convert.sh $(get_build_dir retroarch-joypad-autoconfig)/udev
cp -r $PKG_DIR/joverride/joverride.dat $PKG_BUILD/advance/linux/joverride.dat
#rm $PKG_DIR/joverride/joverride.dat
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
   cp -r $PKG_DIR/config/* $INSTALL/usr/share/advance/
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
