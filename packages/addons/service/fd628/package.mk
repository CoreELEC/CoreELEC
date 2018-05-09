################################################################################
#      This file is part of CoreELEC - http://coreelec.org
#      Copyright (C) 2018 Arthur Liberman (arthur_liberman (at) hotmail.com)
#
#  CoreELEC is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 2 of the License, or
#  (at your option) any later version.
#
#  CoreELEC is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with CoreELEC.  If not, see <http://www.gnu.org/licenses/>.
################################################################################

PKG_NAME="fd628"
PKG_VERSION="1.0"
PKG_REV="120"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="http://coreelec.org"
PKG_URL=""
PKG_DEPENDS_TARGET="toolchain"
PKG_SECTION="service"
PKG_SHORTDESC="Service for controlling FD628 VFD display icons"
PKG_LONGDESC="Service for controlling FD628 VFD display icons, e.g. Ethernet/WiFi connection status and Time"
PKG_TOOLCHAIN="manual"

PKG_IS_ADDON="yes"
PKG_ADDON_NAME="FD628 Service"
PKG_ADDON_PROJECTS="S905 S912"
PKG_ADDON_TYPE="xbmc.service"
PKG_MAINTAINER="Arthur Liberman (arthur_liberman@hotmail.com)"

addon() {
  mkdir -p $ADDON_BUILD/$PKG_ADDON_ID

  cp $PKG_DIR/addon.xml $ADDON_BUILD/$PKG_ADDON_ID

  # set only version (revision will be added by buildsystem)
  $SED -e "s|@ADDON_VERSION@|$ADDON_VERSION|g" \
       -i $ADDON_BUILD/$PKG_ADDON_ID/addon.xml
}
