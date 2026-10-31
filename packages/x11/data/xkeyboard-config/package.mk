# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="xkeyboard-config"
PKG_VERSION="2.49"
PKG_SHA256="78daee6a3c4be18b777f2a8d657dcaf7e5d2243e1936ee5a4fd03bacd477cfa4"
PKG_LICENSE="MIT AND HPND AND HPND-sell-variant"
PKG_SITE="https://www.X.org"
PKG_URL="https://gitlab.freedesktop.org/xkeyboard-config/xkeyboard-config/-/archive/${PKG_NAME}-${PKG_VERSION}/${PKG_NAME}-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_TARGET="toolchain util-macros"
PKG_LONGDESC="X keyboard extension data files."

configure_package() {
  if [ "${DISPLAYSERVER}" = "x11" ]; then
    PKG_DEPENDS_TARGET+=" xkbcomp"
  fi
}

pre_configure_target() {
  PKG_MESON_OPTS_TARGET="-Dcompat-rules=true"

  if [ "${DISPLAYSERVER}" = "x11" ]; then
    PKG_MESON_OPTS_TARGET+=" -Dxorg-rules-symlinks=true"
  else
    PKG_MESON_OPTS_TARGET+=" -Dxorg-rules-symlinks=false"
  fi
}
