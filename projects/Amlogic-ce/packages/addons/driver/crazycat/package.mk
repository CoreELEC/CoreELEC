# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-2018 Team LibreELEC (https://libreelec.tv)
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="crazycat"
PKG_VERSION="ca1ea9fc2cfaedfc32bd0ac628e03e9aa379e3ad"
PKG_SHA256="6b44a96d82c4a3e052864a995baceaede46b37c048c5718a6f62a009492d08ff"
PKG_LICENSE="GPL"
PKG_SITE="https://bitbucket.org/CrazyCat/media_build"
PKG_URL="https://bitbucket.org/CrazyCat/media_build/get/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain linux media_tree_cc"
PKG_NEED_UNPACK="$LINUX_DEPENDS $(get_pkg_directory media_tree_cc)"
PKG_SECTION="driver.dvb"
PKG_LONGDESC="DVB driver for TBS cards with CrazyCats additions"

PKG_IS_ADDON="embedded"
PKG_IS_KERNEL_PKG="yes"
PKG_ADDON_IS_STANDALONE="yes"
PKG_ADDON_NAME="DVB drivers for TBS"
PKG_ADDON_TYPE="xbmc.service"
PKG_ADDON_VERSION="${ADDON_VERSION}.${PKG_REV}"

if [ "$PROJECT" = "Amlogic-ng" ]; then
  PKG_PATCH_DIRS="amlogic-4.9"
  PKG_DEPENDS_TARGET="$PKG_DEPENDS_TARGET media_tree_aml"
  PKG_NEED_UNPACK="$PKG_NEED_UNPACK $(get_pkg_directory media_tree_aml)"
fi

pre_make_target() {
  export KERNEL_VER=$(get_module_dir)
  export LDFLAGS=""
}

make_target() {
  cp -RP $(get_build_dir media_tree_cc)/* $PKG_BUILD/linux
  if [ "$PROJECT" = "Amlogic-ng" ]; then
    cp -Lr $(get_build_dir media_tree_aml)/* $PKG_BUILD/linux
    echo "obj-y += video_dev/" >> "$PKG_BUILD/linux/drivers/media/platform/meson/Makefile"
    echo "obj-y += dvb/" >> "$PKG_BUILD/linux/drivers/media/platform/meson/Makefile"
    echo 'source "drivers/media/platform/meson/dvb/Kconfig"' >>  "$PKG_BUILD/linux/drivers/media/platform/Kconfig"
    sed -e 's/ && RC_CORE//g' -i $PKG_BUILD/linux/drivers/media/usb/dvb-usb/Kconfig
  fi

  # make config all
  kernel_make VER=$KERNEL_VER SRCDIR=$(kernel_path) allyesconfig

  # hack to workaround media_build bug
  if [ "$PROJECT" = Rockchip ]; then
    sed -e 's/CONFIG_DVB_CXD2820R=m/# CONFIG_DVB_CXD2820R is not set/g' -i v4l/.config
    sed -e 's/CONFIG_DVB_LGDT3306A=m/# CONFIG_DVB_LGDT3306A is not set/g' -i v4l/.config
#  elif [ "$PROJECT" = "Amlogic-ng" ]; then
#    sed -e 's/# CONFIG_MEDIA_TUNER_TDA18250 is not set/CONFIG_MEDIA_TUNER_TDA18250=m/g' -i $PKG_BUILD/v4l/.config
  fi

  # add menuconfig to edit .config
  kernel_make VER=$KERNEL_VER SRCDIR=$(kernel_path)
}

makeinstall_target() {
  install_driver_addon_files "$PKG_BUILD/v4l/"
}
