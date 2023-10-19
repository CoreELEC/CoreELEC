# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libva"
PKG_VERSION="2.20.0"
PKG_SHA256="117f8d658a5fc9ea406ca80a3eb4ae1d70b15a54807c9ed77199c812bed73b60"
PKG_ARCH="x86_64"
PKG_LICENSE="GPL"
PKG_SITE="https://01.org/linuxmedia"
PKG_URL="https://github.com/intel/libva/archive/${PKG_VERSION}.tar.gz"
PKG_LONGDESC="Libva is an implementation for VA-API (VIdeo Acceleration API)."

if [ "${DISPLAYSERVER}" = "x11" ]; then
  PKG_DEPENDS_TARGET="toolchain libX11 libXext libXfixes libdrm"
  DISPLAYSERVER_LIBVA="-Dwith_x11=yes -Dwith_glx=no -Dwith_wayland=no"
elif [ "${DISPLAYSERVER}" = "wl" ]; then
  DISPLAYSERVER_LIBVA="-Dwith_x11=no -Dwith_glx=no -Dwith_wayland=yes"
  PKG_DEPENDS_TARGET="toolchain libdrm wayland"
else
  PKG_DEPENDS_TARGET="toolchain libdrm"
  DISPLAYSERVER_LIBVA="-Dwith_x11=no -Dwith_glx=no -Dwith_wayland=no"
fi

PKG_MESON_OPTS_TARGET="-Ddisable_drm=false \
                       -Denable_docs=false \
                       ${DISPLAYSERVER_LIBVA}"
