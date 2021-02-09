# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="chocolate-doom"
PKG_VERSION="b26157aca5b12049b35d8dfcf969c51967a369f6"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/chocolate-doom/chocolate-doom"
PKG_URL="$PKG_SITE.git"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Chocolate Doom is a Doom source port that is minimalist and historically accurate."
GET_HANDLER_SUPPORT="git"
PKG_TOOLCHAIN="auto"

pre_configure_target() {
sed -i "s|./convert-font|$PKG_BUILD/textscreen/fonts/convert-font|g" $PKG_BUILD/textscreen/fonts/Makefile.am
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/config/emuelec/bin
  cd $PKG_BUILD
  cp .$TARGET_NAME/src/chocolate-* $INSTALL/usr/config/emuelec/bin
  cp .$TARGET_NAME/src/midiread $INSTALL/usr/config/emuelec/bin
  cp .$TARGET_NAME/src/mus2mid $INSTALL/usr/config/emuelec/bin
}
