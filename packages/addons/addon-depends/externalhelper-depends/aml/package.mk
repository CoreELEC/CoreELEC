# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aml"
PKG_VERSION="1.0.0"
PKG_SHA256="b2b8f743213af39f40e8bc611147d69e2ea9e010b9b19cb65246582338f28d96"
PKG_LICENSE="ISC"
PKG_SITE="https://github.com/any1/aml"
PKG_URL="https://github.com/any1/aml/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain pkg-config:host"
PKG_SHORTDESC="Another Main Loop libary"

PKG_DEPENDS_CONFIG="wayland wayland-protocols"
