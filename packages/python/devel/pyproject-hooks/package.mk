# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2024-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pyproject-hooks"
PKG_VERSION="1.3.3"
PKG_SHA256="defda19b854fa0d3bd4f76ea4ddcba8abd7dcfcdd585a6690ade050744fc5f43"
PKG_LICENSE="MIT"
PKG_SITE="https://pypi.org/project/pyproject-hooks/"
PKG_URL="https://files.pythonhosted.org/packages/source/p/pyproject_hooks/pyproject_hooks-${PKG_VERSION}.tar.gz"
PKG_SOURCE_DIR="pyproject_hooks-${PKG_VERSION}"
PKG_DEPENDS_HOST="flit:host pyinstaller:host"
PKG_LONGDESC="pyproject-hooks provides the basic functionality to help write tooling that generates distribution files from Python projects."
PKG_TOOLCHAIN="python-flit"
