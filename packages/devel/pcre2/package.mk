# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pcre2"
PKG_VERSION="10.43"
PKG_SHA256="e2a53984ff0b07dfdb5ae4486bbb9b21cca8e7df2434096cc9bf1b728c350bcb"
PKG_LICENSE="BSD"
PKG_SITE="http://www.pcre.org/"
PKG_URL="https://github.com/PCRE2Project/pcre2/releases/download/pcre2-${PKG_VERSION}/pcre2-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_HOST="toolchain:host"
PKG_DEPENDS_TARGET="toolchain pcre2:host"
PKG_LONGDESC="A set of functions that implement regular expression pattern matching using the same syntax."
PKG_BUILD_FLAGS="+pic"

PKG_CMAKE_OPTS_HOST="-DBUILD_SHARED_LIBS=OFF \
                     -DBUILD_STATIC_LIBS=ON \
                     -DPCRE2_BUILD_PCRE2_8=ON \
                     -DPCRE2_BUILD_PCRE2_16=ON \
                     -DPCRE2_BUILD_PCRE2_32=ON \
                     -DPCRE2_SUPPORT_JIT=ON \
                     -DPCRE2_BUILD_TESTS=OFF \
                     -DPCRE2_SUPPORT_LIBEDIT=OFF \
                     -DPCRE2_SUPPORT_LIBREADLINE=OFF"

PKG_CMAKE_OPTS_TARGET="-DBUILD_SHARED_LIBS=OFF \
                       -DPCRE2_BUILD_PCRE2_16=ON \
                       -DPCRE2_SUPPORT_JIT=ON \
                       -DPCRE2_SUPPORT_LIBEDIT=OFF \
                       -DPCRE2_SUPPORT_LIBREADLINE=OFF"

post_unpack() {
  sed -e 's|^INSTALL(FILES ${man1} DESTINATION man/man1)||' \
      -e 's|^INSTALL(FILES ${man3} DESTINATION man/man3)||' \
      -e 's|^INSTALL(FILES ${html} DESTINATION share/doc/pcre2/html)||' \
      -i ${PKG_BUILD}/CMakeLists.txt
}

post_makeinstall_target() {
  safe_remove ${INSTALL}/usr/bin

  cp ${PKG_NAME}-config ${TOOLCHAIN}/bin
  sed -e "s:\(['= ]\)/usr:\\1${PKG_ORIG_SYSROOT_PREFIX}/usr:g" -i ${TOOLCHAIN}/bin/${PKG_NAME}-config
  chmod +x ${TOOLCHAIN}/bin/${PKG_NAME}-config
}
