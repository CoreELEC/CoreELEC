# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="fuse3"
PKG_VERSION="3.18.3"
PKG_SHA256="bcd19582c5e30f7fe45dd86a5540e998590aa01903afc7ebcbeea6c8ac5421ee"
PKG_LICENSE="LGPL-2.1-only"
PKG_SITE="https://github.com/libfuse/libfuse/"
PKG_URL="https://github.com/libfuse/libfuse/releases/download/fuse-${PKG_VERSION}/fuse-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain systemd"
PKG_LONGDESC="FUSE is an interface for userspace programs to export a filesystem to the Linux kernel."

PKG_MESON_OPTS_TARGET="-Ddisable-mtab=false \
                       -Dutils=true \
                       -Dexamples=false \
                       -Duseroot=false \
                       -Dtests=false"

post_makeinstall_target() {
  rm -rf ${INSTALL}/etc
  rm -rf ${INSTALL}/usr/lib/udev
}
