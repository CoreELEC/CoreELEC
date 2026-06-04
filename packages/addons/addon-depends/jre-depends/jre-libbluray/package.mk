# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

. $(get_pkg_directory libbluray)/package.mk

PKG_NAME="jre-libbluray"
PKG_DEPENDS_TARGET+=" apache-ant:host"
PKG_LONGDESC="libbluray jar for BD-J menus"
PKG_URL=""
PKG_DEPENDS_UNPACK+=" jdk-${MACHINE_HARDWARE_NAME}-zulu libbluray"
PKG_PATCH_DIRS+=" $(get_pkg_directory libbluray)/patches"
PKG_BUILD_FLAGS="-sysroot"

unpack() {
  mkdir -p ${PKG_BUILD}
  tar --strip-components=1 -xf ${SOURCES}/${PKG_NAME:4}/${PKG_NAME:4}-${PKG_VERSION}.tar.xz -C ${PKG_BUILD}
}

pre_configure_target() {
  export JAVA_HOME="$(get_build_dir jdk-${MACHINE_HARDWARE_NAME}-zulu)"
  export PATH+=":$(get_build_dir jdk-${MACHINE_HARDWARE_NAME}-zulu)/bin"
  PKG_MESON_OPTS_TARGET+=" -Djdk_home=$(get_build_dir jdk-${MACHINE_HARDWARE_NAME}-zulu)"

  # build also jar
  PKG_MESON_OPTS_TARGET="${PKG_MESON_OPTS_TARGET/bdj_jar=disabled/bdj_jar=enabled}"
}
