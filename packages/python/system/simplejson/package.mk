# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2017 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="simplejson"
PKG_VERSION="3.19.3"
PKG_SHA256="8e086896c36210ab6050f2f9f095a5f1e03c83fa0e7f296d6cba425411364680"
PKG_LICENSE="OSS"
PKG_SITE="http://pypi.org/project/simplejson"
PKG_URL="https://files.pythonhosted.org/packages/source/${PKG_NAME:0:1}/${PKG_NAME}/${PKG_NAME}-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain Python3 setuptools:host"
PKG_LONGDESC="A simple, fast, complete, correct and extensible JSON encoder and decoder for Python 2.5+."
PKG_TOOLCHAIN="python"

post_makeinstall_target() {
  python_remove_source

  rm -rf ${INSTALL}/usr/lib/python*/site-packages/*/tests
}
