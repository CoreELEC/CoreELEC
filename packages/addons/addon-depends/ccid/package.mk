# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2012 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="ccid"
PKG_VERSION="1.8.2"
PKG_SHA256="d74294e23d436546c3e719c95a4da180b17f5e7ffdd36efca53f75351cb0de75"
PKG_LICENSE="LGPL-2.1-or-later"
PKG_SITE="https://ccid.apdu.fr"
PKG_URL="https://ccid.apdu.fr/files/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain pcsc-lite"
PKG_LONGDESC="A generic USB Chip/Smart Card Interface Devices driver."

PKG_MESON_OPTS_TARGET="-Ddefault_library=shared \
                       -Dudev-rules=false \
                       -Dserial=true"
