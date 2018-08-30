# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)
# Copyright (C) 2018-present CoreELEC (https://coreelec.org)

PKG_NAME="CoreELEC-settings"
PKG_VERSION="61dc05b32cc6a033cc1d7f37d8ec4da55f46aed4"
PKG_SHA256="00c64b58d021ae4bed0a7e019f2c6f2acf3076efd7a4ada3840542cc0d804934"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="https://coreelec.org"
PKG_URL="https://github.com/CoreELEC/service.coreelec.settings/archive/$PKG_VERSION.tar.gz"
PKG_SOURCE_DIR="service.coreelec.settings-$PKG_VERSION*"
PKG_DEPENDS_TARGET="toolchain Python2 connman pygobject dbus-python"
PKG_SECTION=""
PKG_SHORTDESC="CoreELEC-settings: Settings dialog for CoreELEC"
PKG_LONGDESC="CoreELEC-settings: is a settings dialog for CoreELEC"

PKG_MAKE_OPTS_TARGET="DISTRONAME=$DISTRONAME ROOT_PASSWORD=$ROOT_PASSWORD"

if [ "$DISPLAYSERVER" = "x11" ]; then
  PKG_DEPENDS_TARGET="$PKG_DEPENDS_TARGET setxkbmap"
else
  PKG_DEPENDS_TARGET="$PKG_DEPENDS_TARGET bkeymaps"
fi

post_makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/coreelec
    cp $PKG_DIR/scripts/* $INSTALL/usr/lib/coreelec

#  # bluetooth is optional
#    if [ ! "$BLUETOOTH_SUPPORT" = yes ]; then
#      rm -f resources/lib/modules/bluetooth.py
#    fi

  ADDON_INSTALL_DIR=$INSTALL/usr/share/kodi/addons/service.coreelec.settings

  $TOOLCHAIN/bin/python -Wi -t -B $TOOLCHAIN/lib/$PKG_PYTHON_VERSION/compileall.py $ADDON_INSTALL_DIR/resources/lib/ -f
  rm -rf $(find $ADDON_INSTALL_DIR/resources/lib/ -name "*.py")

  $TOOLCHAIN/bin/python -Wi -t -B $TOOLCHAIN/lib/$PKG_PYTHON_VERSION/compileall.py $ADDON_INSTALL_DIR/oe.py -f
  rm -rf $ADDON_INSTALL_DIR/oe.py
}

post_install() {
  enable_service backup-restore.service
  enable_service factory-reset.service
}
