# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2025-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="docker-compose"
PKG_VERSION="5.1.4"
PKG_LICENSE="Apache-2.0"
PKG_SITE="https://github.com/docker/compose"
PKG_LONGDESC="Define and run multi-container applications with Docker."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="d4fb48b72857810314d3ee77123c89954101844efa4788031221f4c370495946"
    PKG_URL="${PKG_SITE}/releases/download/v${PKG_VERSION}/docker-compose-linux-aarch64"
    ;;
  "arm")
    PKG_SHA256="5cce4229012b8b18067fba078c9ec4e2a5dd47cb4cb3a0cc3d431f6fc429060f"
    PKG_URL="${PKG_SITE}/releases/download/v${PKG_VERSION}/docker-compose-linux-armv7"
    ;;
  "x86_64")
    PKG_SHA256="33b208d7e76639db742fae84b966cc01dacae58ca3fc4dabbc907045aefdf0c4"
    PKG_URL="${PKG_SITE}/releases/download/v${PKG_VERSION}/docker-compose-linux-x86_64"
    ;;
esac

PKG_SOURCE_NAME="docker-compose-linux-${ARCH}-${PKG_VERSION}"

unpack() {
  mkdir -p ${PKG_BUILD}
    cp -P ${SOURCES}/${PKG_NAME}/${PKG_SOURCE_NAME} ${PKG_BUILD}/docker-compose
    chmod +x ${PKG_BUILD}/docker-compose
}
