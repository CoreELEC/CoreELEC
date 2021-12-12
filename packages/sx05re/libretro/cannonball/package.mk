################################################################################
#      This file is part of Lakka - http://www.lakka.tv
#
#  This Program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2, or (at your option)
#  any later version.
#
#  This Program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with OpenELEC.tv; see the file COPYING.  If not, write to
#  the Free Software Foundation, 51 Franklin Street, Suite 500, Boston, MA 02110, USA.
#  http://www.gnu.org/copyleft/gpl.html
################################################################################

PKG_NAME="cannonball"
PKG_VERSION="81f9b56381a4bd0e4ab90f81370c057b9ef46f7f"
PKG_SHA256="35851d362e24a5235a71afa91d06786e044d2358e7f609051ac91007332c44b5"
PKG_ARCH="any"
PKG_LICENSE="GPLv3"
PKG_SITE="https://github.com/libretro/cannonball"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_PRIORITY="optional"
PKG_SECTION="libretro"
PKG_SHORTDESC="Cannonball: An Enhanced OutRun Engine"
PKG_LONGDESC="Cannonball: An Enhanced OutRun Engine"

PKG_IS_ADDON="no"
PKG_TOOLCHAIN="make"
PKG_AUTORECONF="no"

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp cannonball_libretro.so $INSTALL/usr/lib/libretro/
}
