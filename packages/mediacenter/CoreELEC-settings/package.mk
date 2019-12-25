# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="CoreELEC-settings"
PKG_VERSION="f491c71e6fcde9e510cb8d81c7ef856bea58f7c8"
PKG_SHA256="aad9c6947e66841c3e336028d4f53fd8a04aea55c509803ce87665111f07cab1"
PKG_LICENSE="GPL"
PKG_SITE="https://coreelec.org"
PKG_URL="https://github.com/CoreELEC/service.coreelec.settings/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain Python2 connman pygobject dbus-python"
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
