# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="btrfs-progs"
PKG_VERSION="6.5.1"
PKG_SHA256="8ea63fe3361dbfb38676e82f69a49a6f23043cc0b7347a46c5c19513d032b8ee"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="https://btrfs.wiki.kernel.org/index.php/Main_Page"
PKG_URL="https://github.com/kdave/btrfs-progs/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain util-linux zlib systemd lzo"
PKG_SECTION="tools"
PKG_SHORTDESC="tools for the btrfs filesystem"
PKG_LONGDESC="tools for the btrfs filesystem"
PKG_TOOLCHAIN="configure"

PKG_BUILD_FLAGS="-sysroot"

PKG_IS_ADDON="yes"
PKG_ADDON_NAME="BTRFS Tools"
PKG_ADDON_TYPE="xbmc.python.script"

PKG_CONFIGURE_OPTS_TARGET="--disable-backtrace \
                           --disable-convert \
                           --disable-documentation \
                           --disable-python \
                           --disable-zstd"

pre_configure_target() {
  ./autogen.sh
}

addon() {
  mkdir -p ${ADDON_BUILD}/${PKG_ADDON_ID}/bin/
    cp -P ${PKG_INSTALL}/usr/bin/{btrfs,btrfsck,btrfstune,fsck.btrfs,mkfs.btrfs} ${ADDON_BUILD}/${PKG_ADDON_ID}/bin
}
