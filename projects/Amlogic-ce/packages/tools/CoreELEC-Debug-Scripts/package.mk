# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="CoreELEC-Debug-Scripts"
PKG_VERSION="558f2530a0ed36986bf01b1bebbc043d5b4e44f9"
PKG_SHA256="5f1235d4a2e0054ee90e421f54468a73a39f070c0a446984edb36b8fb6470b96"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/cdu13a/CoreELEC-Debug-Scripts"
PKG_URL="https://github.com/cdu13a/CoreELEC-Debug-Scripts/archive/$PKG_VERSION.tar.gz"
PKG_SOURCE_NAME="$PKG_NAME-$PKG_VERSION.tar.gz"
PKG_LONGDESC="A set of scripts to help debug user issues with CoreELEC"
PKG_TOOLCHAIN="manual"


makeinstall_target() {
  mkdir -p $INSTALL/usr/bin
    install -m 0755 dispinfo.sh $INSTALL/usr/bin/dispinfo
    install -m 0755 remoteinfo.sh $INSTALL/usr/bin/remoteinfo
    install -m 0755 audinfo.sh $INSTALL/usr/bin/audinfo
    install -m 0755 ce-debug.sh $INSTALL/usr/bin/ce-debug
}
