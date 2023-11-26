# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="glad"
PKG_VERSION="2.0.4"
PKG_SHA256="02629644c242dcc27c58222bd2c001d5e2f3765dbbcfd796542308bddebab401"
PKG_LICENSE="MIT"
PKG_SITE="https://glad.dav1d.de"
PKG_URL="https://github.com/Dav1dde/glad/archive/refs/tags/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Multi-Language Vulkan/GL/GLES/EGL/GLX/WGL Loader-Generator based on the official specs"
PKG_TOOLCHAIN="manual"

makeinstall_host() {
  export DONT_BUILD_LEGACY_PYC=1
  exec_thread_safe python3 setup.py install --prefix=${TOOLCHAIN}
}
