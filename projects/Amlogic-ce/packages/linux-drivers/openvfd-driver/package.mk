# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Arthur Liberman (arthur_liberman@hotmail.com)

PKG_NAME="openvfd-driver"
PKG_VERSION="e6f57b329d2cfe4bc4670ad10a68a21bcbf3d846"
PKG_SHA256="e8c5c136900b9b47d93c7770d20ab9ee5b67d2ba05afff6005160f6d08b8b348"
PKG_LICENSE="GPLv3"
PKG_SITE="https://github.com/arthur-liberman/linux_openvfd"
PKG_URL="https://github.com/arthur-liberman/linux_openvfd/archive/$PKG_VERSION.tar.gz"
PKG_SOURCE_DIR="linux_openvfd-$PKG_VERSION*"
PKG_DEPENDS_TARGET="toolchain linux"
PKG_NEED_UNPACK="$LINUX_DEPENDS"
PKG_LONGDESC="openvfd-driver: an open source Linux driver for VFD displays"
PKG_TOOLCHAIN="manual"

make_target() {
  kernel_make -C "$(kernel_path)" M="$PKG_BUILD/driver"

  CFLAGS+=" -Wno-implicit-function-declaration"
  make OpenVFDService
}

makeinstall_target() {
  mkdir -p $INSTALL/$(get_full_module_dir)/$PKG_NAME
    find $PKG_BUILD/ -name \*.ko -not -path '*/\.*' -exec cp {} $INSTALL/$(get_full_module_dir)/$PKG_NAME \;

  mkdir -p $INSTALL/usr/sbin
    cp -P OpenVFDService $INSTALL/usr/sbin

  mkdir -p $INSTALL/usr/lib/coreelec
    cp $PKG_DIR/scripts/* $INSTALL/usr/lib/coreelec/

  mkdir -p $INSTALL/etc/openvfd.conf.d/
    cp $PKG_DIR/openvfd.conf.d/* $INSTALL/etc/openvfd.conf.d/
}

post_install() {
  enable_service openvfd.service
}
