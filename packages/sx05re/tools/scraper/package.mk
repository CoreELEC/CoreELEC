# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="scraper"
PKG_VERSION="1.4.6"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/sselph/scraper"
PKG_URL="https://github.com/sselph/scraper/releases/download/v$PKG_VERSION/scraper_rpi2.zip"
PKG_DEPENDS_TARGET="toolchain"
PKG_PRIORITY="optional"
PKG_SECTION="emuelec"
PKG_SHORTDESC="A scraper for EmulationStation written in Go using hashing"
PKG_LONGDESC="A scraper for EmulationStation written in Go using hashing"
PKG_TOOLCHAIN="make"
PKG_IS_ADDON="no"
PKG_AUTORECONF="no" 

unpack() {
mkdir -p $BUILD/$PKG_SOURCE_DIR-$PKG_VERSION
unzip -oq  $ROOT/sources/$PKG_NAME/$PKG_NAME-$PKG_VERSION.zip -d $BUILD/$PKG_NAME-$PKG_VERSION
}

post_unpack() {
rm $BUILD/$PKG_NAME-$PKG_VERSION/LICENSE.txt
}

make_target() {
: not 
}

makeinstall_target() {
mkdir -p $INSTALL/usr/bin/
    cp $BUILD/$PKG_NAME-$PKG_VERSION/* $INSTALL/usr/bin/
}

