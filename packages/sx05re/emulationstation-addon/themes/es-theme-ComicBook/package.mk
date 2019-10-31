# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="es-theme-ComicBook"
PKG_VERSION="9a68f104b609f5bd7c7a782090817ebe48e9a197"
PKG_SHA256="fad822542cf54d9e7136921ae1759a53545ada79a6e98bcc8ab14223e0b5d582"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/TMNTturtleguy/es-theme-ComicBook"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_SECTION="emuelec"
PKG_SHORTDESC="ComicBook theme for Emulationstation"
PKG_IS_ADDON="no"
PKG_AUTORECONF="no"
PKG_TOOLCHAIN="manual"

make_target() {
  : not
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/config/emulationstation/themes/ComicBook
    cp -r * $INSTALL/usr/config/emulationstation/themes/ComicBook
    
  for i in ags apple2 atarijaguar batman bbcmicro channelf coco desktop dragon32 kids kodi lightgun love macintosh mame-libretro mame-advmame mame-mame4all mamerow mario mess oric ports ps2 pspminis racing retropie samcoupe sonic sports starwars steam stratagus ti99 TMNT trackball trs-80 wii wiiu zmachine; do
  rm -rf "$INSTALL/usr/config/emulationstation/themes/ComicBook/$i"
  done
}

