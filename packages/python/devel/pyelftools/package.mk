# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pyelftools"
PKG_VERSION="0.30"
PKG_SHA256="548fc0dfe905b8378844889df610513f48fee299bb35100cf99f9b55cb610ff8"
PKG_LICENSE="Unlicense"
PKG_SITE="https://github.com/eliben/pyelftools"
PKG_URL="https://github.com/eliben/pyelftools/archive/refs/tags/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="Python3:host setuptools:host"
PKG_LONGDESC="Library for analyzing ELF files and DWARF debugging information"
PKG_TOOLCHAIN="manual"

makeinstall_host() {
  exec_thread_safe python3 setup.py install --prefix=${TOOLCHAIN}
}
