# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="tailscale"
PKG_VERSION="1.96.4"
PKG_REV="0"
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
PKG_ADDON_TYPE="xbmc.service"

case "${ARCH}" in
  x86_64)
    TAILSCALE_ARCH="amd64"
    PKG_SHA256="a1cba18826b1f91cb25ef7f5b8259b5258339b42db7867af9269e21829ea78cc"
    ;;
  arm)
    TAILSCALE_ARCH="arm"
    PKG_SHA256="6c8731f096147aaf9e187b39f892692fb2bd56dc2eb1fb8fd06982164f339869"
    ;;
  aarch64)
    TAILSCALE_ARCH="arm64"
    PKG_SHA256="a27249bc70d7b37a68f8be7f5c4507ea5f354e592dce43cb5d4f3e742b313c3c"
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
