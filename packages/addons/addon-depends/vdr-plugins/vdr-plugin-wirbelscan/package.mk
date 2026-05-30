# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="vdr-plugin-wirbelscan"
PKG_VERSION="2026.05.15"
PKG_SHA256="5b0c3a8b52b054621ec9fd2319240359a0d91171820baf3e7ded63202a082809"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://www.gen2vdr.de/wirbel/wirbelscan/index2.html"
PKG_URL="https://www.gen2vdr.de/wirbel/wirbelscan/vdr-wirbelscan-${PKG_VERSION}.tgz"
PKG_DEPENDS_TARGET="toolchain vdr librepfunc"
PKG_DEPENDS_CONFIG="librepfunc"
PKG_NEED_UNPACK="$(get_pkg_directory vdr)"
PKG_LONGDESC="Performs a channel scans for DVB-T, DVB-C and DVB-S"
PKG_TOOLCHAIN="manual"
PKG_BUILD_FLAGS="+pic"

make_target() {
  VDR_DIR=$(get_build_dir vdr)
  make VDRDIR=${VDR_DIR} \
    LIBDIR="." \
    LOCALEDIR="./locale"
}
