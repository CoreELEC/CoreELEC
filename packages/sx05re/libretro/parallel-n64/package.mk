# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="parallel-n64"
PKG_VERSION="dba9fc8b64351b6cf3dfb0bec2ec39f01101f48e"
PKG_SHA256="1740a6cf3ca4c2c2a1076c55ffb513455c4c6295290bc755314e9d9d925b3f2b"
PKG_REV="2"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/libretro/parallel-n64"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_SECTION="libretro"
PKG_SHORTDESC="Optimized/rewritten Nintendo 64 emulator made specifically for Libretro. Originally based on Mupen64 Plus."
PKG_LONGDESC="Optimized/rewritten Nintendo 64 emulator made specifically for Libretro. Originally based on Mupen64 Plus."
PKG_TOOLCHAIN="make"
PKG_BUILD_FLAGS="-lto"

if [[ "$ARCH" == "arm" ]]; then
	PKG_PATCH_DIRS="${ARCH}"
	PKG_MAKE_OPTS_TARGET=" platform=${PROJECT}"
	
if [ "${DEVICE}" == "OdroidGoAdvance" ] || [ "$DEVICE" == "GameForce" ]; then
		PKG_MAKE_OPTS_TARGET=" platform=Odroidgoa"
	fi
else
	PKG_PATCH_DIRS="emuelec-aarch64"
	PKG_MAKE_OPTS_TARGET=" platform=emuelec64-armv8"
	
if [ "${DEVICE}" == "OdroidGoAdvance" ] || [ "$DEVICE" == "GameForce" ]; then
#todo add odroidgoadvance to 64bits
		PKG_MAKE_OPTS_TARGET=" platform=emuelec64-armv8"
	fi
fi

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp parallel_n64_libretro.so $INSTALL/usr/lib/libretro/
}
