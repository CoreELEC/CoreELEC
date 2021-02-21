# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="dolphinSA"
PKG_VERSION="0776263c5e1f205f77c4836c1489c7200da34763"
PKG_SHA256="87f68d53d4d84c3784ce27e8314994f1e7d7d67d0f1a663a248fbeae0e1a05ce"
PKG_ARCH="aarch64"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/dolphin-emu/dolphin"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain qt-everywhere libevdev"
PKG_LONGDESC="Dolphin is a GameCube / Wii emulator, allowing you to play games for these two platforms on PC with improvements. "
PKG_BUILD_FLAGS="-lto"

PKG_CMAKE_OPTS_TARGET=" -DENABLE_LTO=on -DDISTRIBUTOR='EmuELEC' -DBUILD_SHARED_LIBS=OFF -DTHREADS_PTHREAD_ARG=OFF -DENABLE_FBDEV=ON -DENABLE_EGL=ON -DENABLE_X11=OFF -DENABLE_NOGUI=ON -DUSE_DISCORD_PRESENCE=OFF -DCMAKE_BUILD_TYPE=Release"

makeinstall_target() {
export CXXFLAGS="`echo $CXXFLAGS | sed -e "s|-O.|-O2|g"`"
mkdir -p $INSTALL/usr/bin
cp -rf $PKG_BUILD/.${TARGET_NAME}/Binaries/dolphin-emu-nogui $INSTALL/usr/bin
cp -rf $PKG_DIR/scripts/* $INSTALL/usr/bin

mkdir -p $INSTALL/usr/config/emuelec/configs/dolphin-emu
cp -rf $PKG_BUILD/Data/Sys/* $INSTALL/usr/config/emuelec/configs/dolphin-emu
cp -rf $PKG_DIR/config/* $INSTALL/usr/config/emuelec/configs/dolphin-emu
}
