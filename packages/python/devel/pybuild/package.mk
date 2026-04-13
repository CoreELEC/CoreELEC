# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2024-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pybuild"
PKG_VERSION="1.4.3"
PKG_SHA256="5aa4231ae0e807efdf1fd0623e07366eca2ab215921345a2e38acdd5d0fa0a74"
PKG_LICENSE="BSD"
PKG_SITE="https://pypi.org/project/build/"
PKG_URL="https://files.pythonhosted.org/packages/source/b/build/build-${PKG_VERSION}.tar.gz"
PKG_SOURCE_DIR="build-${PKG_VERSION}"
PKG_DEPENDS_HOST="flit:host pyproject-hooks:host pyinstaller:host pypackaging:host"
PKG_LONGDESC="A simple, correct Python build frontend."
PKG_TOOLCHAIN="python-flit"
