# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="Adafruit_Python_PCA9685"
PKG_VERSION="f86db2ca5de379748afd71e379ce2155f47d21e7"
PKG_SHA256="46e11dcfbfaab5756ddeb6ab31740cf8c7419ec1d32be2519f19882e7d86e426"
PKG_ARCH="any"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/adafruit/${PKG_NAME}"
PKG_URL="https://github.com/adafruit/${PKG_NAME}/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain Python2 distutilscross:host"
PKG_SECTION="python"
PKG_SHORTDESC="Adafruit Python PCA9685 Library"
PKG_LONGDESC="Python code to use the PCA9685 PWM servo/LED controller with a Raspberry Pi or BeagleBone black."
PKG_TOOLCHAIN="manual"
