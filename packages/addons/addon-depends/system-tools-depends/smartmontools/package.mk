# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="smartmontools"
PKG_VERSION="7.5"
PKG_SHA256="690b83ca331378da9ea0d9d61008c4b22dde391387b9bbad7f29387f2595f76e"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://www.smartmontools.org"
PKG_URL="https://downloads.sourceforge.net/project/smartmontools/smartmontools/${PKG_VERSION}/${PKG_NAME}-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Control and monitor storage systems using S.M.A.R.T."
PKG_BUILD_FLAGS="-sysroot -cfg-libs"

PKG_CONFIGURE_OPTS_TARGET="--prefix=/usr \
                           --without-initscriptdir \
                           --without-nvme-devicescan \
                           --without-systemdenvfile \
                           --without-systemdsystemunitdir \
                           --without-systemdenvfile \
                           --without-systemdsystemunitdir"
