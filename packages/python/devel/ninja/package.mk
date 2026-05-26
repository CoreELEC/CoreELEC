# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2014 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="ninja"
PKG_VERSION="1.13.2"
PKG_SHA256="974d6b2f4eeefa25625d34da3cb36bdcebe7fbce40f4c16ac0835fd1c0cbae17"
PKG_LICENSE="Apache-2.0"
PKG_SITE="https://ninja-build.org/"
PKG_URL="https://github.com/ninja-build/ninja/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="Python3:host"
PKG_LONGDESC="Small build system with a focus on speed"
PKG_TOOLCHAIN="manual"

make_host() {
  python3 ../configure.py --bootstrap
}

makeinstall_host() {
  mkdir -p ${TOOLCHAIN}/bin
    cp ninja ${TOOLCHAIN}/bin
}
