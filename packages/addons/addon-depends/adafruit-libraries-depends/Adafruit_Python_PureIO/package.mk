# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="Adafruit_Python_PureIO"
PKG_VERSION="6f4976d91c52d70b67b28bba75a429b5328a52c1"
PKG_SHA256="891a4d077fe6610de6aa4b0dc5b9933a6c7db3492072df60c0383662f28c2ae9"
PKG_ARCH="any"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/adafruit/${PKG_NAME}"
PKG_URL="https://github.com/adafruit/${PKG_NAME}/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain Python2 distutilscross:host"
PKG_SECTION="python"
PKG_SHORTDESC="Pure python access to Linux IO including I2C and SPI."
PKG_LONGDESC="Pure python access to Linux IO including I2C and SPI. Drop in replacement for smbus and spidev modules."
PKG_TOOLCHAIN="manual"
