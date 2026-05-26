# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="m4"
PKG_VERSION="1.4.21"
PKG_SHA256="dc487e11d2f0c9e01555bb1af26be4eae983ec8f0726746505b4327186eb21fc"
PKG_LICENSE="GPL-3.0-or-later"
PKG_SITE="http://www.gnu.org/software/m4/"
PKG_URL="https://ftpmirror.gnu.org/m4/${PKG_NAME}-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_HOST="ccache:host"
PKG_LONGDESC="The m4 macro processor."
PKG_BUILD_FLAGS="-cfg-libs:host"

PKG_CONFIGURE_OPTS_HOST="gl_cv_func_gettimeofday_clobber=no --target=${TARGET_NAME}"

post_makeinstall_host() {
  make prefix=${SYSROOT_PREFIX}/usr install
}
