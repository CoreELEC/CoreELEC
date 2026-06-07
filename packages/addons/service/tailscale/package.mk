# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="tailscale"
PKG_VERSION="1.98.4"
PKG_REV="3"
PKG_ARCH="any"
PKG_LICENSE="BSD-3-Clause"
PKG_SITE="https://tailscale.com"
PKG_DEPENDS_TARGET="toolchain"
PKG_SECTION="service"
PKG_SHORTDESC="Tailscale: private WireGuard made easy"
PKG_LONGDESC="Tailscale (${PKG_VERSION}) creates a secure network between your devices using WireGuard. Connect to your tailnet, use exit nodes, and access local subnets."
PKG_TOOLCHAIN="manual"

PKG_IS_ADDON="yes"
PKG_ADDON_NAME="Tailscale"
PKG_ADDON_ICON_SIZE="170"
PKG_ADDON_TYPE="xbmc.service"

case "${ARCH}" in
  "x86_64")
    PKG_SHA256="e6c08a8ee7e63e69aaf1b62ecd12672b3883fbcd2a176bf6cfa42a15fdce0b6b"
    TAILSCALE_ARCH="amd64"
    ;;
  "arm")
    PKG_SHA256="18d4568fe5c72ac31fdac4a8af233770a0357673e6a32f315d04eb0453f495bd"
    TAILSCALE_ARCH="arm"
    ;;
  "aarch64")
    PKG_SHA256="3cb068eb1368b6bb218d0ef0aa0a7a679a7156b7c979e2279cc2c2321b5f05c7"
    TAILSCALE_ARCH="arm64"
    ;;
esac

PKG_URL="https://pkgs.tailscale.com/stable/tailscale_${PKG_VERSION}_${TAILSCALE_ARCH}.tgz"

unpack() {
  mkdir -p ${PKG_BUILD}
    tar xzf ${SOURCES}/${PKG_NAME}/${PKG_NAME}-${PKG_VERSION}.tgz -C ${PKG_BUILD} --strip-components=1
}

make_target() {
  :
}

addon() {
  mkdir -p ${ADDON_BUILD}/${PKG_ADDON_ID}/bin
    cp ${PKG_BUILD}/tailscale ${ADDON_BUILD}/${PKG_ADDON_ID}/bin
    cp ${PKG_BUILD}/tailscaled ${ADDON_BUILD}/${PKG_ADDON_ID}/bin
}
