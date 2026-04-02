# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2026-present Team CoreELEC (https://coreelec.org)

PKG_NAME="qt6"
PKG_VERSION="6.11.0"
PKG_SHA256=""
PKG_LICENSE="GPL"
PKG_SITE="https://qt-project.org"
PKG_URL="https://download.qt.io/official_releases/qt/${PKG_VERSION%.*}/${PKG_VERSION}/single/qt-everywhere-src-${PKG_VERSION}.tar.xz"
PKG_SOURCE_NAME="qt-everywhere-src-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain freetype libjpeg-turbo libpng openssl sqlite zlib qt6:host"
PKG_LONGDESC="A cross-platform application and UI framework."
PKG_BUILD_FLAGS="-sysroot"
PKG_TOOLCHAIN="manual"

# hide warnings
export LANG=C.UTF-8 LC_ALL=C.UTF-8

#######################################################################################################
unpack() {
  mkdir -p ${PKG_BUILD}

  # we don't use big qtwebengine, save space and unpack time
  tar --strip-components=1 \
    --exclude='qtwebengine' --exclude='qt3d' --exclude='qtdatavis3d' \
    --exclude='qtquick3d' --exclude='qtquick3dphysics' \
    --exclude='qtcharts' --exclude='qtcoap' \
    --exclude='qthttpserver' --exclude='qtmqtt' \
    --exclude='qtserialbus' --exclude='qtspeech' \
    --exclude='qtwayland' --exclude='qtvirtualkeyboard' \
    -xf ${SOURCES}/${PKG_NAME}/${PKG_SOURCE_NAME} -C ${PKG_BUILD}
}

#######################################################################################################
configure_host() {
  [ "${SKIP_QT_HOST_BUILD}" = "yes" ] && return

  QT_OPTS_HOST="
    -DQT_BUILD_EXAMPLES=OFF
    -DQT_BUILD_TESTS=OFF
    -DQT_BUILD_DOCS=OFF
    -DQT_BUILD_BENCHMARKS=OFF
    -DBUILD_SHARED_LIBS=OFF
    -DFEATURE_optimize_size=ON
    -DFEATURE_precompile_header=ON
    -DFEATURE_reduce_exports=ON

    -DFEATURE_printsupport=OFF
    -DFEATURE_permissions=OFF
    -DFEATURE_sqlmodel=OFF
    -DFEATURE_androiddeployqt=OFF
    -DBUILD_qtwayland=OFF
    -DQT_BUILD_QTWAYLAND=OFF
    -DFEATURE_emoji_segmenter=OFF
    -DINPUT_emoji_segmenter=no
    -DFEATURE_sessionmanager=OFF
    -DFEATURE_itemmodeltester=OFF

    -DFEATURE_xcb=OFF
    -DFEATURE_wayland=OFF
    -DFEATURE_eglfs=OFF
    -DFEATURE_linuxfb=OFF
    -DFEATURE_vnc=OFF
    -DFEATURE_minimal=ON
    -DFEATURE_offscreen=ON

    -DFEATURE_concurrent=ON
    -DFEATURE_dbus=ON
    -DFEATURE_gui=ON
    -DFEATURE_network=ON
    -DFEATURE_sql=ON
    -DFEATURE_testlib=ON
    -DFEATURE_widgets=ON
    -DFEATURE_xml=ON

    -DFEATURE_doubleconversion=ON
    -DFEATURE_system_doubleconversion=OFF
    -DFEATURE_glib=OFF
    -DFEATURE_iconv=OFF
    -DFEATURE_icu=OFF
    -DFEATURE_mimetype_database=ON
    -DFEATURE_pcre2=ON
    -DFEATURE_system_pcre2=OFF

    -DFEATURE_openssl=ON
    -DFEATURE_openssl_linked=ON
    -DFEATURE_dtls=ON
    -DFEATURE_ocsp=ON
    -DFEATURE_sctp=OFF
    -DFEATURE_gssapi=OFF
    -DFEATURE_libproxy=OFF

    -DFEATURE_openssl_hash=OFF

    -DFEATURE_gif=ON
    -DFEATURE_ico=OFF
    -DFEATURE_jpeg=ON
    -DFEATURE_system_jpeg=ON
    -DFEATURE_png=ON
    -DFEATURE_system_png=ON
    -DFEATURE_freetype=OFF
    -DFEATURE_system_freetype=OFF
    -DFEATURE_harfbuzz=OFF
    -DFEATURE_fontconfig=OFF
    -DFEATURE_accessibility=OFF

    -DFEATURE_texthtmlparser=ON
    -DFEATURE_cssparser=ON
    -DFEATURE_textodfwriter=ON
    -DFEATURE_textmarkdownreader=ON
    -DFEATURE_system_libmd4c=OFF
    -DFEATURE_textmarkdownwriter=ON

    -DFEATURE_opengl=OFF
    -DFEATURE_opengles2=OFF
    -DFEATURE_opengles3=OFF
    -DFEATURE_vulkan=OFF
    -DFEATURE_egl=OFF
    -DINPUT_opengl=no

    -DFEATURE_directfb=OFF
    -DFEATURE_eglfs=OFF
    -DFEATURE_linuxfb=OFF
    -DFEATURE_vnc=OFF

    -DFEATURE_evdev=OFF
    -DFEATURE_libinput=OFF
    -DFEATURE_mtdev=OFF
    -DFEATURE_tslib=OFF
    -DFEATURE_xkbcommon=OFF

    -DFEATURE_gtk3=OFF

    -DFEATURE_sql_sqlite=OFF
    -DFEATURE_system_sqlite=OFF
    -DFEATURE_sql_mysql=OFF
    -DFEATURE_sql_psql=OFF
    -DFEATURE_sql_odbc=OFF
    -DFEATURE_sql_db2=OFF
    -DFEATURE_sql_ibase=OFF
    -DFEATURE_sql_oci=OFF

    -DFEATURE_neon=OFF

    -DFEATURE_system_zlib=ON
    -DFEATURE_zstd=OFF
  "

  cmake --fresh .. -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_SUPPRESS_DEVELOPER_WARNINGS=ON \
    -DCMAKE_CXX_STANDARD=17 \
    -DCMAKE_C_COMPILER_LAUNCHER=${TOOLCHAIN}/bin/ccache \
    -DCMAKE_CXX_COMPILER_LAUNCHER=${TOOLCHAIN}/bin/ccache \
    -DFEATURE_ccache=ON \
    -DQT_USE_CCACHE=ON \
    ${QT_OPTS_HOST}

  echo "configure host ok"
}

make_host() {
  [ "${SKIP_QT_HOST_BUILD}" = "yes" ] && return
  cmake --build . --target host_tools --parallel
  echo "make host ok"
}

makeinstall_host() {
  : # no need to install
}

#######################################################################################################
configure_target() {
  TOOLCHAIN_FILE="${PKG_REAL_BUILD}/toolchain-libreelec.cmake"

  cat >${TOOLCHAIN_FILE} <<EOF
set(CMAKE_SYSTEM_NAME      Linux)
set(CMAKE_SYSTEM_PROCESSOR ${TARGET_ARCH})
set(CMAKE_C_COMPILER       "${CC}")
set(CMAKE_CXX_COMPILER     "${CXX}")

set(CMAKE_SYSROOT          "${SYSROOT_PREFIX}")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
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

  QT_OPTS_TARGET="
    -DQT_BUILD_EXAMPLES=OFF
    -DQT_BUILD_TESTS=OFF
    -DQT_BUILD_DOCS=OFF
    -DQT_BUILD_BENCHMARKS=OFF
    -DBUILD_SHARED_LIBS=OFF
    -DFEATURE_optimize_size=ON
    -DFEATURE_precompile_header=ON
    -DFEATURE_reduce_exports=ON

    -DFEATURE_printsupport=OFF
    -DFEATURE_permissions=OFF
    -DFEATURE_sqlmodel=OFF
    -DFEATURE_androiddeployqt=OFF
    -DBUILD_qtwayland=OFF
    -DQT_BUILD_QTWAYLAND=OFF
    -DFEATURE_emoji_segmenter=OFF
    -DINPUT_emoji_segmenter=no
    -DFEATURE_sessionmanager=OFF
    -DFEATURE_itemmodeltester=OFF
    
    -DFEATURE_libudev=OFF

    -DFEATURE_xcb=OFF
    -DFEATURE_wayland=OFF
    -DFEATURE_eglfs=OFF
    -DFEATURE_linuxfb=OFF
    -DFEATURE_vnc=OFF
    -DFEATURE_minimal=ON
    -DFEATURE_offscreen=ON

    -DFEATURE_alsa=OFF \
    -DFEATURE_pulseaudio=OFF \
    -DFEATURE_coreaudio=OFF \
    -DFEATURE_wmsdk=OFF \
    -DFEATURE_mmrenderer=OFF

    -DFEATURE_concurrent=ON
    -DFEATURE_dbus=ON
    -DFEATURE_gui=ON
    -DFEATURE_network=ON
    -DFEATURE_sql=ON
    -DFEATURE_testlib=ON
    -DFEATURE_widgets=ON
    -DFEATURE_xml=ON

    -DFEATURE_doubleconversion=ON
    -DFEATURE_system_doubleconversion=OFF
    -DFEATURE_glib=OFF
    -DFEATURE_iconv=OFF
    -DFEATURE_icu=OFF
    -DFEATURE_mimetype_database=ON
    -DFEATURE_pcre2=ON
    -DFEATURE_system_pcre2=OFF

    -DFEATURE_openssl=ON
    -DFEATURE_openssl_linked=ON
    -DFEATURE_dtls=ON
    -DFEATURE_ocsp=ON
    -DFEATURE_sctp=OFF
    -DFEATURE_gssapi=OFF
    -DFEATURE_libproxy=OFF

    -DFEATURE_openssl_hash=OFF

    -DFEATURE_gif=ON
    -DFEATURE_ico=OFF
    -DFEATURE_jpeg=ON
    -DFEATURE_system_jpeg=ON
    -DFEATURE_png=ON
    -DFEATURE_system_png=ON
    -DFEATURE_freetype=OFF
    -DFEATURE_system_freetype=OFF
    -DFEATURE_harfbuzz=OFF
    -DFEATURE_fontconfig=OFF
    -DFEATURE_accessibility=OFF

    -DFEATURE_texthtmlparser=ON
    -DFEATURE_cssparser=ON
    -DFEATURE_textodfwriter=ON
    -DFEATURE_textmarkdownreader=ON
    -DFEATURE_system_libmd4c=OFF
    -DFEATURE_textmarkdownwriter=ON

    -DFEATURE_opengl=OFF
    -DFEATURE_opengles2=OFF
    -DFEATURE_opengles3=OFF
    -DFEATURE_vulkan=OFF
    -DFEATURE_egl=OFF
    -DINPUT_opengl=no

    -DFEATURE_directfb=OFF
    -DFEATURE_eglfs=OFF
    -DFEATURE_linuxfb=OFF
    -DFEATURE_vnc=OFF

    -DFEATURE_evdev=OFF
    -DFEATURE_libinput=OFF
    -DFEATURE_mtdev=OFF
    -DFEATURE_tslib=OFF
    -DFEATURE_xkbcommon=OFF

    -DFEATURE_gtk3=OFF

    -DFEATURE_sql_sqlite=OFF
    -DFEATURE_system_sqlite=OFF
    -DFEATURE_sql_mysql=OFF
    -DFEATURE_sql_psql=OFF
    -DFEATURE_sql_odbc=OFF
    -DFEATURE_sql_db2=OFF
    -DFEATURE_sql_ibase=OFF
    -DFEATURE_sql_oci=OFF

    -DFEATURE_neon=ON

    -DFEATURE_system_zlib=ON
    -DFEATURE_zstd=OFF
  "

  unset CC CXX LD AR AS RANLIB NM STRIP OBJCOPY CFLAGS CXXFLAGS LDFLAGS CPPFLAGS

  cmake --fresh .. -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_SUPPRESS_DEVELOPER_WARNINGS=ON \
    -DCMAKE_CXX_STANDARD=17 \
    -DCMAKE_TOOLCHAIN_FILE="${TOOLCHAIN_FILE}" \
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DCMAKE_STAGING_PREFIX="${SYSROOT_PREFIX}/usr" \
    -DQT_QMAKE_TARGET_MKSPEC=devices/linux-libreelec-g++ \
    -DQT_HOST_PATH="${PKG_BUILD}/.${HOST_NAME}/qtbase/lib/cmake" \
    -DQT_HOST_PATH_CMAKE_DIR="${PKG_BUILD}/.${HOST_NAME}/qtbase/lib/cmake" \
    -DCMAKE_C_COMPILER_LAUNCHER=${TOOLCHAIN}/bin/ccache \
    -DCMAKE_CXX_COMPILER_LAUNCHER=${TOOLCHAIN}/bin/ccache \
    -DFEATURE_ccache=ON \
    -DQT_USE_CCACHE=ON \
    ${QT_OPTS_TARGET}
}

make_target() {
  cmake --build . --parallel
  echo "make target ok"
}

makeinstall_target() {
  echo "makeinstall_target"
  cmake --install .
}
