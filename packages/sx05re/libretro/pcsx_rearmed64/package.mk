# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2021-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="pcsx_rearmed64"
PKG_VERSION="3993490baa28964c5e3e8f879b58147184d9b0f7"
PKG_SHA256="02d44da19a32cdf4ca75ee53bdcdc910471c525bf2a54c26264ae1ac97de2c7a"
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
