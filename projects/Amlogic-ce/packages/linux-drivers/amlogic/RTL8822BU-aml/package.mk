# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="RTL8822BU-aml"
PKG_VERSION="b655ba8114d7def7e6181bfe4635fbb831e2b8d6"
PKG_SHA256="bbb6dab43a0e21654a7796ce07796677bbfda322d74b6a55dd08e3bde4953208"
PKG_ARCH="arm aarch64"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/khadas/android_hardware_wifi_realtek_drivers_8822bu"
PKG_URL="https://github.com/cilynx/rtl88x2BU_WiFi_linux_v5.3.1_27678.20180430_COEX20180427-5959/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain linux"
PKG_NEED_UNPACK="$LINUX_DEPENDS"
PKG_LONGDESC="Realtek RTL8822BU Linux driver"
PKG_IS_KERNEL_PKG="yes"
PKG_TOOLCHAIN="manual"

post_unpack() {
  sed -i 's/-DCONFIG_CONCURRENT_MODE//g; s/^CONFIG_POWER_SAVING.*$/CONFIG_POWER_SAVING = n/g; s/^CONFIG_RTW_DEBUG.*/CONFIG_RTW_DEBUG = n/g' $PKG_BUILD/Makefile
  sed -i 's/^#define CONFIG_DEBUG.*//g' $PKG_BUILD/include/autoconf.h
  sed -i 's/#define DEFAULT_RANDOM_MACADDR.*1/#define DEFAULT_RANDOM_MACADDR 0/g' $PKG_BUILD/core/rtw_ieee80211.c
  sed -i 's/rtw_drv_log_level/0/g' $PKG_BUILD/core/*.c
}

pre_make_target() {
  unset LDFLAGS
}

make_target() {
  make  \
    ARCH=$TARGET_KERNEL_ARCH \
    KSRC=$(kernel_path) \
    CROSS_COMPILE=$TARGET_KERNEL_PREFIX \
    USER_EXTRA_CFLAGS="-fgnu89-inline"
}

makeinstall_target() {
  mkdir -p $INSTALL/$(get_full_module_dir)/$PKG_NAME
    find $PKG_BUILD/ -name \*.ko -not -path '*/\.*' -exec cp {} $INSTALL/$(get_full_module_dir)/$PKG_NAME \;
}
