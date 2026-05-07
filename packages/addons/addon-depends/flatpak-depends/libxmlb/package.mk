# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libxmlb"
PKG_VERSION="0.3.25"
PKG_SHA256="1b4c42ea390d3c5358764645667f92bde3e82e5b777c0415f6c4eec21273dbdf"
PKG_LICENSE="LGPL-2.1"
PKG_SITE="https://github.com/hughsie/libxmlb"
PKG_URL="https://github.com/hughsie/libxmlb/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="toolchain:host glib:host"
PKG_DEPENDS_TARGET="toolchain glib Python3 xz zstd"
PKG_DEPENDS_CONFIG="xz"
PKG_LONGDESC="A library to help create and query binary XML blobs"
PKG_BUILD_FLAGS="+pic:host +pic -sysroot"

PKG_MESON_OPTS_HOST="-Ddefault_library=static \
                     -Dgtkdoc=false \
                     -Dintrospection=false \
                     -Dtests=false \
                     -Dstemmer=false \
                     -Dlzma=disabled \
                     -Dzstd=disabled"

PKG_MESON_OPTS_TARGET="-Ddefault_library=static \
                       -Dgtkdoc=false \
                       -Dintrospection=false \
                       -Dtests=false \
                       -Dstemmer=false \
                       -Dlzma=enabled \
                       -Dzstd=enabled"
