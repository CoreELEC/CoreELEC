# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018 Arthur Liberman (arthur_liberman@hotmail.com)

PKG_NAME="openvfd"
PKG_VERSION="2df85d9"
PKG_SHA256="f45e6a0293a9c49610af36f1ae6846cc6cdf77d90c9e7b6ec21cb1d0f23d11f4"
PKG_ARCH="arm aarch64"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/arthur-liberman/linux_openvfd"
PKG_URL="https://github.com/arthur-liberman/linux_openvfd/archive/$PKG_VERSION.tar.gz"
PKG_SOURCE_DIR="linux_openvfd-$PKG_VERSION*"
PKG_DEPENDS_TARGET="toolchain linux"
PKG_NEED_UNPACK="$LINUX_DEPENDS"
PKG_SECTION="driver"
PKG_SHORTDESC="openvfd: Driver for VFD displays"
PKG_LONGDESC="openvfd: Driver for VFD displays"

PKG_TOOLCHAIN="manual"

pre_make_target() {
  unset LDFLAGS
}

make_target() {
  kernel_make -C "$(kernel_path)" M="$PKG_BUILD/driver"

  make OpenVFDService
}

makeinstall_target() {
  mkdir -p $INSTALL/$(get_full_module_dir)/$PKG_NAME
    find $PKG_BUILD/ -name \*.ko -not -path '*/\.*' -exec cp {} $INSTALL/$(get_full_module_dir)/$PKG_NAME \;

  mkdir -p $INSTALL/usr/sbin
    cp -P OpenVFDService $INSTALL/usr/sbin
}

post_install() {
  enable_service openvfd.service
}
