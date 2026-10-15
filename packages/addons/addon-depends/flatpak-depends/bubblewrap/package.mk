# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="bubblewrap"
PKG_VERSION="0.13.1"
PKG_SHA256="b5cdc362b4bbe8906318ec68418de1fb050a8c6701534a81c0e94088ad74c22e"
PKG_LICENSE="LGPL-2.0-or-later"
PKG_SITE="https://github.com/containers/bubblewrap"
PKG_URL="https://github.com/containers/bubblewrap/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libcap Python3"
PKG_LONGDESC="Low-level unprivileged sandboxing tool used by Flatpak and similar projects"
PKG_BUILD_FLAGS="-sysroot"

PKG_MESON_OPTS_TARGET="-Dbash_completion=disabled \
                       -Dman=disabled \
                       -Dpython=python3 \
                       -Dselinux=disabled \
                       -Dtests=false \
                       -Dzsh_completion=disabled"
