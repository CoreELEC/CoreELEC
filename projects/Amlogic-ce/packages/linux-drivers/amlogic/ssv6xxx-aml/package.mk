# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="ssv6xxx-aml"
PKG_VERSION="d1708e72e7e69ed2b29aa7a48d05be76544c8f1d"
PKG_SHA256="4d8ec5677bd7e61c0ec87deef723433f4c5da927de1df4c4d37fed1507303846"
PKG_ARCH="arm aarch64"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/khadas/android_hardware_wifi_icomm_drivers_ssv6xxx/"
PKG_URL="https://github.com/khadas/android_hardware_wifi_icomm_drivers_ssv6xxx/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain linux"
PKG_NEED_UNPACK="$LINUX_DEPENDS"
PKG_LONGDESC="ssv6xxx Linux driver"
PKG_IS_KERNEL_PKG="yes"
PKG_TOOLCHAIN="manual"

pre_make_target() {
  unset LDFLAGS
}

make_target() {
  if [ "$TARGET_KERNEL_ARCH" = "arm64" ]; then
    PLATFORM="aml-s905"
  else
    PLATFORM="aml-s805"
  fi

  cd $PKG_BUILD/ssv6051
    ./ver_info.pl include/ssv_version.h
    cp Makefile.android Makefile
    sed -i 's,PLATFORMS =,PLATFORMS = '"$PLATFORM"',g' Makefile
    make module SSV_ARCH="$TARGET_KERNEL_ARCH" \
      SSV_CROSS="$TARGET_KERNEL_PREFIX" \
      SSV_KERNEL_PATH="$(kernel_path)"
}

makeinstall_target() {
  mkdir -p $INSTALL/$(get_full_module_dir)/$PKG_NAME
    find $PKG_BUILD/ssv6051/ -name \*.ko -not -path '*/\.*' -exec cp {} $INSTALL/$(get_full_module_dir)/$PKG_NAME \;

  mkdir -p $INSTALL/$(get_full_firmware_dir)/ssv6051
    cp -r $PKG_BUILD/ssv6051/image/* $INSTALL/$(get_full_firmware_dir)/ssv6051
}
