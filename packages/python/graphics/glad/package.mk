# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="glad"
PKG_VERSION="2.0.5"
PKG_SHA256="850351f1960f3fed775f0b696d7f17615f306beb56be38a20423284627626df1"
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
