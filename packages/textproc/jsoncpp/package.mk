# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="jsoncpp"
PKG_VERSION="1.9.8"
PKG_SHA256="51828cf3574281d2b79ec2a1c56a9e4c20cc1103711321ea96384cffb8d2d904"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/open-source-parsers/jsoncpp/"
PKG_URL="https://github.com/open-source-parsers/jsoncpp/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="A C++ library for interacting with JSON."
PKG_BUILD_FLAGS="+pic"

PKG_MESON_OPTS_TARGET="-Dtests=false \
                       --default-library static"
