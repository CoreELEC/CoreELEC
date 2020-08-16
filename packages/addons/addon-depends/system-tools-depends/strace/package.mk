# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="strace"
PKG_VERSION="5.8"
PKG_SHA256="df4a669f7fff9cc302784085bd4b72fab216a426a3f72c892b28a537b71e7aa9"
PKG_LICENSE="BSD"
PKG_SITE="https://strace.io/"
PKG_URL="https://strace.io/files/$PKG_VERSION/strace-$PKG_VERSION.tar.xz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="strace is a diagnostic, debugging and instructional userspace utility"
PKG_TOOLCHAIN="autotools"

makeinstall_target() {
  :
}
