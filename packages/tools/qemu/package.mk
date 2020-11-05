# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="qemu"
PKG_VERSION="5.1.0"
PKG_SHA256="c9174eb5933d9eb5e61f541cd6d1184cd3118dfe4c5c4955bc1bdc4d390fa4e5"
PKG_LICENSE="GPL"
PKG_SITE="http://wiki.qemu.org"
PKG_URL="https://download.qemu.org/qemu-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_HOST="toolchain:host glib:host pixman:host Python3:host zlib:host"
PKG_LONGDESC="QEMU is a generic and open source machine emulator and virtualizer."
PKG_TOOLCHAIN="configure"

pre_configure_host() {
  HOST_CONFIGURE_OPTS="--bindir=${TOOLCHAIN}/bin \
                       --extra-cflags=-I${TOOLCHAIN}/include \
                       --extra-ldflags=-L${TOOLCHAIN}/lib \
                       --libexecdir=${TOOLCHAIN}/lib \
                       --localstatedir=${TOOLCHAIN}/var \
                       --prefix=${TOOLCHAIN} \
                       --sbindir=${TOOLCHAIN}/sbin \
                       --sysconfdir=${TOOLCHAIN}/etc \
                       --disable-blobs \
                       --disable-docs \
                       --disable-gcrypt \
                       --disable-system \
                       --disable-vnc \
                       --disable-werror \
                       --target-list=${TARGET_ARCH}-linux-user"
}
