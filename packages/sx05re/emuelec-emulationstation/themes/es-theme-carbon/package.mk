# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="es-theme-carbon"
PKG_VERSION="10ad0b91e354c1abfa68763fcdfa90aa550e6475"
PKG_SHA256="c902098b9e10c0a6c9c56e86aa14e8534aecf54ce179e0ea1e83b0b7f84b7be0"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/fabricecaruso/es-theme-carbon"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_SECTION="emuelec"
PKG_SHORTDESC="The EmulationStation theme Carbon Fabrice Caruso's fork"
PKG_IS_ADDON="no"
PKG_AUTORECONF="no"
PKG_TOOLCHAIN="manual"

make_target() {
  : not
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/config/emulationstation/themes/es-theme-carbon
    cp -r * $INSTALL/usr/config/emulationstation/themes/es-theme-carbon
}
