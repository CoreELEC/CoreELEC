# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2020-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="lcms2"
PKG_VERSION="2.19.1"
PKG_SHA256="bfc54f7bab59fbc921012014a8032e4cba4abd46db47d46b76416a8c0b2815c8"
PKG_LICENSE="MIT/GPLv3"
PKG_SITE="http://www.littlecms.com"
PKG_URL="https://github.com/mm2/Little-CMS/releases/download/lcms${PKG_VERSION}/lcms2-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain tiff"
PKG_LONGDESC="An small-footprint color management engine, with special focus on accuracy and performance."
PKG_BUILD_FLAGS="+pic"

PKG_MESON_OPTS_TARGET="-Ddefault_library=static \
                       -Dprefer_static=true \
                       -Dtests=disabled \
                       -Dutils=true"
