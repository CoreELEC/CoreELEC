# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2012 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="ccid"
PKG_VERSION="1.8.4"
PKG_SHA256="4ff98151a7feb828a711e2f9d68c6b6065a97c597a81c2cbbd11d7f7edcfe743"
PKG_LICENSE="LGPL-2.1-or-later"
PKG_SITE="https://ccid.apdu.fr"
PKG_URL="https://ccid.apdu.fr/files/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain pcsc-lite"
PKG_LONGDESC="A generic USB Chip/Smart Card Interface Devices driver."

PKG_MESON_OPTS_TARGET="-Ddefault_library=shared \
                       -Dudev-rules=false \
                       -Dserial=true"
