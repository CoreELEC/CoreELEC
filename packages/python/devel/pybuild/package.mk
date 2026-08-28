# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2024-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pybuild"
PKG_VERSION="1.6.0"
PKG_SHA256="bd2c8afc603e7a2e0ce70e2ea85f0a6d02043bafbd307f5bada0f98669eca5af"
PKG_LICENSE="MIT"
PKG_SITE="https://pypi.org/project/build/"
PKG_URL="https://files.pythonhosted.org/packages/source/b/build/build-${PKG_VERSION}.tar.gz"
PKG_SOURCE_DIR="build-${PKG_VERSION}"
PKG_DEPENDS_HOST="flit:host pyproject-hooks:host pyinstaller:host pypackaging:host"
PKG_LONGDESC="A simple, correct Python build frontend."
PKG_TOOLCHAIN="python-flit"
