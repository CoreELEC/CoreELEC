# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2026-present Team CoreELEC (https://coreelec.org)

PKG_NAME="qt6"
PKG_VERSION="6.11.0"
PKG_SHA256="acf3b3db04c9e5d0820e8324b097320388954c297cee83d2bd698789234f68a4"
PKG_LICENSE="GPL"
PKG_SITE="https://qt-project.org"
PKG_URL="https://download.qt.io/official_releases/qt/${PKG_VERSION%.*}/${PKG_VERSION}/single/qt-everywhere-src-${PKG_VERSION}.tar.xz"
PKG_SOURCE_NAME="qt-everywhere-src-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_HOST="toolchain"
PKG_DEPENDS_TARGET="toolchain freetype libjpeg-turbo libpng openssl sqlite zlib qt6:host"
PKG_LONGDESC="A cross-platform application and UI framework."
PKG_BUILD_FLAGS="-sysroot"
PKG_TOOLCHAIN="manual"

CMAKE_OPTS_COMMON="
  -DBUILD_SHARED_LIBS=OFF
  -DBUILD_qtdeclarative=OFF
  -DQT_BUILD_EXAMPLES=OFF
  -DQT_BUILD_TESTS=OFF
  -DQT_BUILD_DOCS=OFF
  -DQT_BUILD_BENCHMARKS=OFF
  -DINPUT_opengl=no
  -DFEATURE_optimize_size=ON
  -DFEATURE_reduce_exports=ON
  -DFEATURE_directfb=OFF
  -DFEATURE_egl=OFF
  -DFEATURE_eglfs=OFF
  -DFEATURE_evdev=OFF
  -DFEATURE_fontconfig=OFF
  -DFEATURE_gui=ON
  -DFEATURE_harfbuzz=OFF
  -DFEATURE_ico=OFF
  -DFEATURE_libudev=OFF
  -DFEATURE_linuxfb=OFF
  -DFEATURE_mtdev=OFF
  -DFEATURE_opengl=OFF
  -DFEATURE_opengles2=OFF
  -DFEATURE_opengles3=OFF
  -DFEATURE_printsupport=OFF
  -DFEATURE_vulkan=OFF
  -DFEATURE_xkbcommon=OFF
  -DFEATURE_zstd=OFF
  -DQT_FEATURE_icu=OFF
  -DQT_FEATURE_vnc=OFF
  -DQT_FEATURE_wayland=OFF
  -DQT_FEATURE_wayland_client=OFF
"

CMAKE_OPTS_HOST="
  ${CMAKE_OPTS_COMMON}
  -DQT_BUILD_SUBMODULES='qtbase;qtshadertools;qtdeclarative'
"


CMAKE_OPTS_TARGET="
  ${CMAKE_OPTS_COMMON}
  -DQT_BUILD_SUBMODULES='qtbase;qtserialport;qtwebsockets'
"

LANG=C.UTF-8 LC_ALL=C.UTF-8  # hide build warnings

unpack() {
  mkdir -p ${PKG_BUILD}

  # we don't use big qtwebengine, save space and unpack time
  tar --strip-components=1 \
    --exclude='qtwebengine'      --exclude='qt3d' \
    --exclude='qtdatavis3d'      --exclude='qtquick3d' \
    --exclude='qtquick3dphysics' --exclude='qtcharts' \
    --exclude='qtcoap'           --exclude='qthttpserver' \
    --exclude='qtmqtt'           --exclude='qtserialbus'  \
    --exclude='qtspeech'         --exclude='qtwayland' \
    --exclude='qtvirtualkeyboard' \
    -xf ${SOURCES}/${PKG_NAME}/${PKG_SOURCE_NAME} -C ${PKG_BUILD}

  # fix pcre2 detection
  # https://github.com/ericLemanissier/cocorepo/commit/21abf03212100c1da7c17358b91732b624d4acc7
  sed -e 's|PCRE2::16BIT|PCRE2::pcre2-16|' \
      -i ${PKG_BUILD}/qtbase/cmake/FindWrapSystemPCRE2.cmake
}

configure_host() {
  cmake .. -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_SUPPRESS_DEVELOPER_WARNINGS=ON \
    -DCMAKE_CXX_STANDARD=17 \
    -DCMAKE_C_COMPILER_LAUNCHER="${TOOLCHAIN}/bin/ccache" \
    -DCMAKE_CXX_COMPILER_LAUNCHER="${TOOLCHAIN}/bin/ccache" \
    -DQT_USE_CCACHE=ON \
    -DCMAKE_EXE_LINKER_FLAGS="-Wl,--start-group -Wl,--whole-archive -L${TOOLCHAIN}/lib -lffi -lpcre2-16 -lpcre2-8 -lglib-2.0 -lgmodule-2.0 -lgobject-2.0 -Wl,--no-whole-archive -Wl,--end-group" \
    ${CMAKE_OPTS_HOST}

  # CMAKE_EXE_LINKER_FLAGS is used to add extra libraries because
  # ffi and pcre2 are not found (wrong order by default, bug in qt)
}

make_host() {
  cmake --build . --target host_tools --parallel
}

makeinstall_host() {
  : # used by target directly
}

configure_target() {
  TOOLCHAIN_FILE="${PKG_REAL_BUILD}/toolchain-libreelec.cmake"

  cat >${TOOLCHAIN_FILE} <<EOF
set(CMAKE_SYSTEM_NAME       Linux)
set(CMAKE_SYSTEM_PROCESSOR  ${TARGET_ARCH})
set(CMAKE_SYSROOT           "${SYSROOT_PREFIX}")
set(CMAKE_C_COMPILER        "${CC}")
set(CMAKE_CXX_COMPILER      "${CXX}")
set(CMAKE_C_COMPILER_LAUNCHER   "${TOOLCHAIN}/bin/ccache")
set(CMAKE_CXX_COMPILER_LAUNCHER "${TOOLCHAIN}/bin/ccache")
set(QT_USE_CCACHE ON)

set(CMAKE_FIND_ROOT_PATH "${SYSROOT_PREFIX}" "${TOOLCHAIN}")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(CMAKE_INSTALL_PREFIX   "/usr")
set(CMAKE_STAGING_PREFIX   "${SYSROOT_PREFIX}/usr")
set(QT_QMAKE_TARGET_MKSPEC "devices/linux-libreelec-g++")
EOF

  QMAKE_CONF_DIR="${PKG_BUILD}/qtbase/mkspecs/devices/linux-libreelec-g++"
  mkdir -p "${QMAKE_CONF_DIR}"

  cat >"${QMAKE_CONF_DIR}/qmake.conf" <<EOF
MAKEFILE_GENERATOR      = UNIX
CONFIG                 += incremental
QMAKE_INCREMENTAL_STYLE = sublib
include(../../common/linux.conf)
include(../../common/gcc-base-unix.conf)
include(../../common/g++-unix.conf)
load(device_config)
QMAKE_CC         = ${CC}
QMAKE_CXX        = ${CXX}
QMAKE_LINK       = ${CXX}
QMAKE_LINK_SHLIB = ${CXX}
QMAKE_AR         = ${AR} cqs
QMAKE_OBJCOPY    = ${OBJCOPY}
QMAKE_NM         = ${NM} -P
QMAKE_STRIP      = ${STRIP}
QMAKE_CFLAGS     = ${CFLAGS}
QMAKE_CXXFLAGS   = ${CXXFLAGS}
QMAKE_LFLAGS     = ${LDFLAGS}
load(qt_config)
EOF

  cat >"${QMAKE_CONF_DIR}/qplatformdefs.h" <<EOF
#include "../../linux-g++/qplatformdefs.h"
EOF

  unset CC CXX LD AR AS RANLIB NM STRIP OBJCOPY CFLAGS CXXFLAGS LDFLAGS CPPFLAGS

  cmake .. -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_SUPPRESS_DEVELOPER_WARNINGS=ON \
    -DCMAKE_INSTALL_MESSAGE=NEVER \
    -DCMAKE_CXX_STANDARD=17 \
    -DCMAKE_TOOLCHAIN_FILE="${TOOLCHAIN_FILE}" \
    -DQT_HOST_PATH="${PKG_BUILD}/.${HOST_NAME}/qtbase/lib/cmake" \
    -DQT_HOST_PATH_CMAKE_DIR="${PKG_BUILD}/.${HOST_NAME}/qtbase/lib/cmake" \
    ${CMAKE_OPTS_TARGET}
}

make_target() {
  cmake --build . --parallel
}

makeinstall_target() {
  cmake --install .
}
