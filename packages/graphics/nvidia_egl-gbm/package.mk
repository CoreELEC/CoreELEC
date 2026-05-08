# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="nvidia_egl-gbm"
PKG_VERSION="1.1.3"
PKG_SHA256="2669f59a22e1d41d73b02866f3ed35e55b50c6afc70f078bcff97bfef67736ad"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/NVIDIA/egl-gbm"
PKG_URL="${PKG_SITE}/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libdrm eglexternalplatform mesa"
PKG_LONGDESC="NVIDIA GBM EGL external platform library (libnvidia-egl-gbm)"
PKG_TOOLCHAIN="meson"

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/share/egl/egl_external_platform.d
    cp -p ${PKG_BUILD}/src/15_nvidia_gbm.json                                   ${INSTALL}/usr/share/egl/egl_external_platform.d

  mkdir -p ${INSTALL}/usr/lib
    cp -p ${PKG_BUILD}/.${TARGET_NAME}/src/libnvidia-egl-gbm.so.${PKG_VERSION}  ${INSTALL}/usr/lib
    ln -sf libnvidia-egl-gbm.so.${PKG_VERSION}                                  ${INSTALL}/usr/lib/libnvidia-egl-gbm.so.1
    ln -sf libnvidia-egl-gbm.so.1                                               ${INSTALL}/usr/lib/libnvidia-egl-gbm.so
}
