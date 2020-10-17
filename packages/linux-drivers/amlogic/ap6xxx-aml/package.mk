# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2017-2018 Team LibreELEC (https://libreelec.tv)
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="ap6xxx-aml"
PKG_VERSION="ba2fbd0174b744521f4a25da796b9f62e9e4f7a7"
PKG_SHA256="60d9f755d2b4d2ae97ac09d92311d527718394e0c02af46b9cdcf30a0dc497b8"
PKG_ARCH="arm aarch64"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/CoreELEC/ap6xxx-aml"
PKG_URL="https://github.com/CoreELEC/ap6xxx-aml/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain linux"
PKG_NEED_UNPACK="$LINUX_DEPENDS"
PKG_LONGDESC="ap6xxx: Linux drivers for AP6xxx WLAN chips used in some devices based on Amlogic SoCs"
PKG_IS_KERNEL_PKG="yes"
PKG_TOOLCHAIN="manual"

pre_make_target() {
  unset LDFLAGS
}

make_target() {
  echo
  echo "building ap6356s and others"
  make -C  $PKG_BUILD/bcmdhd.1.363.59.144.x.cn \
       PWD=$PKG_BUILD/bcmdhd.1.363.59.144.x.cn \
       KDIR=$(kernel_path) \
       ARCH=$TARGET_KERNEL_ARCH \
       CROSS_COMPILE=$TARGET_KERNEL_PREFIX \
       CONFIG_BCMDHD_DISABLE_WOWLAN=y \
       clean dhd

  if [ "$PROJECT" = "Amlogic-ng" ]; then
    echo
    echo "building ap6275s"
    make -C  $PKG_BUILD/bcmdhd.100.10.315.x \
         PWD=$PKG_BUILD/bcmdhd.100.10.315.x \
         KDIR=$(kernel_path) \
         ARCH=$TARGET_KERNEL_ARCH \
         CROSS_COMPILE=$TARGET_KERNEL_PREFIX \
         CONFIG_BCMDHD_DISABLE_WOWLAN=y \
         clean bcmdhd_sdio
  fi
}

makeinstall_target() {
  mkdir -p $INSTALL/$(get_full_module_dir)/$PKG_NAME

  find $PKG_BUILD/ -name \*.ko -not -path '*/\.*' \
    -exec cp {} $INSTALL/$(get_full_module_dir)/$PKG_NAME \;
}

post_install() {
  if [ "$PROJECT" = "Amlogic" ]; then
    rm $INSTALL/usr/lib/modprobe.d/dhd_sdio.conf
    rm $INSTALL/usr/lib/udev/rules.d/80-dhd_sdio.rules
  fi
}
