# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2024-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pyproject-hooks"
PKG_VERSION="1.3.0"
PKG_SHA256="26dfd4229ff1820b7964be33bd3eb64367eb7c841215defe11341450c219ce20"
PKG_LICENSE="MIT"
PKG_SITE="https://pypi.org/project/pyproject-hooks/"
PKG_URL="https://files.pythonhosted.org/packages/source/p/pyproject_hooks/pyproject_hooks-${PKG_VERSION}.tar.gz"
PKG_SOURCE_DIR="pyproject_hooks-${PKG_VERSION}"
PKG_DEPENDS_HOST="flit:host pyinstaller:host"
PKG_LONGDESC="pyproject-hooks provides the basic functionality to help write tooling that generates distribution files from Python projects."
PKG_TOOLCHAIN="python-flit"
