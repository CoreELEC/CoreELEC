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

PKG_NAME="imagemagick"
PKG_VERSION="7.0.7-32"
PKG_SHA256="f1785adf8bbf378b47e789c74c2fd9ebdd5ec1c4de12e53306f8f6eb5b55d656"
PKG_ARCH="any"
PKG_LICENSE="http://www.imagemagick.org/script/license.php"
PKG_SITE="http://www.imagemagick.org/"
PKG_URL="https://github.com/ImageMagick/ImageMagick/archive/$PKG_VERSION.tar.gz"
PKG_SOURCE_DIR="ImageMagick-$PKG_VERSION"
PKG_DEPENDS_TARGET="toolchain libX11"
PKG_SECTION="graphics"
PKG_LONGDESC="Software suite to create, edit, compose, or convert bitmap images"

PKG_CONFIGURE_OPTS_TARGET="--enable-static \
                           --enable-shared \
                           --with-quantum-depth=8 \
                           --enable-hdri=no \
                           --disable-openmp"

makeinstall_target() {
  make install DESTDIR=$INSTALL
}
