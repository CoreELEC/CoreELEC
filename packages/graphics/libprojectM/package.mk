# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libprojectM"
PKG_VERSION="e6bda8e744301f6395d4b4c8c5c77bfa9eae2598"
PKG_SHA256="8d0206a369f172c7a66eb4acf8291f21a8ac20124a88f2865dc126e211e83a61"
PKG_LICENSE="LGPL-2.1-or-later"
PKG_SITE="https://github.com/garbear/projectm"
PKG_URL="https://github.com/garbear/projectm/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain freetype glm"
PKG_LONGDESC="A MilkDrop compatible opensource music visualizer."
PKG_BUILD_FLAGS="+pic"

PKG_CMAKE_OPTS_TARGET="-DBUILD_SHARED_LIBS=OFF \
                       -DENABLE_SYSTEM_GLM=ON"

if [ "${OPENGL_SUPPORT}" = "yes" ]; then
  PKG_DEPENDS_TARGET+=" ${OPENGL}"
  PKG_CMAKE_OPTS_TARGET+=" -DENABLE_GLES=OFF"
fi

if [ "${OPENGLES_SUPPORT}" = "yes" ]; then
  PKG_DEPENDS_TARGET+=" ${OPENGLES}"
  PKG_CMAKE_OPTS_TARGET+=" -DENABLE_GLES=ON"
fi

pre_configure_target() {
  if [ "${DISPLAYSERVER}" = "no" -a "${OPENGL_SUPPORT}" = "yes" ]; then
    export CFLAGS+=" -DSOIL_EGL"
    export GL_LIBS="-lOpenGL"
  elif [ "${OPENGLES_SUPPORT}" = "yes" ]; then
    export CFLAGS+=" -DSOIL_GLES2"
  fi
}
