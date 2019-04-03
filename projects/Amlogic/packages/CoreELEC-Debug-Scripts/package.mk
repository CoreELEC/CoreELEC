# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2019-present Team CoreELEC (https://coreelec.org)

PKG_NAME="CoreELEC-Debug-Scripts"
PKG_VERSION="7dc30732b8fbfd9685f8f340c04665d57ecb0711"
PKG_SHA256="db9eb1269a9aa10127ad26bd62e16cc10147aabb0a100554c65f28e5cf7b816c"
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
}
