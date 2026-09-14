# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="automake"
PKG_VERSION="1.19"
PKG_SHA256="e3e2c2e3abf37898138db5b6c1d1dc35c9160c5978be7947d2c741705251d445"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="http://sources.redhat.com/automake/"
PKG_URL="https://ftp.gnu.org/gnu/automake/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_HOST="ccache:host autoconf:host"
PKG_LONGDESC="A GNU tool for automatically creating Makefiles."
PKG_BUILD_FLAGS="-parallel -cfg-libs:host"

PKG_CONFIGURE_OPTS_HOST="--target=${TARGET_NAME} --disable-silent-rules"

post_makeinstall_host() {
  make prefix=${SYSROOT_PREFIX}/usr install
}
