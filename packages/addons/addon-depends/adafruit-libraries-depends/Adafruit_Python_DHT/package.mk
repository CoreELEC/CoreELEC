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

PKG_NAME="Adafruit_Python_DHT"
PKG_VERSION="a609d7d"
PKG_SHA256="820025c19ce9f3e59fa6d389c860eb567d5a665a08ec45e237839ca214b97615"
PKG_ARCH="any"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/adafruit/${PKG_NAME}"
PKG_URL="https://github.com/adafruit/${PKG_NAME}/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain Python2 distutilscross:host"
PKG_SECTION="python"
PKG_SHORTDESC="Adafruit Python DHT Library"
PKG_LONGDESC="Python library to read the DHT series of humidity and temperature sensors on a Raspberry Pi or Beaglebone Black."
PKG_TOOLCHAIN="manual"

pre_make_target() {
  export PYTHONXCPREFIX="$SYSROOT_PREFIX/usr"
  export LDSHARED="$CC -shared"
}

make_target() {
  case "$PROJECT:$DEVICE" in
    "RPi:RPi")
      PKG_RPI_VERSION="--force-pi"
      ;;
    "RPi:RPi2")
      PKG_RPI_VERSION="--force-pi2"
      ;;
    *)
      PKG_RPI_VERSION=""
  esac

  python setup.py build $PKG_RPI_VERSION --cross-compile
}
