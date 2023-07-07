# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libprojectM"
PKG_VERSION="3.1.12"
PKG_SHA256="62b5b1b543b25cb8ad392d879378cfdc5c129165cf4d4f33fb159e364d42f135"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/projectM-visualizer/projectm"
PKG_URL="https://github.com/projectM-visualizer/projectm/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain freetype glm"
PKG_LONGDESC="A MilkDrop compatible opensource music visualizer."
PKG_TOOLCHAIN="configure"
PKG_BUILD_FLAGS="+pic"

PKG_CONFIGURE_OPTS_TARGET="--disable-shared \
                           --enable-static \
                           --disable-qt \
                           --disable-pulseaudio \
                           --disable-jack \
                           --disable-sdl \
                           --disable-llvm \
                           --disable-emscripten \
                           --enable-threading \
                           --enable-preset-subdirs"

if [ "${OPENGL_SUPPORT}" = "yes" ]; then
  PKG_DEPENDS_TARGET+=" ${OPENGL}"
  PKG_CONFIGURE_OPTS_TARGET+=" --disable-gles \
                               --enable-gl"
fi

if [ "${OPENGLES_SUPPORT}" = "yes" ]; then
  PKG_DEPENDS_TARGET+=" ${OPENGLES}"
  PKG_CONFIGURE_OPTS_TARGET+=" --enable-gles \
                               --disable-gl"
fi

# workaround due broken release files, remove at next bump
pre_configure_target() {
  ./autogen.sh

  if [ "${OPENGLES_SUPPORT}" = "yes" ]; then
    export CFLAGS+=" -DSOIL_GLES2"
  fi
}
