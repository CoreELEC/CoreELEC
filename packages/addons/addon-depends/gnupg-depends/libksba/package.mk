# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libksba"
PKG_VERSION="1.6.8"
PKG_SHA256="0f4510f1c7a679c3545990a31479f391ad45d84e039176309d42f80cf41743f5"
PKG_LICENSE="GPL-3.0"
PKG_SITE="https://www.gnupg.org"
PKG_URL="https://www.gnupg.org/ftp/gcrypt/libksba/libksba-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_TARGET="toolchain libgpg-error"
PKG_LONGDESC="A library to work with X.509 certificates"
PKG_BUILD_FLAGS="-sysroot"

PKG_CONFIGURE_OPTS_TARGET="--disable-doc \
                           --with-libgpg-error-prefix=${SYSROOT_PREFIX}/usr"
