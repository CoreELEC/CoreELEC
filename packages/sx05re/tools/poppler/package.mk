# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="poppler"
PKG_VERSION="0.74.0"
PKG_SHA256="92e09fd3302567fd36146b36bb707db43ce436e8841219025a82ea9fb0076b2f"
PKG_ARCH="any"
PKG_SITE="https://poppler.freedesktop.org"
PKG_URL="https://poppler.freedesktop.org/$PKG_NAME-$PKG_VERSION.tar.xz"
PKG_DEPENDS_TARGET="toolchain cairo"
PKG_LONGDESC="Poppler is a PDF rendering library based on the xpdf-3.0 code base."
PKG_TOOLCHAIN="cmake"

PKG_CMAKE_OPTS_TARGET="-DENABLE_LIBOPENJPEG=none \
                       -DENABLE_GLIB=ON"
