################################################################################
#      This file is part of LibreELEC - https://libreelec.tv
#      Copyright (C) 2016-present Team LibreELEC
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

PKG_NAME="gcc-linaro-aarch64-elf"
PKG_VERSION="4.9-2016.02"
PKG_SHA256="67b8d6d3c764a5b2e21bf4a9cc990f6d6db691df5fcc814e34051c521346ec10"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE=""
PKG_URL="https://releases.linaro.org/components/toolchain/binaries/${PKG_VERSION}/aarch64-elf/gcc-linaro-${PKG_VERSION}-x86_64_aarch64-elf.tar.xz"
PKG_SOURCE_DIR="gcc-linaro-${PKG_VERSION}-x86_64_aarch64-elf"
PKG_DEPENDS_HOST="toolchain"
PKG_SECTION="lang"
PKG_LONGDESC="Linaro Aarch64 GNU Linux Binary Toolchain"
PKG_TOOLCHAIN="manual"

makeinstall_host() {
  mkdir -p $TOOLCHAIN/lib/gcc-linaro-aarch64-elf/
    cp -a * $TOOLCHAIN/lib/gcc-linaro-aarch64-elf
}
