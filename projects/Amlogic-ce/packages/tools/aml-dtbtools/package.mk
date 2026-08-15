# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)
# Copyright (C) 2020-present Team CoreELEC (https://coreelec.org)

PKG_NAME="aml-dtbtools"
PKG_VERSION="b2ca13ce06627d4e38b3fce56d7aadf077b7bc7d"
PKG_SHA256=""
PKG_LICENSE="free"
PKG_SITE="https://github.com/Wilhansen/aml-dtbtools"
PKG_URL="https://github.com/Wilhansen/aml-dtbtools/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="gcc:host zlib:host dtc:host"
PKG_DEPENDS_TARGET="toolchain zlib dtc"
PKG_LONGDESC="AML DTB Tools"

PKG_MAKE_OPTS_HOST="dtbTool"
PKG_MAKE_OPTS_TARGET="dtbTool dtbSplit"

pre_make_host() {
  rm -f ${PKG_MAKE_OPTS_HOST}
}

makeinstall_host() {
  mkdir -p ${TOOLCHAIN}/bin
    cp dtbTool ${TOOLCHAIN}/bin
}

pre_make_target() {
  rm -f ${PKG_MAKE_OPTS_TARGET}
}

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/bin
    cp dtbTool dtbSplit ${INSTALL}/usr/bin
}
