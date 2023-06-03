# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="rustc-snapshot"
PKG_VERSION="$(get_pkg_version rust)"
PKG_SHA256="7d891d3e9bc4f1151545c83cbe3bc6af9ed234388c45ca2e19641262f48615e2"
PKG_LICENSE="MIT"
PKG_SITE="https://www.rust-lang.org"
PKG_URL="https://static.rust-lang.org/dist/rustc-${PKG_VERSION}-${MACHINE_HARDWARE_NAME}-unknown-linux-gnu.tar.xz"
PKG_LONGDESC="rustc bootstrap compiler"
PKG_TOOLCHAIN="manual"
