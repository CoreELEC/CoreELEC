# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="chocolate-doom"
PKG_VERSION="01168584c8f0e6485efcd8ffa5e9f720809eb64c"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/chocolate-doom/chocolate-doom"
PKG_URL="$PKG_SITE.git"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Chocolate Doom is a Doom source port that is minimalist and historically accurate."
GET_HANDLER_SUPPORT="git"
PKG_TOOLCHAIN="auto"

makeinstall_target() {
  mkdir -p $INSTALL/usr/config/emuelec/bin
  cd $PKG_BUILD
  cp .$TARGET_NAME/src/chocolate-* $INSTALL/usr/config/emuelec/bin
  cp .$TARGET_NAME/src/midiread $INSTALL/usr/config/emuelec/bin
  cp .$TARGET_NAME/src/mus2mid $INSTALL/usr/config/emuelec/bin
}
