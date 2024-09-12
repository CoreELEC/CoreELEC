# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="jsoncpp"
PKG_VERSION="1.9.6"
PKG_SHA256="f93b6dd7ce796b13d02c108bc9f79812245a82e577581c4c9aabe57075c90ea2"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/open-source-parsers/jsoncpp/"
PKG_URL="https://github.com/open-source-parsers/jsoncpp/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="A C++ library for interacting with JSON."
PKG_BUILD_FLAGS="+pic"

PKG_MESON_OPTS_TARGET="-Dtests=false \
                       --default-library static"
