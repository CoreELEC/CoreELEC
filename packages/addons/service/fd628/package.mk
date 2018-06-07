################################################################################
#      This file is part of CoreELEC - https://coreelec.org
#      Copyright (C) 2018-present CoreELEC (team (at) coreelec.org)
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
PKG_REV="100"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE=""
PKG_URL=""
PKG_DEPENDS_TARGET="toolchain"
PKG_SECTION="service"
PKG_SHORTDESC="Add-on removed"
PKG_LONGDESC="Add-on removed"
PKG_TOOLCHAIN="manual"

PKG_ADDON_BROKEN="FD628 Display is no longer maintained and has been superseded by VFD Display."

PKG_IS_ADDON="yes"
PKG_ADDON_NAME="FD628 Display"
PKG_ADDON_TYPE="xbmc.broken"

addon() {
  :
}
