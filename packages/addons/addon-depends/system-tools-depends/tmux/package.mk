# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2025-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="tmux"
PKG_VERSION="3.6b"
PKG_SHA256="390759d25fdba016887ec982b808927e637070fd7d03a8021f8ef3102b9ae3c7"
PKG_LICENSE="ISC"
PKG_SITE="https://github.com/tmux/tmux/wiki"
PKG_URL="https://github.com/tmux/tmux/releases/download/${PKG_VERSION}/${PKG_NAME}-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libevent ncurses"
PKG_LONGDESC="tmux is a terminal multiplexer"
PKG_BUILD_FLAGS="-sysroot -cfg-libs"
