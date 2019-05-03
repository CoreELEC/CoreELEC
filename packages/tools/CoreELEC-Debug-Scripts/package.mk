# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Team CoreELEC (https://coreelec.org)

PKG_NAME="CoreELEC-Debug-Scripts"
PKG_VERSION="a7a4c19c5b77bc8a60aa44fd06bfe0791fad0996"
PKG_SHA256="2e8312fd170822fb32e3ef3f51b4849c9d872010c3c8f700d70e31f79034620e"
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
