# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2012 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="ccid"
PKG_VERSION="1.8.0"
PKG_SHA256="531dc29e7c1b9e22e1918f0767b625d52ce8a98f266eb2144c5cf5dbd29c0f67"
PKG_LICENSE="LGPL-2.1-or-later"
PKG_SITE="https://ccid.apdu.fr"
PKG_URL="https://ccid.apdu.fr/files/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain pcsc-lite"
PKG_LONGDESC="A generic USB Chip/Smart Card Interface Devices driver."

PKG_MESON_OPTS_TARGET="-Ddefault_library=shared \
                       -Dudev-rules=false \
                       -Dserial=true"
