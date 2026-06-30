# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="patchelf"
PKG_VERSION="0.19.0"
PKG_SHA256="b189d3ec57730757895b9e7d3a1f136d3af96ec9228ae6ef0a07c20a213f28f5"
PKG_LICENSE="GPL-3.0-or-later"
PKG_SITE="https://github.com/NixOS/patchelf"
PKG_URL="https://github.com/NixOS/patchelf/releases/download/${PKG_VERSION}/patchelf-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_HOST="autotools:host"
PKG_LONGDESC="A small utility to modify the dynamic linker and RPATH of ELF executables"
PKG_TOOLCHAIN="autotools"
PKG_BUILD_FLAGS="-cfg-libs:host"
