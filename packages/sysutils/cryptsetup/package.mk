# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020 Jeff Doozan <github@doozan.com>
# Copyright (C) 2026-present Team CoreELEC (https://coreelec.org)

PKG_NAME="cryptsetup"
PKG_VERSION="2.8.8"
PKG_SHA256="3acfa685f2dd7fcc832e0b77bc7093aa7da554a51ce8dafbb4138eaa854eee35"
PKG_LICENSE="GPL"
PKG_URL="https://www.kernel.org/pub/linux/utils/cryptsetup/v${PKG_VERSION%.*}/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_HOST="toolchain ccache:host"
PKG_DEPENDS_TARGET="toolchain popt libdevmapper util-linux json-c libssh openssl"
PKG_LONGDESC="cryptsetup utility for managing LUKS containers"

PKG_MESON_OPTS_TARGET=" \
  -Dintegritysetup=false \
  -Dveritysetup=false \
  -Dudev=false \
  -Dasciidoc=disabled \
  -Dblkid=true"

pre_configure_target() {
  export TARGET_CFLAGS+=" -Wno-unused-variable -Wno-format-truncation"
  export TARGET_LDFLAGS+=" -lcrypto -lz"
}
