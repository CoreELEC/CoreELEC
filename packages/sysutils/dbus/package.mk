# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="dbus"
PKG_VERSION="1.15.10"
PKG_SHA256="f700f2f1d0473f11e52f3f3e179f577f31b85419f9ae1972af8c3db0bcfde178"
PKG_LICENSE="GPL"
PKG_SITE="https://dbus.freedesktop.org"
PKG_URL="https://dbus.freedesktop.org/releases/${PKG_NAME}/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="meson:host gcc:host expat systemd"
PKG_LONGDESC="D-Bus is a message bus, used for sending messages between applications."

PKG_MESON_OPTS_TARGET="--libexecdir=/usr/lib/dbus \
                       -Dverbose_mode=false \
                       -Dapparmor=disabled \
                       -Dasserts=false \
                       -Dchecks=true \
                       -Dembedded_tests=false \
                       -Dinstalled_tests=false \
                       -Dmodular_tests=disabled \
                       -Dxml_docs=disabled \
                       -Ddoxygen_docs=disabled \
                       -Dducktype_docs=disabled \
                       -Dx11_autolaunch=disabled \
                       -Dselinux=disabled \
                       -Dlibaudit=disabled \
                       -Dsystemd=enabled \
                       -Duser_session=false \
                       -Dinotify=enabled \
                       -Dvalgrind=disabled \
                       -Ddbus_user=dbus \
                       -Druntime_dir=/run \
                       -Dsystem_socket=/run/dbus/system_bus_socket"

post_makeinstall_target() {
  rm -rf ${INSTALL}/etc/rc.d
  rm -rf ${INSTALL}/usr/lib/dbus-1.0/include
}

post_install() {
  add_user dbus x 81 81 "System message bus" "/" "/bin/sh"
  add_group dbus 81
  add_group netdev 497

  echo "chmod 4750 ${INSTALL}/usr/lib/dbus/dbus-daemon-launch-helper" >>${FAKEROOT_SCRIPT}
  echo "chown 0:81 ${INSTALL}/usr/lib/dbus/dbus-daemon-launch-helper" >>${FAKEROOT_SCRIPT}
}
