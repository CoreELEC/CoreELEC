################################################################################
#      This file is part of LibreELEC - https://libreelec.tv
#      Copyright (C) 2016-present Team LibreELEC
#
#  LibreELEC is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 2 of the License, or
#  (at your option) any later version.
#
#  LibreELEC is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with LibreELEC.  If not, see <http://www.gnu.org/licenses/>.
################################################################################

PKG_NAME="hauppauge"
PKG_VERSION="f334f01"
PKG_SHA256="f7ccf652d90a51bd709f9d84a286a4e468c62fe50dc216473eb43ffdc187b0a9"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="http://git.linuxtv.org/media_build.git"
PKG_URL="https://git.linuxtv.org/media_build.git/snapshot/${PKG_VERSION}.tar.gz"
PKG_SOURCE_DIR="${PKG_VERSION}"
PKG_DEPENDS_TARGET="toolchain linux media_tree"
PKG_NEED_UNPACK="$LINUX_DEPENDS media_tree"
PKG_SECTION="driver.dvb"
PKG_LONGDESC="DVB drivers for Hauppauge"

PKG_IS_ADDON="yes"
PKG_IS_KERNEL_PKG="yes"
PKG_ADDON_IS_STANDALONE="yes"
PKG_ADDON_NAME="DVB drivers for Hauppauge"
PKG_ADDON_TYPE="xbmc.service"
PKG_ADDON_VERSION="${ADDON_VERSION}.${PKG_REV}"

if [ $LINUX = "amlogic-3.14" ]; then
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
  make VER=$KERNEL_VER SRCDIR=$(kernel_path) stagingconfig

  # hack to workaround media_build bug
  if [ $LINUX = "amlogic-3.14" ]; then
    sed -e 's/CONFIG_VIDEO_TVP5150=m/# CONFIG_VIDEO_TVP5150 is not set/g' -i $PKG_BUILD/v4l/.config
    sed -e 's/CONFIG_DVB_LGDT3306A=m/# CONFIG_DVB_LGDT3306A is not set/g' -i $PKG_BUILD/v4l/.config
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
  fi

  make VER=$KERNEL_VER SRCDIR=$(kernel_path)
}

makeinstall_target() {
  install_driver_addon_files "$PKG_BUILD/v4l/"
}
