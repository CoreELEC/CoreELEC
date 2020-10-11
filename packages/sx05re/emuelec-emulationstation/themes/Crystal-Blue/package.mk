# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="Crystal-Blue"
PKG_VERSION="abe669e09f519c4490f65a8ea935873e5f41e807"
PKG_SHA256="aa523e6d034c2b41464cfcdc32c9ed791d238946be045b88c08b2a1a420b71a9"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/dm2912/Crystal-Blue"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_SECTION="emuelec"
PKG_SHORTDESC="Crystal (Blue) Theme for EMUELEC by Dim (dm2912)"
PKG_TOOLCHAIN="manual"

make_target() {
  : not
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/config/emulationstation/themes/Crystal-Blue
    cp -r * $INSTALL/usr/config/emulationstation/themes/Crystal-Blue
}
