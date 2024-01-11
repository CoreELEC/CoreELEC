# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libcap-ng"
PKG_VERSION="0.8.4"
PKG_SHA256="5615c76a61039e283a6bd107c4faf345ae5ad4dcd45907defe5e474d8fdb6fd2"
PKG_LICENSE="LGPLv2.1"
PKG_SITE="https://github.com/stevegrubb/libcap-ng"
PKG_URL="https://github.com/stevegrubb/libcap-ng/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Libcap-ng is a library for Linux that makes using posix capabilities easy."
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_TARGET="--enable-static --disable-shared --with-python=no --with-python3=no"
