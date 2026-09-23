# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="bubblewrap"
PKG_VERSION="0.13.0"
PKG_SHA256="caecc52894a20ad9c932af8cb2a2c1bfd251b2d085f16d6c4d0424624c6c3cf9"
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
