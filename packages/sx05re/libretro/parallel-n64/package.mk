# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="parallel-n64"
PKG_VERSION="76193f8308c467f0f4e88b4550d30d9331da5958"
PKG_SHA256="82b7f50dae6e2fd5bc324f303995bed94700d46859b744179b5af69f6e2b5415"
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

PKG_MAKE_OPTS_TARGET=" platform=${PROJECT}"

if [ "${DEVICE}" == "OdroidGoAdvance" ]; then
PKG_MAKE_OPTS_TARGET=" platform=Odroidgoa"
fi

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp parallel_n64_libretro.so $INSTALL/usr/lib/libretro/
}
