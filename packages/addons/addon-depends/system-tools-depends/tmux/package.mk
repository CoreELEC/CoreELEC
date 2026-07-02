# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2025-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="tmux"
PKG_VERSION="3.7b"
PKG_SHA256="87f2e99e3b685973f2ca002ffd6ed7e51a5744f7009daae5a15670b6d532db96"
PKG_LICENSE="ISC"
PKG_SITE="https://github.com/tmux/tmux/wiki"
PKG_URL="https://github.com/tmux/tmux/releases/download/${PKG_VERSION}/${PKG_NAME}-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libevent ncurses"
PKG_LONGDESC="tmux is a terminal multiplexer"
PKG_BUILD_FLAGS="-sysroot -cfg-libs"
