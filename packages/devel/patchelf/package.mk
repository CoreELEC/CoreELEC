# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="patchelf"
PKG_VERSION="0.19.1"
PKG_SHA256="2cce01de93653829f6ab68a20c2ec275e1c00a946110704a27e928d2e6e88716"
PKG_LICENSE="GPL-3.0-or-later"
PKG_SITE="https://github.com/NixOS/patchelf"
PKG_URL="https://github.com/NixOS/patchelf/releases/download/${PKG_VERSION}/patchelf-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_HOST="autotools:host"
PKG_LONGDESC="A small utility to modify the dynamic linker and RPATH of ELF executables"
PKG_TOOLCHAIN="autotools"
PKG_BUILD_FLAGS="-cfg-libs:host"
