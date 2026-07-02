# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020 Jeff Doozan <github@doozan.com>
# Copyright (C) 2026-present Team CoreELEC (https://coreelec.org)

PKG_NAME="cryptsetup"
PKG_MAJOR="2.8"
PKG_VERSION="$PKG_MAJOR.6"
PKG_LICENSE="GPL"
PKG_URL="https://www.kernel.org/pub/linux/utils/cryptsetup/v$PKG_MAJOR/$PKG_NAME-$PKG_VERSION.tar.xz"
PKG_SHA256="8004265fd993885d08f7b633dbe056851de1a210307613a4ebddc743fccefe5a"
PKG_LONGDESC="cryptsetup utility for managing LUKS containers"
PKG_DEPENDS_HOST="toolchain ccache:host"
PKG_DEPENDS_TARGET="toolchain popt libdevmapper util-linux json-c libssh openssl"

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
