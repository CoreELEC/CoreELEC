# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="crazycat"
PKG_VERSION="f77791e13e0a29edeb775383f89e37bb1ed80416"
PKG_SHA256="524a5cdbbb653b0db46d20b56249953e473302bd20e8097bf06e3918e5c0a35f"
PKG_LICENSE="GPL"
PKG_SITE="https://bitbucket.org/CrazyCat/media_build"
PKG_URL="https://bitbucket.org/CrazyCat/media_build/get/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain linux media_tree_cc"
PKG_NEED_UNPACK="$LINUX_DEPENDS media_tree_cc"
PKG_SECTION="driver.dvb"
PKG_LONGDESC="DVB driver for TBS cards with CrazyCats additions"

PKG_IS_ADDON="embedded"
PKG_IS_KERNEL_PKG="yes"
PKG_ADDON_IS_STANDALONE="yes"
PKG_ADDON_NAME="DVB drivers for TBS"
PKG_ADDON_TYPE="xbmc.service"
PKG_ADDON_VERSION="${ADDON_VERSION}.${PKG_REV}"

configure_package() {
  if [ "$PROJECT" = "Amlogic" ]; then
    PKG_PATCH_DIRS="amlogic"
    PKG_DEPENDS_TARGET="$PKG_DEPENDS_TARGET media_tree_aml"
    PKG_NEED_UNPACK="$PKG_NEED_UNPACK media_tree_aml"
  fi
}

pre_make_target() {
  export KERNEL_VER=$(get_module_dir)
  export LDFLAGS=""
}

make_target() {
  cp -RP $(get_build_dir media_tree_cc)/* $PKG_BUILD/linux
  if [ "$PROJECT" = "Amlogic" ]; then
    cp -RP $(get_build_dir media_tree_aml)/* $PKG_BUILD/linux
    echo "obj-y += video_dev/" >> "$PKG_BUILD/linux/drivers/media/platform/meson/Makefile"
    echo "obj-y += dvb-avl/" >> "$PKG_BUILD/linux/drivers/media/platform/meson/Makefile"
    echo "obj-y += wetek/" >> "$PKG_BUILD/linux/drivers/media/platform/meson/Makefile"
  fi

  # make config all
  kernel_make VER=$KERNEL_VER SRCDIR=$(kernel_path) allyesconfig

  # hack to workaround media_build bug
  if [ "$PROJECT" = Rockchip ]; then
    sed -e 's/CONFIG_DVB_CXD2820R=m/# CONFIG_DVB_CXD2820R is not set/g' -i v4l/.config
    sed -e 's/CONFIG_DVB_LGDT3306A=m/# CONFIG_DVB_LGDT3306A is not set/g' -i v4l/.config
  elif [ "$PROJECT" = "Amlogic" ]; then
    sed -e 's/CONFIG_DVB_LGDT3306A=m/# CONFIG_DVB_LGDT3306A is not set/g' -i v4l/.config
    sed -e 's/CONFIG_VIDEO_S5C73M3=m/# CONFIG_VIDEO_S5C73M3 is not set/g' -i $PKG_BUILD/v4l/.config
    sed -e 's/CONFIG_VIDEO_SAA7146_VV=m/# CONFIG_VIDEO_SAA7146_VV is not set/g' -i $PKG_BUILD/v4l/.config
    sed -e 's/CONFIG_VIDEO_OV2659=m/# CONFIG_VIDEO_OV2659 is not set/g' -i $PKG_BUILD/v4l/.config
    sed -e 's/CONFIG_VIDEO_OV5647=m/# CONFIG_VIDEO_OV5647 is not set/g' -i $PKG_BUILD/v4l/.config
    sed -e 's/CONFIG_VIDEO_S5K5BAF=m/# CONFIG_VIDEO_S5K5BAF is not set/g' -i $PKG_BUILD/v4l/.config
    sed -e 's/CONFIG_VIDEO_VIVID=m/# CONFIG_VIDEO_VIVID is not set/g' -i $PKG_BUILD/v4l/.config
    sed -e 's/CONFIG_VIDEO_TVP514X=m/# CONFIG_VIDEO_TVP514X is not set/g' -i $PKG_BUILD/v4l/.config
    sed -e 's/CONFIG_VIDEO_TVP7002=m/# CONFIG_VIDEO_TVP7002 is not set/g' -i $PKG_BUILD/v4l/.config
    sed -e 's/CONFIG_VIDEO_CADENCE_CSI2RX=m/# CONFIG_VIDEO_CADENCE_CSI2RX is not set/g' -i $PKG_BUILD/v4l/.config
    sed -e 's/CONFIG_VIDEO_CADENCE_CSI2TX=m/# CONFIG_VIDEO_CADENCE_CSI2TX is not set/g' -i $PKG_BUILD/v4l/.config
    sed -e 's/# CONFIG_MEDIA_TUNER_TDA18250 is not set/CONFIG_MEDIA_TUNER_TDA18250=m/g' -i $PKG_BUILD/v4l/.config
  fi

  # add menuconfig to edit .config
  kernel_make VER=$KERNEL_VER SRCDIR=$(kernel_path)
}

makeinstall_target() {
  install_driver_addon_files "$PKG_BUILD/v4l/"
}

