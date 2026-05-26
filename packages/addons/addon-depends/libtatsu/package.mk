# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2025-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libtatsu"
PKG_VERSION="1.0.5"
PKG_SHA256="536fa228b14f156258e801a7f4d25a3a9dd91bb936bf6344e23171403c57e440"
PKG_LICENSE="LGPL-2.1-or-later"
PKG_SITE="http://www.libimobiledevice.org"
PKG_URL="https://github.com/libimobiledevice/libtatsu/releases/download/${PKG_VERSION}/libtatsu-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_TARGET="toolchain curl libplist"
PKG_LONGDESC="Library handling the communication with Apple Tatsu Signing Server (TSS)"
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_TARGET="--enable-static \
                           --disable-shared \
                           --disable-largefile"
