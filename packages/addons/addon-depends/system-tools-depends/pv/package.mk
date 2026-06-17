# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pv"
PKG_VERSION="1.11.0"
PKG_SHA256="fc02c9fc2b82b20a92cc8d98f844be63f22abd98751a8e4abc875e1d803662eb"
PKG_LICENSE="GPL-3.0-or-later"
PKG_SITE="http://www.ivarch.com/programs/pv.shtml"
PKG_URL="http://www.ivarch.com/programs/sources/pv-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Pipe Viewer can be inserted into any normal pipeline between two processes."
PKG_BUILD_FLAGS="-sysroot -cfg-libs"
