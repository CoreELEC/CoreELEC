# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="xkeyboard-config"
PKG_VERSION="2.48"
PKG_SHA256="b77041324f0109f77161ee43743fe04baa485866af8460d31e476ad3f7648fd5"
PKG_LICENSE="MIT AND HPND AND HPND-sell-variant"
PKG_SITE="https://www.X.org"
PKG_URL="https://www.x.org/releases/individual/data/${PKG_NAME}/${PKG_NAME}-${PKG_VERSION}.tar.xz"
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
