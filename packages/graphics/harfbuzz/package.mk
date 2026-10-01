# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2012 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="harfbuzz"
PKG_VERSION="14.5.1"
PKG_SHA256="7e2fa4e8c7c98e8d8140671f5772542afaaa6acccfbd746506886b6d85f7f8d6"
PKG_LICENSE="MIT"
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
