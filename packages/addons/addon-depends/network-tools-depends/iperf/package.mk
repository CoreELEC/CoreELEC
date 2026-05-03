# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="iperf"
PKG_VERSION="3.21"
PKG_SHA256="dd289b6700d3bc33eda7fa3ce6db217d6ca42239edbcb2e7f152bf7bf5c8a5aa"
PKG_LICENSE="BSD"
PKG_SITE="http://software.es.net/iperf/"
PKG_URL="https://github.com/esnet/iperf/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain openssl"
PKG_LONGDESC="A tool to measuring maximum TCP and UDP bandwidth performance."
PKG_TOOLCHAIN="autotools"
PKG_BUILD_FLAGS="-sysroot"

PKG_CONFIGURE_OPTS_TARGET="--enable-static \
                           --disable-shared"
