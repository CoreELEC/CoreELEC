# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libxml2"
PKG_VERSION="2.15.3"
PKG_SHA256="635685ed77c202daf4d20e1427f70ba75c15b8ed8093afec581c8a8022763202"
PKG_LICENSE="MIT"
PKG_SITE="http://xmlsoft.org"
PKG_URL="https://gitlab.gnome.org/GNOME/${PKG_NAME}/-/archive/v${PKG_VERSION}/${PKG_NAME}-v${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_HOST="zlib:host ninja:host Python3:host"
PKG_DEPENDS_TARGET="toolchain zlib"
PKG_LONGDESC="The libxml package contains an XML library, which allows you to manipulate XML files."
PKG_TOOLCHAIN="cmake"

PKG_CMAKE_OPTS_ALL="-DBUILD_SHARED_LIBS=ON \
                    -DLIBXML2_WITH_ICONV=OFF \
                    -DLIBXML2_WITH_ICU=OFF \
                    -DLIBXML2_WITH_TESTS=OFF \
                    -DLIBXML2_WITH_THREADS=ON \
                    -DLIBXML2_WITH_PYTHON=OFF \
                    -DLIBXML2_WITH_ZLIB=OFF"

PKG_CMAKE_OPTS_HOST="${PKG_CMAKE_OPTS_ALL}"

PKG_CMAKE_OPTS_TARGET="${PKG_CMAKE_OPTS_ALL}"

post_makeinstall_target() {
  sed -e "s:\(['= ]\)/usr:\\1${SYSROOT_PREFIX}/usr:g" -i ${SYSROOT_PREFIX}/usr/bin/xml2-config

  rm -rf ${INSTALL}/usr/bin
  rm -rf ${INSTALL}/usr/lib/xml2Conf.sh
}
