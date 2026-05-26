# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="tinyxml2"
PKG_VERSION="11.0.0"
PKG_SHA256="5556deb5081fb246ee92afae73efd943c889cef0cafea92b0b82422d6a18f289"
PKG_LICENSE="Zlib"
PKG_SITE="https://leethomason.github.io/tinyxml2/"
PKG_URL="https://github.com/leethomason/tinyxml2/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="TinyXML2 is a simple, small, C++ XML parser."
PKG_TOOLCHAIN="cmake"
PKG_BUILD_FLAGS="+pic"

PKG_CMAKE_OPTS_TARGET="-DBUILD_SHARED_LIBS=OFF \
                       -DBUILD_TESTING=OFF"
