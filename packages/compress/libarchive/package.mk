# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libarchive"
PKG_VERSION="3.7.4"
PKG_SHA256="f887755c434a736a609cbd28d87ddbfbe9d6a3bb5b703c22c02f6af80a802735"
PKG_LICENSE="GPL"
PKG_SITE="https://www.libarchive.org"
PKG_URL="https://www.libarchive.org/downloads/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_HOST="cmake:host ninja:host"
PKG_DEPENDS_TARGET="cmake:host gcc:host"
PKG_SHORTDESC="A multi-format archive and compression library."

PKG_CMAKE_OPTS_TARGET="-DCMAKE_POSITION_INDEPENDENT_CODE=1 \
                       -DENABLE_EXPAT=0 \
                       -DENABLE_ICONV=0 \
                       -DENABLE_LIBXML2=0 \
                       -DENABLE_LZO=1 \
                       -DENABLE_TEST=0 \
                       -DENABLE_COVERAGE=0 \
                       -DENABLE_WERROR=0"

post_makeinstall_target() {
  rm -rf ${INSTALL}

  # delete the shared library as we only want static
  rm ${SYSROOT_PREFIX}/usr/lib/libarchive.so*
}
