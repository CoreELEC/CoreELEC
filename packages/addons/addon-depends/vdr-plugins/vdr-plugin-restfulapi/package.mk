# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="vdr-plugin-restfulapi"
PKG_VERSION="92762bb5a9d9c7df7ab832aab0737fa12d9805c9"
PKG_SHA256="5992e3d5a2b14b18c0bb5493d791e8f5bba0adb7eb31bba3f930b7dd48478969"
PKG_LICENSE="GPL-2.0-only"
PKG_SITE="https://github.com/yavdr/vdr-plugin-restfulapi"
PKG_URL="https://github.com/yavdr/${PKG_NAME}/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain vdr cxxtools vdr-plugin-wirbelscan"
PKG_NEED_UNPACK="$(get_pkg_directory vdr) $(get_pkg_directory vdr-plugin-wirbelscan)"
PKG_LONGDESC="Allows to access many internals of the VDR via a restful API."
PKG_TOOLCHAIN="manual"

pre_build_target() {
  cp $(get_build_dir vdr-plugin-wirbelscan)/wirbelscan_services.h ${PKG_BUILD}/wirbelscan/
}

make_target() {
  VDR_DIR=$(get_build_dir vdr)
  export PKG_CONFIG_PATH=${VDR_DIR}:${PKG_CONFIG_PATH}
  export CPLUS_INCLUDE_PATH=${VDR_DIR}/include

  make \
    LIBDIR="." \
    LOCDIR="./locale" \
    all install-i18n \
    USE_LIBMAGICKPLUSPLUS=0
}

post_make_target() {
  VDR_DIR=$(get_build_dir vdr)
  VDR_APIVERSION=$(sed -ne '/define APIVERSION/s/^.*"\(.*\)".*$/\1/p' ${VDR_DIR}/config.h)
  LIB_NAME=lib${PKG_NAME/-plugin/}

  cp --remove-destination ${PKG_BUILD}/${LIB_NAME}.so ${PKG_BUILD}/${LIB_NAME}.so.${VDR_APIVERSION}
}
