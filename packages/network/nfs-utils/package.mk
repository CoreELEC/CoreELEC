# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2024-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="nfs-utils"
PKG_VERSION="2.7.1"
PKG_SHA256="885c948a84a58bca4148f459588f9a7369dbb40dcc466f04e455c6b10fd0aa48"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="http://www.linux-nfs.org/"
PKG_URL="https://www.kernel.org/pub/linux/utils/nfs-utils/${PKG_VERSION}/nfs-utils-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain keyutils libevent libtirpc rpcbind sqlite util-linux"
PKG_LONGDESC="Linux NFS userland utility package"

PKG_CONFIGURE_OPTS_TARGET="--disable-gss \
                           --disable-nfsv41 \
                           --disable-nfsdcld \
                           --disable-nfsrahead \
                           --disable-nfsdcltrack \
                           --disable-ldap"

pre_configure_target() {
  cd ${PKG_BUILD}
  rm -rf .${TARGET_NAME}
}

makeinstall_target() {
  mkdir -p "${INSTALL}/usr/sbin/"
    cp -PR utils/mount/mount.nfs "${INSTALL}/usr/sbin/"
    ln -s mount.nfs "${INSTALL}/usr/sbin/mount.nfs4"
    ln -s mount.nfs "${INSTALL}/usr/sbin/umount.nfs"
    ln -s mount.nfs "${INSTALL}/usr/sbin/umount.nfs4"
}
