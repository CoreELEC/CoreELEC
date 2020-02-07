# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="Rockchip"
PKG_VERSION=""
PKG_LICENSE="various"
PKG_SITE=""
PKG_URL=""
PKG_DEPENDS_TARGET="toolchain emuelec"
PKG_TOOLCHAIN="manual"

makeinstall_target() {

if [ "$DEVICE" == "OdroidGoAdvance" ]; then
	mkdir -p $INSTALL/usr/share/bootloader
	cp boot.ini $INSTALL/usr/share/bootloader
fi

}
