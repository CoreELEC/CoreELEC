# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="procps-ng"
PKG_VERSION="4.0.6"
PKG_SHA256="144410bd111330b191d4384ba6e8b4861390c644b7188e9487f779116b35a33c"
PKG_LICENSE="GPL-2.0-or-later AND LGPL-2.1-or-later"
PKG_SITE="https://gitlab.com/procps-ng/procps"
PKG_URL="https://gitlab.com/procps-ng/procps/-/archive/v${PKG_VERSION}/procps-v${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_TARGET="toolchain ncurses"
PKG_LONGDESC="Command line and full screen utilities for browsing procfs."
PKG_TOOLCHAIN="autotools"
PKG_BUILD_FLAGS="+pic"

PKG_CONFIGURE_OPTS_TARGET="ac_cv_func_malloc_0_nonnull=yes \
                           ac_cv_func_realloc_0_nonnull=yes \
                           --disable-shared \
                           --disable-modern-top \
                           --enable-static"

PKG_MAKE_OPTS_TARGET="src/free src/top/top library/libproc2.la library/libproc2.pc"

PKG_MAKEINSTALL_OPTS_TARGET="install-libLTLIBRARIES install-pkgconfigDATA install-library_libproc2_la_includeHEADERS"

pre_configure_target() {
  sed -i -e "s/UNKNOWN/${PKG_VERSION}/" ../configure
}

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/bin
    cp -P ${PKG_BUILD}/.${TARGET_NAME}/src/free ${INSTALL}/usr/bin
    cp -P ${PKG_BUILD}/.${TARGET_NAME}/src/top/top ${INSTALL}/usr/bin

  make DESTDIR=${SYSROOT_PREFIX} -j1 ${PKG_MAKEINSTALL_OPTS_TARGET}

  sed 's@proc/misc.h@procps/misc.h@' \
    ${PKG_BUILD}/library/include/readproc.h \
    >${SYSROOT_PREFIX}/usr/include/libproc2/readproc.h
}
