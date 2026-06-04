# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="liblognorm"
PKG_VERSION="2.1.0"
PKG_SHA256="1f6c63d218a0a182b7758ce177033b604c2df30bbff80ea42b9bc032975e9ca9"
PKG_LICENSE="LGPL-2.1-or-later"
PKG_SITE="https://www.liblognorm.com"
PKG_URL="https://github.com/rsyslog/liblognorm/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libestr libfastjson"
PKG_TOOLCHAIN="autotools"
PKG_LONGDESC="A fast samples-based log normalization library."

PKG_CONFIGURE_OPTS_TARGET="--enable-static \
                           --disable-shared \
                           ac_cv_func_malloc_0_nonnull=yes \
                           ac_cv_func_realloc_0_nonnull=yes"
