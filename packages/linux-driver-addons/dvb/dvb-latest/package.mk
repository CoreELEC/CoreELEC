# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="dvb-latest"
PKG_VERSION="a8aef9cea0a4a2f3ea86c0b37bd6a1378018c0c1"
PKG_SHA256="a26566b1f8ad52f2a1b3c60f0ee8862702e51e2a7bc9e5385999137db0da3fe3"
PKG_LICENSE="GPL"
PKG_SITE="http://git.linuxtv.org/media_build.git"
PKG_URL="https://git.linuxtv.org/media_build.git/snapshot/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain linux media_tree"
PKG_NEED_UNPACK="$LINUX_DEPENDS media_tree"
PKG_SECTION="driver.dvb"
PKG_LONGDESC="DVB drivers from the latest kernel (media_build)"

PKG_IS_ADDON="embedded"
PKG_IS_KERNEL_PKG="yes"
PKG_ADDON_IS_STANDALONE="yes"
PKG_ADDON_NAME="DVB drivers from the latest kernel"
PKG_ADDON_TYPE="xbmc.service"
PKG_ADDON_VERSION="${ADDON_VERSION}.${PKG_REV}"

if [ "$PROJECT" = "Amlogic" ]; then
  PKG_PATCH_DIRS="amlogic"
  PKG_DEPENDS_TARGET="$PKG_DEPENDS_TARGET media_tree_aml"
  PKG_NEED_UNPACK="$PKG_NEED_UNPACK media_tree_aml"
fi

pre_make_target() {
  export KERNEL_VER=$(get_module_dir)
  export LDFLAGS=""
}

make_target() {
  cp -RP $(get_build_dir media_tree)/* $PKG_BUILD/linux
  if [ "$PROJECT" = "Amlogic" ]; then
    cp -Lr $(get_build_dir media_tree_aml)/* $PKG_BUILD/linux
    echo "obj-y += video_dev/" >> "$PKG_BUILD/linux/drivers/media/platform/meson/Makefile"
    echo "obj-y += dvb/" >> "$PKG_BUILD/linux/drivers/media/platform/meson/Makefile"
  fi 

  # make config all
  kernel_make VER=$KERNEL_VER SRCDIR=$(kernel_path) allyesconfig

  # hack to workaround media_build bug
  if [ "$PROJECT" = "Amlogic" ]; then
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
  if [ "$PROJECT" = Rockchip ]; then
    sed -e 's/CONFIG_VIDEO_ADV7604=m/# CONFIG_VIDEO_ADV7604 is not set/g' -i v4l/.config
  fi

  kernel_make VER=$KERNEL_VER SRCDIR=$(kernel_path)
}

makeinstall_target() {
  install_driver_addon_files "$PKG_BUILD/v4l/"
}
