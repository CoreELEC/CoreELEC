################################################################################
#      This file is part of CoreELEC - https://coreelec.org
#      Copyright (C) 2018-present CoreELEC (team (at) coreelec.org)
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
#  along with CoreELEC. If not, see <http://www.gnu.org/licenses/>.
################################################################################

PKG_NAME="u-boot_firmware"
PKG_SITE="https://github.com/hardkernel/u-boot_firmware/"
PKG_DEPENDS_TARGET="toolchain"
PKG_ARCH="arm aarch64"
PKG_LICENSE="GPL"
PKG_SECTION="tools"
PKG_SHORTDESC="u-boot_firmware: Required firmware Files for U-Boot"
PKG_LONGDESC=""
PKG_TOOLCHAIN=manual

case "$DEVICE" in
  "Odroid_C2")
    PKG_VERSION="b7b90c1"
    PKG_URL="https://github.com/hardkernel/u-boot_firmware/archive/$PKG_VERSION.tar.gz"
    PKG_SHA256="39bf7c7a62647699572e088259cfe514579c09fa1b1b1ab3fade857b27da5ce9"
    ;;
esac

makeinstall_target() {
  :
}
