# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="xow"
PKG_VERSION="5c7ead6eb4785ad30e6adbcf62a36a35db9f6f53"
PKG_SHA256="bce29513afff4b696f9ce3f175fc1ec96f05ee14f4de32c366ee8073077176d2"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/medusalix/xow"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Linux driver for the Xbox One wireless dongle  "
PKG_TOOLCHAIN="make"

PKG_MAKE_OPTS_TARGET=" BUILD=RELEASE"

pre_configure_target() {
sed -i "s|ld -r|\$(LD) -r|" Makefile

}
makeinstall_target() {
mkdir -p $INSTALL/usr/bin
cp xow $INSTALL/usr/bin
}
