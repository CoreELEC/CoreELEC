# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2021-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libfreeaptx"
PKG_VERSION="0.2.2"
PKG_SHA256="5ab5ebddf3f2eb7ce47a505b87460b00fc1ede99c70010796d3575ab31ea80bf"
PKG_LICENSE="LGPL-2.1-or-later"
PKG_SITE="https://github.com/iamthehorker/libfreeaptx"
PKG_URL="https://github.com/iamthehorker/libfreeaptx/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Open Source aptX codec library"
PKG_TOOLCHAIN="manual"
PKG_BUILD_FLAGS="+pic"

make_target() {
  ${CC} ${CFLAGS} -I${PKG_BUILD} -c -o ${PKG_NAME##*lib}.o ${PKG_NAME##*lib}.c
  ${AR} -rcs ${PKG_NAME}.a ${PKG_NAME##*lib}.o
}

makeinstall_target() {
  mkdir -p ${SYSROOT_PREFIX}/usr/lib
    cp -a ${PKG_NAME}.a ${SYSROOT_PREFIX}/usr/lib/

  mkdir -p ${SYSROOT_PREFIX}/usr/include
    cp -a ${PKG_NAME##*lib}.h ${SYSROOT_PREFIX}/usr/include/

  mkdir -p ${SYSROOT_PREFIX}/usr/lib/pkgconfig
    cat >${SYSROOT_PREFIX}/usr/lib/pkgconfig/${PKG_NAME}.pc <<EOF
prefix=/usr
exec_prefix=\${prefix}
libdir=\${exec_prefix}/lib
includedir=\${prefix}/include

Name: ${PKG_NAME}
Description: Open Source aptX codec library
Version: ${PKG_VERSION}
Libs: -Wl,-rpath=\${libdir} -L\${libdir} -l${PKG_NAME##*lib}
Cflags: -I\${includedir}
EOF
}
