# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pv"
PKG_VERSION="1.8.5"
PKG_SHA256="d22948d06be06a5be37336318de540a2215be10ab0163f8cd23a20149647b780"
PKG_LICENSE="GNU"
PKG_SITE="http://www.ivarch.com/programs/pv.shtml"
PKG_URL="http://www.ivarch.com/programs/sources/pv-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Pipe Viewer can be inserted into any normal pipeline between two processes."
PKG_BUILD_FLAGS="-sysroot"

PKG_CONFIGURE_OPTS_TARGET="--enable-static-nls"
