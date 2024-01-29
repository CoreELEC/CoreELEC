# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="glm"
PKG_VERSION="1.0.0"
PKG_SHA256="e51f6c89ff33b7cfb19daafb215f293d106cd900f8d681b9b1295312ccadbd23"
PKG_LICENSE="MIT"
PKG_SITE="https://glm.g-truc.net/"
PKG_URL="https://github.com/g-truc/glm/archive/refs/tags/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="OpenGL Mathematics (GLM)"

PKG_CMAKE_OPTS_TARGET="-DGLM_BUILD_TESTS=OFF"

# Not needed by GLM itself, but users will need it. So instead of adding this
# to every user, put it here once.
if [ "${OPENGLES_SUPPORT}" = "yes" ]; then
  PKG_DEPENDS_TARGET+=" ${OPENGLES}"
fi

if [ "${OPENGL_SUPPORT}" = "yes" ]; then
  PKG_DEPENDS_TARGET+=" ${OPENGL}"
fi

# Hack install solution until cmake install restored in upstream package
makeinstall_target() {
  target_has_feature 32bit && PKG_VOID_SIZE=4 || PKG_VOID_SIZE=8

  for _dir in ${SYSROOT_PREFIX} ${INSTALL}; do
    mkdir -p ${_dir}/usr/include ${_dir}/usr/lib/pkgconfig ${_dir}/usr/lib/cmake/glm
      cp -r ${PKG_BUILD}/glm ${_dir}/usr/include
      cp ${PKG_DIR}/config/glm.pc ${_dir}/usr/lib/pkgconfig
      cp ${PKG_DIR}/config/*.cmake ${_dir}/usr/lib/cmake/glm

      sed -e "s/@@VERSION@@/${PKG_VERSION}/g" \
          -e "s/@@VOID_SIZE@@/${PKG_VOID_SIZE}/g" \
          -i ${_dir}/usr/lib/pkgconfig/glm.pc ${_dir}/usr/lib/cmake/glm/*.cmake
  done
}
