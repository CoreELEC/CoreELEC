# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="xorg-intel-gpu-tools"
PKG_VERSION="1.29"
PKG_SHA256="e0fe0d0f088cb50090b212f28b8960fb1e6160c681f5bea0654973aaa9909d8f"
PKG_LICENSE="GPL"
PKG_DEPENDS_TARGET="toolchain cairo elfutils kmod libdrm procps-ng systemd"
PKG_SITE="https://gitlab.freedesktop.org/drm/igt-gpu-tools"
PKG_URL="https://www.x.org/releases/individual/app/igt-gpu-tools-${PKG_VERSION}.tar.xz"
PKG_LONGDESC="Test suite and tools for DRM/KMS drivers"

PKG_MESON_OPTS_TARGET="-Dchamelium=disabled \
                       -Ddocs=disabled \
                       -Dlibdrm_drivers=auto \
                       -Dlibunwind=disabled \
                       -Dman=disabled \
                       -Doverlay=disabled \
                       -Drunner=disabled \
                       -Dtests=enabled \
                       -Dvalgrind=disabled"

pre_configure_target() {
  # xorg-intel-gpu-tools does not build with NDEBUG (requires assert for tests)
  export TARGET_CFLAGS=$(echo ${TARGET_CFLAGS} | sed -e "s|-DNDEBUG||g")
}
