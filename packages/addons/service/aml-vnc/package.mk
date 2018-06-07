################################################################################
#      This file is part of LibreELEC - http://www.libreelec.tv
#      Copyright (C) 2016 Team LibreELEC
#
#  LibreELEC is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 2 of the License, or
#  (at your option) any later version.
#
#  LibreELEC is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with LibreELEC.  If not, see <http://www.gnu.org/licenses/>.
################################################################################

PKG_NAME="aml-vnc"
PKG_VERSION="0.1"
PKG_REV="3"
PKG_ARCH="arm aarch64"
PKG_LICENSE="OSS"
PKG_SITE="https://github.com/kszaq/aml-vnc"
PKG_URL=""
PKG_DEPENDS_TARGET="toolchain libvncserver"
PKG_PRIORITY="optional"
PKG_SECTION="service/system"
PKG_SHORTDESC="VNC Server for Amlogic devices"
PKG_LONGDESC="VNC Server for Amlogic devices"
PKG_DISCLAIMER="this is an unofficial addon. please don't ask for support in libreelec forum / irc channel"

PKG_IS_ADDON="yes"
PKG_ADDON_TYPE="xbmc.service"
PKG_ADDON_PROVIDES=""

PKG_AUTORECONF="no"

PKG_MAINTAINER="kszaq (kszaquitto at gmail.com)"

makeinstall_target() {
  : # nop
}

addon() {
  mkdir -p $ADDON_BUILD/$PKG_ADDON_ID/bin
  cp -P $PKG_BUILD/aml-vnc $ADDON_BUILD/$PKG_ADDON_ID/bin
  debug_strip $ADDON_BUILD/$PKG_ADDON_ID/
}
