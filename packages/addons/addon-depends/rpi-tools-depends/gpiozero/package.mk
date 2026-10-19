# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="gpiozero"
PKG_VERSION="2.0.1.post3"
PKG_SHA256="745feab6df463ac2e9de10c67e2dd9f396e668ba4e281e92381d6c460100a8f7"
PKG_ARCH="arm aarch64"
PKG_LICENSE="BSD-3-Clause"
PKG_SITE="https://github.com/RPi-Distro/python-gpiozero"
PKG_URL="https://files.pythonhosted.org/packages/source/${PKG_NAME:0:1}/${PKG_NAME}/${PKG_NAME}-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain colorzero"
PKG_LONGDESC="A simple interface to everyday GPIO components used with Raspberry Pi."
PKG_TOOLCHAIN="manual"
