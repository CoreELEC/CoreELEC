# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2023-present Team CoreELEC (https://coreelec.org)

PKG_NAME="mbedtls"
PKG_VERSION="3.6.6"
PKG_SHA256="8fb65fae8dcae5840f793c0a334860a411f884cc537ea290ce1c52bb64ca007a"
PKG_LICENSE="Apache 2.0"
PKG_SITE="https://github.com/Mbed-TLS/mbedtls"
PKG_URL="https://github.com/Mbed-TLS/mbedtls/releases/download/${PKG_NAME}-${PKG_VERSION}/${PKG_NAME}-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_TARGET="toolchain jsonschema:host Jinja2:host"
PKG_DEPENDS_UNPACK="mbedtls-framework"
PKG_LONGDESC="Mbed TLS is a C library that implements cryptographic primitives, X.509 certificate manipulation and the SSL/TLS and DTLS protocols."

PKG_CMAKE_OPTS_TARGET="-DCMAKE_BUILD_TYPE=Release \
                       -DUSE_SHARED_MBEDTLS_LIBRARY=OFF \
                       -DUSE_STATIC_MBEDTLS_LIBRARY=ON \
                       -DENABLE_TESTING=OFF \
                       -DLINK_WITH_PTHREAD=ON \
                       -DENABLE_PROGRAMS=OFF \
                       -Wno-dev"

post_unpack() {
  cp -r $(get_build_dir mbedtls-framework)/* ${PKG_BUILD}/framework
}
