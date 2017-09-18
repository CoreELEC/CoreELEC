# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pygobject"
PKG_VERSION="3.28.3"
PKG_SHA256="c1322f0c9079975bd430906ff3bf13d7f85df85e3fd43c8356f1254c083b0ecd"
PKG_ARCH="any"
PKG_LICENSE="LGPL"
PKG_SITE="https://github.com/GNOME/pygobject"
PKG_URL="https://github.com/GNOME/pygobject/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain Python3 libffi gobject-introspection pgi"
PKG_SECTION="python/devel"
PKG_SHORTDESC="This archive contains bindings for the GLib, and GObject, to be used in Python."
PKG_LONGDESC="This archive contains bindings for the GLib, and GObject, to be used in Python."
PKG_TOOLCHAIN="autotools"
PKG_IS_ADDON="no"

PKG_CONFIGURE_OPTS_TARGET="--disable-cairo \
                           --enable-shared \
                           --with-python=$TOOLCHAIN/bin/${PKG_PYTHON_VERSION}"

pre_configure_target() {
  export PYTHON_INCLUDES="$($SYSROOT_PREFIX/usr/bin/python3-config --includes)"
}

post_unpack() {
  sed -i "s|@CODE_COVERAGE_RULES@||" $PKG_BUILD/Makefile.am
}

post_makeinstall_target() {
  python_remove_source

  rm -rf $INSTALL/usr/bin
  rm -rf $INSTALL/usr/share/pygobject
}
