# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Arthur Liberman (arthur_liberman@hotmail.com)

PKG_NAME="openvfd-driver"
PKG_VERSION="b29330a7ef534a43a838e25778d423cddfba6e29"
PKG_SHA256="430f09877d93c2b02f922c94489376ddf1936dd1af6c631e27f4ffafd8b39fa9"
PKG_LICENSE="GPLv3"
PKG_SITE="https://github.com/arthur-liberman/linux_openvfd"
PKG_URL="https://github.com/arthur-liberman/linux_openvfd/archive/$PKG_VERSION.tar.gz"
PKG_SOURCE_DIR="linux_openvfd-$PKG_VERSION*"
PKG_DEPENDS_TARGET="toolchain linux"
PKG_NEED_UNPACK="$LINUX_DEPENDS"
PKG_LONGDESC="openvfd-driver: an open source Linux driver for VFD displays"
PKG_TOOLCHAIN="manual"

pre_make_target() {
  unset LDFLAGS
}

make_target() {
  make ARCH=$TARGET_KERNEL_ARCH \
       CROSS_COMPILE=$TARGET_KERNEL_PREFIX \
       -C "$(kernel_path)" M="$PKG_BUILD/driver"

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
