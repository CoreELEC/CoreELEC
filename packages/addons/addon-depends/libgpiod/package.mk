# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2021-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libgpiod"
PKG_VERSION="2.3"
PKG_SHA256="3f7deffb86a4d0add7e4e4b58f21d50d2473f40a37dffddecc09895c3842dccd"
PKG_LICENSE="GPL-2.0-or-later AND LGPL-2.1-or-later"
PKG_SITE="https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git/about/"
PKG_URL="https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git/snapshot/libgpiod-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain Python3 setuptools:host"
PKG_LONGDESC="Tools for interacting with the linux GPIO character device."
PKG_BUILD_FLAGS="+pic -sysroot"

PKG_MESON_OPTS_TARGET="-Dtools=enabled \
                       -Dgpioset-interactive=disabled \
                       -Dtests=disabled \
                       -Dexamples=disabled \
                       -Dbindings-cxx=disabled \
                       -Dbindings-python=disabled \
                       -Dbindings-rust=disabled \
                       -Dbindings-glib=disabled \
                       -Ddbus=disabled \
                       -Dintrospection=disabled \
                       -Dsystemd=disabled \
                       -Dprofiling=false \
                       -Ddefault_library=shared"

post_make_target() {
  (
    cd ../bindings/python
    export CC="${TARGET_CC}"
    CFLAGS="${TARGET_CFLAGS} -I${PKG_BUILD}/include"
    LDFLAGS="${TARGET_LDFLAGS} -L${PKG_BUILD}/.${TARGET_NAME}/lib"
    python_target_env python3 -m build -n -w -x
  )
}
