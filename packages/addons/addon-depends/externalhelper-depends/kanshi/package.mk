# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="kanshi"
PKG_VERSION="1.9.0"
PKG_SHA256="2a61c2765055d29861e3bab639222033358ec547d58782299084bad86f5bd627"
PKG_LICENSE="MIT"
PKG_SITE="https://gitlab.freedesktop.org/emersion/kanshi/"
PKG_URL="https://gitlab.freedesktop.org/emersion/kanshi/-/archive/v1.9.0/kanshi-v1.9.0.tar.gz"
PKG_DEPENDS_TARGET="toolchain libscfg wayland"
PKG_LONGDESC="Dynamic display configuration for wayland"
PKG_DEPENDS_CONFIG="libscfg wayland"
PKG_BUILD_FLAGS="-sysroot"

PKG_MESON_OPTS_TARGET="-Dman-pages=disabled \
                       -Dipc=disabled"
