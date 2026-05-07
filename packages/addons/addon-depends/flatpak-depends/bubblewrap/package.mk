# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="bubblewrap"
PKG_VERSION="0.11.1"
PKG_SHA256="fb6ebf0264dfe9fb88777d352deeedf5aecf2e36e78da148157036b647f86e0f"
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

