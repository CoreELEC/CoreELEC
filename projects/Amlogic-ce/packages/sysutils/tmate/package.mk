# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2022-present Team CoreELEC (https://coreelec.org)

PKG_NAME="tmate"
PKG_VERSION="3e12f558c7b71b7135403cdd2df77d38538a695c"
PKG_SHA256=""
PKG_LICENSE="BSD"
PKG_SITE="https://github.com/tmate-io/tmate"
PKG_URL="https://github.com/tmate-io/tmate/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libevent libssh msgpack-c"
PKG_LONGDESC="Instant terminal sharing."
PKG_TOOLCHAIN="autotools"

post_unpack() {
  # msgpack-c renamed the name
  sed -i "s|msgpack >=|msgpack-c >=|g" ${PKG_BUILD}/configure.ac
}

pre_configure_target() {
  export LIBS+=" -lz -lcrypto"
}

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/bin
  cp tmate ${INSTALL}/usr/bin

  mkdir -p ${INSTALL}/usr/lib/libreelec
  cp ${PKG_DIR}/scripts/* ${INSTALL}/usr/lib/libreelec
}

post_install() {
  enable_service tmate.service
}
