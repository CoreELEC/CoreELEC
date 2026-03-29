# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2024-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pybuild"
PKG_VERSION="1.4.2"
PKG_SHA256="35b14e1ee329c186d3f08466003521ed7685ec15ecffc07e68d706090bf161d1"
PKG_LICENSE="BSD"
PKG_SITE="https://pypi.org/project/build/"
PKG_URL="https://files.pythonhosted.org/packages/source/b/build/build-${PKG_VERSION}.tar.gz"
PKG_SOURCE_DIR="build-${PKG_VERSION}"
PKG_DEPENDS_HOST="flit:host pyproject-hooks:host pyinstaller:host pypackaging:host"
PKG_LONGDESC="A simple, correct Python build frontend."
PKG_TOOLCHAIN="python-flit"
