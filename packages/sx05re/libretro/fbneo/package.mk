# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="fbneo"
PKG_VERSION="52322194e7b6842d85b8c8aeaaae022d6abcb4cf"
PKG_SHA256="b3f396cad0184cdd3239380b3091b2dda6d1635ad2d6b694d23d924444d1c980"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="Non-commercial"
PKG_SITE="https://github.com/libretro/FBNeo"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_SECTION="libretro"
PKG_SHORTDESC="Port of Final Burn Neo to Libretro (v0.2.97.38)."
PKG_LONGDESC="Currently, FB neo supports games on Capcom CPS-1 and CPS-2 hardware, SNK Neo-Geo hardware, Toaplan hardware, Cave hardware, and various games on miscellaneous hardware. "
PKG_TOOLCHAIN="make"


make_target() {
cd $PKG_BUILD/src/burner/libretro
    if [[ "$TARGET_FPU" =~ "neon" ]]; then
      make CC=$CC CXX=$CXX HAVE_NEON=1 USE_CYCLONE=1 profile=performance
    else
      make CC=$CC CXX=$CXX USE_CYCLONE=1 profile=performance
    fi
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp $PKG_BUILD/src/burner/libretro/fbneo_libretro.so $INSTALL/usr/lib/libretro/
}
