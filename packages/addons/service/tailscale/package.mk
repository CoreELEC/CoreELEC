# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="tailscale"
PKG_VERSION="1.104.1"
PKG_REV="10"
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
    PKG_SHA256="108d1d96ecf410d305571e173516f27038f870919a9b89c914a167c6d33a4528"
    TAILSCALE_ARCH="amd64"
    ;;
  "arm")
    PKG_SHA256="aea7ae225989d302444b10b44aa42e89fdfa2a691b4d124ed710b4753518a875"
    TAILSCALE_ARCH="arm"
    ;;
  "aarch64")
    PKG_SHA256="f60294374967f3dfd8cf57bbbd474d6cfce32d123e3a8ddeab82a87940daa806"
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
