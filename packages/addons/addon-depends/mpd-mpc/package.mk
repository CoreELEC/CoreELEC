################################################################################
#      This file is part of LibreELEC - https://libreelec.tv
#      Copyright (C) 2018-present Team LibreELEC
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

PKG_NAME="mpd-mpc"
PKG_VERSION="0.30"
PKG_SHA256="65fc5b0a8430efe9acbe6e261127960682764b20ab994676371bdc797d867fce"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="https://www.musicpd.org"
PKG_URL="https://www.musicpd.org/download/mpc/0/mpc-${PKG_VERSION}.tar.xz"
PKG_SOURCE_DIR="mpc-${PKG_VERSION}*"
PKG_DEPENDS_TARGET="toolchain libiconv"
PKG_LONGDESC="Command-line client for MPD"
PKG_TOOLCHAIN="meson"

makeinstall_target() {
  :
}
