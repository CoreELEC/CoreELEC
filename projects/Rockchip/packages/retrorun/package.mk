# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2021-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="retrorun"
PKG_VERSION="4d682040efcac6e9f903c91cae99db350afbf376"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/navy1978/retrorun-go2"
PKG_URL="$PKG_SITE.git"
PKG_DEPENDS_TARGET="toolchain libgo2 libdrm"
PKG_TOOLCHAIN="make"

pre_configure_target() {
	CFLAGS+=" -I$(get_build_dir libdrm)/include/drm"
	CFLAGS+=" -I$(get_build_dir linux)/include/uapi"
	CFLAGS+=" -I$(get_build_dir linux)/tools/include"
	PKG_MAKE_OPTS_TARGET=" config=release ARCH=" 
}

makeinstall_target() {
	mkdir -p $INSTALL/usr/bin
	if [ ${ARCH} == "arm" ]; then
		cp retrorun $INSTALL/usr/bin/retrorun32
	else
		cp retrorun $INSTALL/usr/bin/retrorun
	fi
}
