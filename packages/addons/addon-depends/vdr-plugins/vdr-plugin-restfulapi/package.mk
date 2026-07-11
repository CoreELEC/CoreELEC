# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="vdr-plugin-restfulapi"
PKG_VERSION="54c9124ca31d859f8af1704316cab65d1e398533"
PKG_SHA256="dd9148ff3799306ea5cdec9a9b409a21c5b07032b4e73a326d4c5c6f7c691e62"
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

  local RESTFULAPI_PATH="/storage/.kodi/addons/service.multimedia.vdr-addon/res/plugins/restfulapi"
  CXXFLAGS+=" -DDOCUMENT_ROOT=\\\"${RESTFULAPI_PATH}/\\\" -DWEBAPP_DIR=\\\"${RESTFULAPI_PATH}\\\""

  make \
    LIBDIR="." \
    LOCDIR="./locale" \
    all install-i18n \
    USE_LIBMAGICKPLUSPLUS=0 \
    CXXFLAGS="$(pkg-config --cflags vdr) ${CXXFLAGS}"
}

post_make_target() {
  VDR_DIR=$(get_build_dir vdr)
  VDR_APIVERSION=$(sed -ne '/define APIVERSION/s/^.*"\(.*\)".*$/\1/p' ${VDR_DIR}/config.h)
  LIB_NAME=lib${PKG_NAME/-plugin/}

  cp --remove-destination ${PKG_BUILD}/${LIB_NAME}.so ${PKG_BUILD}/${LIB_NAME}.so.${VDR_APIVERSION}
}
