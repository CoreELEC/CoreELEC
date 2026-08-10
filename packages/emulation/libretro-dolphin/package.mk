# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libretro-dolphin"
PKG_VERSION="ed70219e8bf86717d505b56a830d14ee7e7acce5"
PKG_SHA256="d83b8d5b83f4a870a0ff0ff8430274b89269c7f2c7a4e806afc85bab5b605256"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://github.com/libretro/dolphin"
PKG_URL="${DISTRO_SRC}/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain systemd bzip2 curl glslang spirv-tools libusb lz4 pugixml zlib zstd lzo xz libiconv"
PKG_LONGDESC="Dolphin is a GameCube and Wii emulator."
PKG_TOOLCHAIN="cmake"

PKG_LIBNAME="dolphin_libretro.so"
PKG_LIBPATH="${PKG_LIBNAME}"
PKG_LIBVAR="DOLPHIN_LIB"

PKG_CMAKE_OPTS_TARGET="-DLIBRETRO=ON \
                       -DCMAKE_BUILD_TYPE=Release \
                       -DENABLE_QT=OFF \
                       -DENABLE_SDL=OFF \
                       -DENABLE_TESTS=OFF \
                       -DENABLE_TESTING=OFF \
                       -DUSE_DISCORD_PRESENCE=OFF \
                       -DENABLE_ANALYTICS=OFF \
                       -DENABLE_AUTOUPDATE=OFF \
                       -DENABLE_CLI_TOOL=OFF \
                       -DENABLE_LTO=OFF"

PKG_CMAKE_OPTS_TARGET+=" -DUSE_SYSTEM_LIBS=AUTO"

PKG_CMAKE_OPTS_TARGET+=" -DUSE_SYSTEM_BZIP2=ON \
                         -DUSE_SYSTEM_CURL=ON \
                         -DUSE_SYSTEM_GLSLANG=ON \
                         -DUSE_SYSTEM_LIBUSB=ON \
                         -DUSE_SYSTEM_LZ4=ON \
                         -DUSE_SYSTEM_PUGIXML=ON \
                         -DUSE_SYSTEM_ZLIB=ON \
                         -DUSE_SYSTEM_ZSTD=ON"

PKG_CMAKE_OPTS_TARGET+=" -DENABLE_VULKAN=OFF"

PKG_CMAKE_OPTS_TARGET+=" -DUSE_SYSTEM_HIDAPI=OFF"

if [ "${OPENGL_SUPPORT}" = "yes" ]; then
  PKG_DEPENDS_TARGET+=" ${OPENGL} libglvnd"
  PKG_CMAKE_OPTS_TARGET+=" -DENABLE_EGL=ON"
fi

if [ "${OPENGLES_SUPPORT}" = "yes" ]; then
  PKG_DEPENDS_TARGET+=" ${OPENGLES}"
  PKG_CMAKE_OPTS_TARGET+=" -DENABLE_EGL=ON"
fi

if [ "${DISPLAYSERVER}" = "x11" ]; then
  PKG_CMAKE_OPTS_TARGET+=" -DENABLE_X11=ON"
else
  PKG_CMAKE_OPTS_TARGET+=" -DENABLE_X11=OFF"
fi

makeinstall_target() {
  mkdir -p ${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}
  cp $(find ${PKG_BUILD}/.${TARGET_NAME} -name ${PKG_LIBNAME} | head -1) \
     ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME}
  echo "set(${PKG_LIBVAR} ${SYSROOT_PREFIX}/usr/lib/${PKG_LIBNAME})" >${SYSROOT_PREFIX}/usr/lib/cmake/${PKG_NAME}/${PKG_NAME}-config.cmake
}
