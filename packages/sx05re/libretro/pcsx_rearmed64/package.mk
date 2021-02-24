# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2021-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="pcsx_rearmed64"
PKG_VERSION="e3e1b865f7c06f57918b97f7293b5b2959fb7b7d"
PKG_SHA256="3f792d6520c41e453920abf99a196d1750f351a8e9749172df64ab60a7117772"
PKG_REV="1"
PKG_ARCH="aarch64"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/libretro/pcsx_rearmed"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain alsa"
PKG_SHORTDESC="ARM optimized PCSX fork"
PKG_TOOLCHAIN="make"
PKG_BUILD_FLAGS="+speed -gold"

make_target() {
cd ${PKG_BUILD}

if [ "${PROJECT}" == "Amlogic" ]; then
	make -f Makefile.libretro GIT_VERSION=${PKG_VERSION} platform=h5
elif [ "${DEVICE}" == "OdroidGoAdvance" ] || [ "${DEVICE}" == "Gameforce" ]; then
	sed -i "s|cortex-a53|cortex-a35|g" Makefile.libretro
	make -f Makefile.libretro GIT_VERSION=${PKG_VERSION} platform=h5
else
	make -f Makefile.libretro GIT_VERSION=${PKG_VERSION} platform=CortexA73_G12B
fi
}

makeinstall_target() {

mkdir -p ${INSTALL}/usr/lib/libretro/
   cp pcsx_rearmed_libretro.so ${INSTALL}/usr/lib/libretro/pcsx_rearmed_64_libretro.so

}
