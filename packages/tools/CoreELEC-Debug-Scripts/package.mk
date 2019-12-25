# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="CoreELEC-Debug-Scripts"
PKG_VERSION="1b3f356349435ee4b6667832b4a61e5741762fdd"
PKG_SHA256="397354c23e785c66644101ca648c0d66b6edc5eef708fab3822100f0e462efb2"
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
