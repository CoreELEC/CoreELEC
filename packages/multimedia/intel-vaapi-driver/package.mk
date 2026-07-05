# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="intel-vaapi-driver"
PKG_VERSION="2.4.5"
PKG_SHA256="f89f77bc46ec6f46392c1b90b9bf34d09f908bf33c9d16d385897b1fe282bacc"
PKG_ARCH="x86_64"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/irql-notlessorequal/intel-vaapi-driver"
PKG_URL="https://github.com/irql-notlessorequal/intel-vaapi-driver/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libva libdrm"
PKG_LONGDESC="intel-vaapi-driver: VA-API user mode driver for Intel GEN Graphics family"

if [ "${DISPLAYSERVER}" = "x11" ]; then
  DISPLAYSERVER_LIBVA="-Dwith_x11=yes -Dwith_wayland_drm=no"
elif [ "${DISPLAYSERVER}" = "wl" ]; then
  DISPLAYSERVER_LIBVA="-Dwith_x11=no -Dwith_wayland_drm=yes"
else
  DISPLAYSERVER_LIBVA="-Dwith_x11=no -Dwith_wayland_drm=no"
fi

PKG_MESON_OPTS_TARGET="-Denable_hybrid_codec=false \
                       -Denable_tests=false \
                       ${DISPLAYSERVER_LIBVA}"
