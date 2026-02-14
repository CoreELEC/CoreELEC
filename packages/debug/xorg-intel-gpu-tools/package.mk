# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="xorg-intel-gpu-tools"
PKG_VERSION="2.3"
PKG_SHA256="429f2578fb16cff7402e9beb325c5c267ad30a2b7b957343c77e8ea24af03887"
PKG_LICENSE="GPL"
PKG_DEPENDS_TARGET="toolchain cairo elfutils kmod libdrm procps-ng systemd"
PKG_SITE="https://gitlab.freedesktop.org/drm/igt-gpu-tools"
PKG_URL="https://www.x.org/releases/individual/app/igt-gpu-tools-${PKG_VERSION}.tar.xz"
PKG_LONGDESC="Test suite and tools for DRM/KMS drivers"
PKG_BUILD_FLAGS="-ndebug"

PKG_MESON_OPTS_TARGET="-Dchamelium=disabled \
                       -Ddocs=disabled \
                       -Dlibdrm_drivers=auto \
                       -Dlibunwind=disabled \
                       -Dman=disabled \
                       -Doverlay=disabled \
                       -Drunner=disabled \
                       -Dtests=enabled \
                       -Dvalgrind=disabled"
