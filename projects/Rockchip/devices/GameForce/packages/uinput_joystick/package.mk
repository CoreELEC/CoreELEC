# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="uinput_joystick"
PKG_VERSION="1.0"
PKG_LICENSE="Apachev2+"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Rockchip uinput joystick for r602"
PKG_TOOLCHAIN="manual"

make_target() {
	$CC $CFLAGS $LDFLAGS \
		$PKG_BUILD/ts_uart.c -o $PKG_BUILD/uinput_joystick \
		$PKG_BUILD/uinput-ctl.c -o $PKG_BUILD/uinput_joystick \
		$PKG_BUILD/common.c -o $PKG_BUILD/uinput_joystick
	
	$CC $CFLAGS $LDFLAGS $PKG_BUILD/fftest.c -o $PKG_BUILD/fftest
}

makeinstall_target() {
	mkdir -p $INSTALL/usr/bin/
	mkdir -p $INSTALL/etc/init.d/
	cp $PKG_BUILD/uinput_joystick $INSTALL/usr/bin/uinput_joystick
	cp $PKG_BUILD/fftest $INSTALL/usr/bin/fftest
}

post_install() {  
	enable_service uinput_joystick.service
}
