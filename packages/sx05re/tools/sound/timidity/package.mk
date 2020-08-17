# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="timidity"
PKG_VERSION="2.15.0"
PKG_ARCH="any"
PKG_LICENSE="GPL2"
PKG_SITE="https://sourceforge.net/projects/timidity/"
PKG_URL="$SOURCEFORGE_SRC/timidity/TiMidity++-$PKG_VERSION.tar.xz"
PKG_DEPENDS_TARGET="toolchain SDL2-git SDL2_mixer"
PKG_LONGDESC="TiMidity++"
PKG_TOOLCHAIN="manual"

pre_configure_target() {
PKG_CONFIGURE_OPTS_TARGET=" --host=${TARGET_NAME} enable_audio=alsa --with-default-output=alsa --with-default-path=/storage/.config/timidity"
}

make_target() {
$PKG_BUILD/configure $PKG_CONFIGURE_OPTS_TARGET
make || echo "fail" #for some reason compilation fails once then it works
make
}

makeinstall_target() {
mkdir -p $INSTALL/usr/config/timidity
cp -rf $PKG_DIR/config/* $INSTALL/usr/config/timidity

mkdir -p $INSTALL/usr/bin
cp $PKG_BUILD/.${TARGET_NAME}/timidity/timidity $INSTALL/usr/bin

}
