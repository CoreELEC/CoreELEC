# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="ccache"
PKG_VERSION="4.10.1"
PKG_SHA256="3a43442ce3916ea48bb6ccf6f850891cbff01d1feddff7cd4bbd49c5cf1188f6"
PKG_LICENSE="GPL"
PKG_SITE="https://ccache.dev/download.html"
PKG_URL="https://github.com/ccache/ccache/releases/download/v${PKG_VERSION}/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_HOST="cmake:host make:host zstd:host libfmt:host xxHash:host"
PKG_LONGDESC="A compiler cache to speed up re-compilation of C/C++ code by caching."
# Override toolchain as ninja is not built yet
PKG_TOOLCHAIN="cmake-make"
PKG_BUILD_FLAGS="+local-cc"

configure_host() {
  # custom cmake build to override the LOCAL_CC/CXX
  cp ${CMAKE_CONF} cmake-ccache.conf

  echo "SET(CMAKE_C_COMPILER   ${CC})"  >> cmake-ccache.conf
  echo "SET(CMAKE_CXX_COMPILER ${CXX})" >> cmake-ccache.conf

  cmake -DCMAKE_TOOLCHAIN_FILE=cmake-ccache.conf \
        -DCMAKE_INSTALL_PREFIX=${TOOLCHAIN} \
        -DENABLE_DOCUMENTATION=OFF \
        -DREDIS_STORAGE_BACKEND=OFF \
        -DENABLE_TESTING=OFF \
        -DDEPS=SYSTEM \
        ..
}

post_makeinstall_host() {
# setup ccache
  if [ -z "${CCACHE_DISABLE}" ]; then
    CCACHE_DIR="${BUILD_CCACHE_DIR}" ${TOOLCHAIN}/bin/ccache --max-size=${CCACHE_CACHE_SIZE}
    CCACHE_DIR="${BUILD_CCACHE_DIR}" ${TOOLCHAIN}/bin/ccache --set-config compression_level=${CCACHE_COMPRESSLEVEL}
  fi

  cat > ${TOOLCHAIN}/bin/host-gcc <<EOF
#!/bin/sh
${TOOLCHAIN}/bin/ccache ${LOCAL_CC} "\$@"
EOF

  chmod +x ${TOOLCHAIN}/bin/host-gcc

  cat > ${TOOLCHAIN}/bin/host-g++ <<EOF
#!/bin/sh
${TOOLCHAIN}/bin/ccache ${LOCAL_CXX} "\$@"
EOF

  chmod +x ${TOOLCHAIN}/bin/host-g++
}
