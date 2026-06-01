# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="quilt"
PKG_VERSION="0.69"
PKG_SHA256="555ddffde22da3c86d1caf5a9c1fb8a152ac2b84730437bd39cc08849c9f4852"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://savannah.nongnu.org/projects/quilt"
PKG_URL="https://download.savannah.gnu.org/releases/quilt/${PKG_NAME}-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="ccache:host gettext:host"
PKG_LONGDESC="Scripts for working with series of patches."
PKG_BUILD_FLAGS="-cfg-libs:host"

pre_configure_host() {
  cd ${PKG_BUILD}
  rm -rf .${HOST_NAME}
}
