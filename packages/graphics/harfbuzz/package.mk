# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2012 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="harfbuzz"
PKG_VERSION="8.0.0"
PKG_SHA256="1f98b5e3d06a344fe667d7e8210094ced458791499839bddde98c167ce6a7c79"
PKG_LICENSE="GPL"
PKG_SITE="http://www.freedesktop.org/wiki/Software/HarfBuzz"
PKG_URL="https://github.com/harfbuzz/harfbuzz/releases/download/${PKG_VERSION}/harfbuzz-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain cairo freetype glib"
PKG_LONGDESC="HarfBuzz is an OpenType text shaping engine."

PKG_MESON_OPTS_TARGET="-Dbenchmark=disabled \
                       -Dcairo=enabled \
                       -Ddocs=disabled \
                       -Dfreetype=enabled \
                       -Dglib=enabled \
                       -Dgobject=disabled \
                       -Dgraphite=disabled \
                       -Dicu=disabled \
                       -Dtests=disabled"
