# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="Pillow"
PKG_VERSION="11.0.0"
PKG_SHA256="f60959120cac783dc39be7e093bff8f9dcbb5be58bcc1372c57492f748a3b759"
PKG_LICENSE="BSD"
PKG_SITE="https://python-pillow.org/"
PKG_URL="https://github.com/python-pillow/${PKG_NAME}/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain Python3 zlib freetype libjpeg-turbo tiff"
PKG_LONGDESC="The Python Imaging Library adds image processing capabilities to your Python interpreter."
PKG_TOOLCHAIN="python"

PKG_PYTHON_OPTS_TARGET="-C--build-option=build_ext -C--build-option=--disable-platform-guessing"

post_makeinstall_target() {
  python_remove_source

  rm -rf ${INSTALL}/usr/bin
}
