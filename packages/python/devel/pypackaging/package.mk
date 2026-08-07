# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2024-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pypackaging"
PKG_VERSION="26.3"
PKG_SHA256="94edc256424af38762eb31306eed28beb9f0efc50a8837492c9d6fd6004aed79"
PKG_LICENSE="Apache-2.0 OR BSD-2-Clause"
PKG_SITE="https://pypi.org/project/build/"
PKG_URL="https://files.pythonhosted.org/packages/source/p/packaging/packaging-${PKG_VERSION}.tar.gz"
PKG_SOURCE_DIR="packaging-${PKG_VERSION}"
PKG_DEPENDS_HOST="flit:host pyinstaller:host"
PKG_LONGDESC="Reusable core utilities for various Python Packaging interoperability specifications."
PKG_TOOLCHAIN="python-flit"
