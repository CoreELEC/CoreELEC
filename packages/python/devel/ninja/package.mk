# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2014 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="ninja"
PKG_VERSION="1.12.0"
PKG_SHA256="8b2c86cd483dc7fcb7975c5ec7329135d210099a89bc7db0590a07b0bbfe49a5"
PKG_LICENSE="Apache"
PKG_SITE="https://ninja-build.org/"
PKG_URL="https://github.com/ninja-build/ninja/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="Python3:host googletest:host"
PKG_LONGDESC="Small build system with a focus on speed"
PKG_TOOLCHAIN="cmake-make"

makeinstall_host() {
  mkdir -p ${TOOLCHAIN}/bin
    cp ninja ${TOOLCHAIN}/bin
}
