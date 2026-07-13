# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="nqptp"
PKG_VERSION="1.2.8"
PKG_SHA256="3a2882a299c21605f53bb215ce537f9cc7a1e894476f639ab28562c68fd183a9"
PKG_REV="0"
PKG_LICENSE="GPL-2.0-only"
PKG_SITE="https://github.com/mikebrady/nqptp"
PKG_URL="https://github.com/mikebrady/nqptp/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_SECTION="service"
PKG_SHORTDESC="Not Quite PTP"
PKG_LONGDESC="Not Quite PTP (NQPTP) is a daemon that provides the PTP clock timing information required by Shairport Sync for AirPlay 2 audio synchronisation."
PKG_TOOLCHAIN="autotools"
PKG_BUILD_FLAGS="-sysroot -cfg-libs"

PKG_IS_ADDON="yes"
PKG_ADDON_NAME="nqptp"
PKG_ADDON_TYPE="xbmc.service"

PKG_CONFIGURE_OPTS_TARGET="--with-systemd-startup"

addon() {
  mkdir -p "${ADDON_BUILD}/${PKG_ADDON_ID}/bin"
  cp ${PKG_INSTALL}/usr/bin/nqptp \
     "${ADDON_BUILD}/${PKG_ADDON_ID}/bin"
}
