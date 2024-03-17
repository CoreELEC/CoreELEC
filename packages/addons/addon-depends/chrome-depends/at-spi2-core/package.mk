# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2017 Escalade
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="at-spi2-core"
PKG_VERSION="2.52.0"
PKG_SHA256="0ac3fc8320c8d01fa147c272ba7fa03806389c6b03d3c406d0823e30e35ff5ab"
PKG_LICENSE="OSS"
PKG_SITE="https://www.gnome.org/"
PKG_URL="https://download.gnome.org/sources/at-spi2-core/${PKG_VERSION:0:4}/at-spi2-core-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain atk dbus glib libxml2"
PKG_LONGDESC="Protocol definitions and daemon for D-Bus at-spi."

configure_package() {
  # Build with x11 support
  if [ ${DISPLAYSERVER} = "x11" ]; then
    PKG_DEPENDS_TARGET+=" libXtst"
  fi
}

pre_configure_target() {
  PKG_MESON_OPTS_TARGET="-Ddocs=false \
                         -Dintrospection=disabled \
                         -Ddbus_daemon=/usr/bin/dbus-daemon"

  if [ ${DISPLAYSERVER} = "x11" ]; then
    PKG_MESON_OPTS_TARGET+="-Dx11=true"
    TARGET_LDFLAGS="${LDFLAGS} -lXext"
  else
    PKG_MESON_OPTS_TARGET+="-Dx11=false"
  fi
}
