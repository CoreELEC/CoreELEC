# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pyelftools"
PKG_VERSION="0.29"
PKG_SHA256="591c2b5150f180937c60714a1ae288365b1a3d6ae68aaaa4503dd5ecf5e3c299"
PKG_LICENSE="Unlicense"
PKG_SITE="https://github.com/eliben/pyelftools"
PKG_URL="https://github.com/eliben/pyelftools/archive/refs/tags/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="Python3:host setuptools:host"
PKG_LONGDESC="Library for analyzing ELF files and DWARF debugging information"
PKG_TOOLCHAIN="manual"

makeinstall_host() {
  exec_thread_safe python3 setup.py install --prefix=${TOOLCHAIN}
}
