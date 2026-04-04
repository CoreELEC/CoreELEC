# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libplacebo"
PKG_VERSION="7.360.0"
PKG_SHA256="c4c70d430083915a157cb790660749649f58fee1e946c0a6e554fd1cba5a2e8f"
PKG_LICENSE="LGPLv2.1"
PKG_SITE="https://code.videolan.org/videolan/libplacebo"
PKG_URL="https://github.com/haasn/libplacebo/archive/refs/tags/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain glad:host Jinja2:host"
PKG_DEPENDS_UNPACK="vulkan-headers"
PKG_LONGDESC="Reusable library for GPU-accelerated image/video processing primitives and shaders"
PKG_BUILD_FLAGS="-sysroot"

PKG_MESON_OPTS_TARGET="-Ddefault_library=static \
                       -Dprefer_static=true \
                       -Dvulkan=disabled \
                       -Dvk-proc-addr=disabled \
                       -Dd3d11=disabled \
                       -Dglslang=disabled \
                       -Dshaderc=disabled \
                       -Dlcms=disabled \
                       -Ddovi=disabled \
                       -Dlibdovi=disabled \
                       -Ddemos=false"

if [ "${OPENGL_SUPPORT}" = "yes" ]; then
  PKG_DEPENDS_TARGET+=" ${OPENGL}"
  PKG_MESON_OPTS_TARGET+=" -Dopengl=enabled -Dgl-proc-addr=enabled"
else
  PKG_MESON_OPTS_TARGET+=" -Dopengl=disabled -Dgl-proc-addr=disabled"
fi

pre_configure_target() {
  export TARGET_CFLAGS+=" -I$(get_build_dir vulkan-headers)/include"
}

post_makeinstall_target() {
  sed 's/^Libs:.*-lplacebo/& -lstdc++/' -i ${INSTALL}/usr/lib/pkgconfig/libplacebo.pc
}
