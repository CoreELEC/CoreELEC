# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)

PKG_NAME="libplist"
PKG_VERSION="2.4.0"
PKG_SHA256="3f5868ae15b117320c1ff5e71be53d29469d4696c4085f89db1975705781a7cd"
PKG_LICENSE="GPL"
PKG_SITE="http://www.libimobiledevice.org/"
PKG_URL="https://github.com/libimobiledevice/libplist/releases/download/${PKG_VERSION}/${PKG_NAME}-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_TARGET="toolchain glib"
PKG_LONGDESC="libplist is a library for manipulating Apple Binary and XML Property Lists"
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_TARGET="--without-cython"

pre_configure_target() {
  # work around bashism in configure script
  export CONFIG_SHELL="/bin/bash"
}

post_configure_target() {
  libtool_remove_rpath libtool
}

post_makeinstall_target() {
  rm -rf ${INSTALL}/usr/bin
}
