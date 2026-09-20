# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pcsc-lite"
PKG_VERSION="2.5.2"
PKG_SHA256="60a08942d8c00a1d86a3bf4c64eddbf7a5569c7c54f6904e3f07801bb5316acd"
PKG_LICENSE="BSD-3-Clause"
PKG_SITE="https://pcsclite.apdu.fr"
PKG_URL="https://pcsclite.apdu.fr/files/pcsc-lite-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain libusb polkit"
PKG_DEPENDS_CONFIG="polkit"
PKG_LONGDESC="Middleware to access a smart card using SCard API (PC/SC)."

PKG_MESON_OPTS_TARGET="-Ddefault_library=both \
                       -Dlibudev=false \
                       -Dlibusb=true \
                       -Dpolkit=true \
                       -Dusbdropdir=/storage/.kodi/addons/service.pcscd/drivers"
