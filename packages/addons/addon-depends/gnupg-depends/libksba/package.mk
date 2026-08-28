# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libksba"
PKG_VERSION="1.8.1"
PKG_SHA256="c2f84393011827219ae117131dba8e7684c2bed0961eed11b0642c2acba440b5"
PKG_LICENSE="GPL-2.0-or-later OR LGPL-3.0-or-later"
PKG_SITE="https://www.gnupg.org"
PKG_URL="https://www.gnupg.org/ftp/gcrypt/libksba/libksba-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_TARGET="toolchain libgpg-error"
PKG_LONGDESC="A library to work with X.509 certificates"
PKG_BUILD_FLAGS="-sysroot"

PKG_CONFIGURE_OPTS_TARGET="--disable-doc \
                           --with-libgpg-error-prefix=${SYSROOT_PREFIX}/usr"
