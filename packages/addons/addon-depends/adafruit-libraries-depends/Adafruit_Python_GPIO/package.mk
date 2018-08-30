# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="Adafruit_Python_GPIO"
PKG_VERSION="c543d1df9c0a71bafb9f0a1f9dceecd79a920e74"
PKG_SHA256="d3cb74fd033ebe5aea1786a584d64ef5eb8082ef2bf0a568b01691612cd04e88"
PKG_ARCH="any"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/adafruit/${PKG_NAME}"
PKG_URL="https://github.com/adafruit/${PKG_NAME}/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain Python2 distutilscross:host"
PKG_SECTION="python"
PKG_SHORTDESC="Adafruit Python GPIO Library"
PKG_LONGDESC="Library to provide a cross-platform GPIO interface on the Raspberry Pi and Beaglebone Black using the RPi.GPIO and Adafruit_BBIO librarie"
PKG_TOOLCHAIN="manual"
