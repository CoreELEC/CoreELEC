# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)

PKG_NAME="JsonSchemaBuilder"
PKG_VERSION="0"
PKG_LICENSE="GPL"
PKG_SITE="http://www.kodi.tv"
PKG_URL=""
PKG_NEED_UNPACK="$(get_pkg_directory $MEDIACENTER)"
PKG_LONGDESC="kodi-platform:"

PKG_CMAKE_SCRIPT="$(get_build_dir $MEDIACENTER)/tools/depends/native/JsonSchemaBuilder/CMakeLists.txt"

PKG_CMAKE_OPTS_HOST="-Wno-dev"

makeinstall_host() {
  mkdir -p $TOOLCHAIN/bin
    cp JsonSchemaBuilder $TOOLCHAIN/bin
}
