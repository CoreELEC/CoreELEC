# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2025-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="docker-compose"
PKG_VERSION="5.3.1"
PKG_LICENSE="Apache-2.0"
PKG_SITE="https://github.com/docker/compose"
PKG_LONGDESC="Define and run multi-container applications with Docker."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="aa611e811d0ea25897839c404bfb5bf93ce706dc51c500a4457890f5d0606a86"
    PKG_URL="${PKG_SITE}/releases/download/v${PKG_VERSION}/docker-compose-linux-aarch64"
    ;;
  "arm")
    PKG_SHA256="69276acb37ea70023cf85a8180e869c0cfc8cb5a3e672821400aa58dea56e2e8"
    PKG_URL="${PKG_SITE}/releases/download/v${PKG_VERSION}/docker-compose-linux-armv7"
    ;;
  "x86_64")
    PKG_SHA256="f9ebc6ebdb19d769b793c245a736caaeb198c62587f13b25c660c13b4987f959"
    PKG_URL="${PKG_SITE}/releases/download/v${PKG_VERSION}/docker-compose-linux-x86_64"
    ;;
esac

PKG_SOURCE_NAME="docker-compose-linux-${ARCH}-${PKG_VERSION}"

unpack() {
  mkdir -p ${PKG_BUILD}
    cp -P ${SOURCES}/${PKG_NAME}/${PKG_SOURCE_NAME} ${PKG_BUILD}/docker-compose
    chmod +x ${PKG_BUILD}/docker-compose
}
