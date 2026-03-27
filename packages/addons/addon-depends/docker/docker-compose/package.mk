# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2025-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="docker-compose"
PKG_VERSION="5.1.1"
PKG_LICENSE="Apache-2.0"
PKG_SITE="https://github.com/docker/compose"
PKG_LONGDESC="Define and run multi-container applications with Docker."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="4b5c42952b7dd81f508d01a771df2a9e5dbffe9b8c5c7d983e738504ad38f056"
    PKG_URL="${PKG_SITE}/releases/download/v${PKG_VERSION}/docker-compose-linux-aarch64"
    ;;
  "arm")
    PKG_SHA256="5cf43b83c705b24df9dbee1d35a6f085189ee2c1169444147192daf932683ed4"
    PKG_URL="${PKG_SITE}/releases/download/v${PKG_VERSION}/docker-compose-linux-armv7"
    ;;
  "x86_64")
    PKG_SHA256="2ac954c9d506b912a12477d72f01601dc72ec918c429c7bae48fd707bdf0f3e5"
    PKG_URL="${PKG_SITE}/releases/download/v${PKG_VERSION}/docker-compose-linux-x86_64"
    ;;
esac

PKG_SOURCE_NAME="docker-compose-linux-${ARCH}-${PKG_VERSION}"

unpack() {
  mkdir -p ${PKG_BUILD}
    cp -P ${SOURCES}/${PKG_NAME}/${PKG_SOURCE_NAME} ${PKG_BUILD}/docker-compose
    chmod +x ${PKG_BUILD}/docker-compose
}
