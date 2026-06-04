# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="netbase"
PKG_VERSION="6.5"
PKG_SHA256="9116047aebbaa1698934052d01c6e09b4c3aed643e93df63d2ddcbec243c26d1"
PKG_LICENSE="GPL-2.0-only"
PKG_SITE="https://salsa.debian.org/md/netbase"
PKG_URL="http://ftp.debian.org/debian/pool/main/n/netbase/netbase_${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="The netbase package provides data for network services and protocols from the iana db."
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  mkdir -p ${INSTALL}/etc
    cp etc/protocols ${INSTALL}/etc
    cp etc/services ${INSTALL}/etc
}
