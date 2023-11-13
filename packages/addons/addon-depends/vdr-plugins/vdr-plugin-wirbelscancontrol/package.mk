# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="vdr-plugin-wirbelscancontrol"
PKG_VERSION="0.0.3"
PKG_SHA256="93418d31bb757cccea9f81edd13a3e84ca0cf239c30252afbf0ced68e9ef6bd5"
PKG_LICENSE="GPL"
PKG_SITE="https://www.gen2vdr.de/wirbel/wirbelscancontrol/index2.html"
PKG_URL="https://www.gen2vdr.de/wirbel/wirbelscancontrol/${PKG_NAME/-plugin/}-${PKG_VERSION}.tgz"
PKG_DEPENDS_TARGET="toolchain vdr gettext:host vdr-plugin-wirbelscan"
PKG_NEED_UNPACK="$(get_pkg_directory vdr)"
PKG_LONGDESC="Adds menu entry for wirbelscan at VDR."
PKG_TOOLCHAIN="manual"
PKG_BUILD_FLAGS="+pic"

pre_build_target() {
  WIRBELSCAN_DIR=$(get_build_dir vdr-plugin-wirbelscan)
  ln -sf ${WIRBELSCAN_DIR}/wirbelscan_services.h ${PKG_BUILD}
  ln -sf $(get_build_dir vdr) ${PKG_BUILD}/vdr
}

make_target() {
  VDR_DIR=$(get_build_dir vdr)
  make VDRDIR=${VDR_DIR} \
    INCLUDES="-I." \
    LIBDIR="." \
    LOCALEDIR="./locale" \
    install
}
