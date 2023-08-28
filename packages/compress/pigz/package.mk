# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pigz"
PKG_VERSION="2.8"
PKG_SHA256="eb872b4f0e1f0ebe59c9f7bd8c506c4204893ba6a8492de31df416f0d5170fd0"
PKG_LICENSE="Other"
PKG_SITE="https://zlib.net/pigz/"
PKG_URL="https://zlib.net/pigz/pigz-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="make:host zlib:host"
PKG_LONGDESC="a parallel implementation of the gzip file compressor"
PKG_TOOLCHAIN="manual"

make_host() {
  make CPPFLAGS="${CPPFLAGS} -I${TOOLCHAIN}/include" \
       LDFLAGS="${LDFLAGS}" \
       CXX="${CXX}" \
       CC="${CC}" \
       pigz
}

makeinstall_host() {
  mkdir -p ${TOOLCHAIN}/bin
    cp pigz ${TOOLCHAIN}/bin
}
