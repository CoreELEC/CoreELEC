# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)
# Copyright (C) 2022-present Team CoreELEC (https://coreelec.org)

PKG_NAME="dbus-python"
PKG_VERSION="1.4.0"
PKG_SHA256="c36b28f10ffcc8f1f798aca973bcc132f91f33eb9b6b8904381b4077766043d5"
PKG_LICENSE="GPL"
PKG_SITE="https://freedesktop.org/wiki/Software/dbus"
PKG_URL="https://dbus.freedesktop.org/releases/dbus-python/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain Python3 dbus dbus-glib"
PKG_LONGDESC="D-BUS is a message bus, used for sending messages between applications."
PKG_BUILD_FLAGS="+lto"
PKG_TOOLCHAIN="autotools"

pre_configure_target() {
  export PKG_CONFIG_PATH="${SYSROOT_PREFIX}/usr/lib/pkgconfig"
  export PYTHON_CONFIG="${SYSROOT_PREFIX}/usr/bin/python3-config"
  export PYTHON_INCLUDES="$(${SYSROOT_PREFIX}/usr/bin/python3-config --includes)"
  export PYTHON_LIBS="$(${SYSROOT_PREFIX}/usr/bin/python3-config --ldflags --embed)"
}

post_makeinstall_target() {
  python_remove_source
}
