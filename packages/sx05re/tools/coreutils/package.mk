# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="coreutils"
PKG_VERSION="8.31"
PKG_SHA256=""
PKG_LICENSE="GPLv2+"
PKG_SITE="https://www.gnu.org/software/coreutils/"
PKG_URL="https://ftp.gnu.org/gnu/coreutils/coreutils-$PKG_VERSION.tar.xz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="The GNU Core Utilities are the basic file, shell and text manipulation utilities of the GNU operating system."
PKG_TOOLCHAIN="auto"


makeinstall_target() {
mkdir -p $INSTALL/usr/bin
cp $PKG_BUILD/.armv8a-libreelec-linux-gnueabi/src/stdbuf $INSTALL/usr/bin/
cp $PKG_BUILD/.armv8a-libreelec-linux-gnueabi/src/timeout $INSTALL/usr/bin/
cp $PKG_BUILD/.armv8a-libreelec-linux-gnueabi/src/sort $INSTALL/usr/bin/
mkdir -p $INSTALL/usr/lib/coreutils
cp $PKG_BUILD/.armv8a-libreelec-linux-gnueabi/src/libstdbuf.so $INSTALL/usr/lib/coreutils/
}
