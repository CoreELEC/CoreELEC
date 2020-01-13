# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="es-theme-EmuELEC-carbon"
PKG_VERSION="11340bf9818c0214d7e3ca13f47399ffbb4e0a7d"
PKG_SHA256="c660ecfe340da937bbc7a73a2f605e1c26ca099758ea16b1e51244ae96d868f0"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/EmuELEC/es-theme-EmuELEC-carbon"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_SECTION="emuelec"
PKG_SHORTDESC="The EmulationStation theme Carbon Fabrice Caruso's fork with changes for EmuELEC by drixplm"
PKG_IS_ADDON="no"
PKG_AUTORECONF="no"
PKG_TOOLCHAIN="manual"

make_target() {
  : not
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/config/emulationstation/themes/es-theme-EmuELEC-carbon
    cp -r * $INSTALL/usr/config/emulationstation/themes/es-theme-EmuELEC-carbon
}
