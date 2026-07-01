# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020 Jeff Doozan <github@doozan.com>

PKG_NAME="cryptsetup"
PKG_MAJOR="2.8"
PKG_VERSION="$PKG_MAJOR.6"
PKG_LICENSE="GPL"
PKG_URL="https://www.kernel.org/pub/linux/utils/cryptsetup/v$PKG_MAJOR/$PKG_NAME-$PKG_VERSION.tar.xz"
PKG_SHA256="8004265fd993885d08f7b633dbe056851de1a210307613a4ebddc743fccefe5a"
PKG_LONGDESC="cryptsetup utility for managing LUKS containers"
PKG_DEPENDS_HOST="toolchain ccache:host"
PKG_DEPENDS_TARGET="toolchain popt libdevmapper util-linux json-c libssh openssl"
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_TARGET="
        --disable-cryptsetup-reencrypt \
        --disable-integritysetup \
        --disable-selinux \
        --disable-rpath \
        --disable-veritysetup \
        --disable-udev \
        --disable-asciidoc \
        --enable-blkid"

post_unpack() {
  # copy files to required subfolder
  mkdir -p ${SYSROOT_PREFIX}/usr/include/json-c
  ln -sf ../json.h ${SYSROOT_PREFIX}/usr/include/json-c/json.h
  cp -a $(get_build_dir json-c)/json_*.h ${SYSROOT_PREFIX}/usr/include/json-c
}

pre_configure_target() {
  export JSON_C_CFLAGS="-I${SYSROOT_PREFIX}/usr/include"
  export JSON_C_LIBS="-L${SYSROOT_PREFIX}/usr/lib -ljson-c"
  export LIBSSH_LIBS="-L${SYSROOT_PREFIX}/usr/lib -lssh -lz"
}
