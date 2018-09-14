# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="hauppauge"
PKG_VERSION="baf4593"
PKG_SHA256="bd42e350cd95b3fcdcb1a138d2445c6de595c0b26851d4bbb50c18118bf8389b"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="http://git.linuxtv.org/media_build.git"
PKG_URL="https://git.linuxtv.org/media_build.git/snapshot/${PKG_VERSION}.tar.gz"
PKG_SOURCE_DIR="${PKG_VERSION}"
PKG_DEPENDS_TARGET="toolchain linux media_tree"
PKG_NEED_UNPACK="$LINUX_DEPENDS media_tree"
PKG_SECTION="driver.dvb"
PKG_LONGDESC="DVB drivers from latest Kernel"

PKG_IS_ADDON="embedded"
PKG_IS_KERNEL_PKG="yes"
PKG_ADDON_IS_STANDALONE="yes"
PKG_ADDON_NAME="DVB drivers for Hauppauge"
PKG_ADDON_TYPE="xbmc.service"
PKG_ADDON_VERSION="${ADDON_VERSION}.${PKG_REV}"

if [ $LINUX = "amlogic-3.14" -o $LINUX = "amlogic-3.10" ]; then
  PKG_PATCH_DIRS="amlogic"
elif [ $LINUX = "rockchip-4.4" ]; then
  PKG_PATCH_DIRS="rockchip"
fi

pre_make_target() {
  export KERNEL_VER=$(get_module_dir)
  export LDFLAGS=""
}

make_target() {
  cp -RP $(get_build_dir media_tree)/* $PKG_BUILD/linux
  kernel_make VER=$KERNEL_VER SRCDIR=$(kernel_path) stagingconfig

  # hack to workaround media_build bug
  if [ $LINUX = "amlogic-3.14" -o $LINUX = "amlogic-3.10" ]; then
    sed -e 's/CONFIG_VIDEO_TVP5150=m/# CONFIG_VIDEO_TVP5150 is not set/g' -i $PKG_BUILD/v4l/.config
#    sed -e 's/CONFIG_DVB_LGDT3306A=m/# CONFIG_DVB_LGDT3306A is not set/g' -i $PKG_BUILD/v4l/.config
    sed -e 's/CONFIG_DVB_AF9013=m/# CONFIG_DVB_AF9013 is not set/g' -i $PKG_BUILD/v4l/.config
    sed -e 's/CONFIG_VIDEO_S5C73M3=m/# CONFIG_VIDEO_S5C73M3 is not set/g' -i $PKG_BUILD/v4l/.config
    sed -e 's/CONFIG_VIDEO_MT9T112=m/# CONFIG_VIDEO_MT9T112 is not set/g' -i $PKG_BUILD/v4l/.config
    sed -e 's/CONFIG_SOC_CAMERA_MT9T112=m/# CONFIG_SOC_CAMERA_MT9T112 is not set/g' -i $PKG_BUILD/v4l/.config
    sed -e 's/CONFIG_SOC_CAMERA_OV772X=m/# CONFIG_SOC_CAMERA_OV772X is not set/g' -i $PKG_BUILD/v4l/.config
    sed -e 's/CONFIG_VIDEO_SAA7146_VV=m/# CONFIG_VIDEO_SAA7146_VV is not set/g' -i $PKG_BUILD/v4l/.config
    sed -e 's/CONFIG_VIDEO_OV2659=m/# CONFIG_VIDEO_OV2659 is not set/g' -i $PKG_BUILD/v4l/.config
    sed -e 's/CONFIG_VIDEO_OV5647=m/# CONFIG_VIDEO_OV5647 is not set/g' -i $PKG_BUILD/v4l/.config
    sed -e 's/CONFIG_VIDEO_OV772X=m/# CONFIG_VIDEO_OV772X is not set/g' -i $PKG_BUILD/v4l/.config
    sed -e 's/CONFIG_VIDEO_S5K5BAF=m/# CONFIG_VIDEO_S5K5BAF is not set/g' -i $PKG_BUILD/v4l/.config
    sed -e 's/CONFIG_VIDEO_VIVID=m/# CONFIG_VIDEO_VIVID is not set/g' -i $PKG_BUILD/v4l/.config
    sed -e 's/CONFIG_VIDEO_TVP514X=m/# CONFIG_VIDEO_TVP514X is not set/g' -i $PKG_BUILD/v4l/.config
    sed -e 's/CONFIG_VIDEO_TVP7002=m/# CONFIG_VIDEO_TVP7002 is not set/g' -i $PKG_BUILD/v4l/.config
    sed -e 's/CONFIG_VIDEO_CADENCE_CSI2RX=m/# CONFIG_VIDEO_CADENCE_CSI2RX is not set/g' -i $PKG_BUILD/v4l/.config
    sed -e 's/CONFIG_VIDEO_CADENCE_CSI2TX=m/# CONFIG_VIDEO_CADENCE_CSI2TX is not set/g' -i $PKG_BUILD/v4l/.config
 fi

  kernel_make VER=$KERNEL_VER SRCDIR=$(kernel_path)
}

makeinstall_target() {
  install_driver_addon_files "$PKG_BUILD/v4l/"
}
