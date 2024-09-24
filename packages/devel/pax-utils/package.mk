# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="pax-utils"
PKG_VERSION="1.3.8"
PKG_SHA256="3d826b7fa1d47003c0e8f68f5bb9848cb70bddfc1f0190492533ccea6d696f24"
PKG_LICENSE="GPL-2.0"
PKG_SITE="https://wiki.gentoo.org/wiki/Hardened/PaX_Utilities"
PKG_URL="https://gitweb.gentoo.org/proj/pax-utils.git/snapshot/pax-utils-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_HOST="meson:host ninja:host"
PKG_LONGDESC="ELF utils that can check files for security relevant properties"

PKG_MESON_OPTS_HOST="-Duse_libcap=disabled"
