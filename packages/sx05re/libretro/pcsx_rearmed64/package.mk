# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2021-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="pcsx_rearmed64"
PKG_VERSION="b715d67a0fee8609b878d46ca644dd70f51dfef2"
PKG_SHA256="d9e6be184fcd8ccc05d9e7a3f6206d2c71022e6f3847e2fe512444b9cabf20ad"
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
