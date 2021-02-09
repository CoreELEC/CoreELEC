# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libxml2"
PKG_VERSION="2.9.10"
PKG_LICENSE="MIT"
PKG_SITE="http://xmlsoft.org"
PKG_URL="ftp://xmlsoft.org/libxml2/$PKG_NAME-$PKG_VERSION.tar.gz"
PKG_DEPENDS_HOST="zlib:host Python2:host"
PKG_DEPENDS_TARGET="toolchain zlib Python2"
PKG_LONGDESC="The libxml package contains an XML library, which allows you to manipulate XML files."

PKG_CONFIGURE_OPTS_ALL="ac_cv_header_ansidecl_h=no \
             --enable-static \
             --enable-shared \
             --disable-silent-rules \
             --enable-ipv6 \
             --without-lzma"

PKG_CONFIGURE_OPTS_HOST="${PKG_CONFIGURE_OPTS_ALL} --with-zlib=${TOOLCHAIN} --with-python=${TOOLCHAIN} --with-sysroot=${TOOLCHAIN} --enable-static --enable-shared"

PKG_CONFIGURE_OPTS_TARGET="${PKG_CONFIGURE_OPTS_ALL} --with-zlib=${SYSROOT_PREFIX}/usr --with-python=${SYSROOT_PREFIX} --with-sysroot=${SYSROOT_PREFIX}"

post_makeinstall_target() {
  sed -e "s:\(['= ]\)/usr:\\1$SYSROOT_PREFIX/usr:g" -i $SYSROOT_PREFIX/usr/bin/xml2-config

  rm -rf $INSTALL/usr/bin
  rm -rf $INSTALL/usr/lib/xml2Conf.sh
}
