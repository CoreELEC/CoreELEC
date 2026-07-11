# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2024-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="cifs-utils"
PKG_VERSION="7.7"
PKG_SHA256="2f8aae9aa5ddd73fbaf4d61e2e5e19ef54124cbf2810b626820050e7bd2f6606"
PKG_LICENSE="GPL-3.0-or-later"
PKG_SITE="https://wiki.samba.org/index.php/LinuxCIFS_utils"
PKG_URL="https://download.samba.org/pub/linux-cifs/cifs-utils/cifs-utils-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_TARGET="toolchain talloc"
PKG_LONGDESC="Linux CIFS userspace utilities"
PKG_BUILD_FLAGS="-cfg-libs"
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_TARGET="ac_cv_func_malloc_0_nonnull=yes \
                           ac_cv_func_realloc_0_nonnull=yes \
                           --disable-cifsupcall \
                           --disable-cifscreds \
                           --disable-cifsidmap \
                           --disable-cifsacl \
                           --disable-smbinfo \
                           --disable-pythontools \
                           --disable-pam \
                           --disable-man \
                           --disable-systemd"

makeinstall_target() {
  mkdir -p "${INSTALL}/usr/sbin/"
    cp -PR mount.cifs "${INSTALL}/usr/sbin/"
    ln -s mount.cifs "${INSTALL}/usr/sbin/mount.smb3"
}
