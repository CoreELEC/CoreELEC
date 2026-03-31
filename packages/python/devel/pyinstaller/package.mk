# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2024-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pyinstaller"
PKG_VERSION="1.0.0"
PKG_SHA256="c6d691331621cf3fec4822f5c6f83cab3705f79b316225dc454127411677c71f"
PKG_LICENSE="MIT"
PKG_SITE="https://pypi.org/project/installer/"
PKG_URL="https://files.pythonhosted.org/packages/source/i/installer/installer-${PKG_VERSION}.tar.gz"
PKG_SOURCE_DIR="installer-${PKG_VERSION}"
PKG_DEPENDS_HOST="flit:host"
PKG_LONGDESC="installer provides basic functionality and abstractions for handling wheels and installing packages from wheels."
PKG_TOOLCHAIN="python-flit"

makeinstall_host() {
  # simple bootstrap install, but should be able to call itself if needed
  unzip -o -d ${TOOLCHAIN}/lib/${PKG_PYTHON_VERSION}/site-packages dist/*.whl
}
