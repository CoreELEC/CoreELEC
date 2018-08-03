# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="crazycat"
PKG_VERSION="899606d"
PKG_SHA256="79e525259e55b3acb969a438e6be2568aa17437c7bb95bdb4da3360dd0f9153d"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="https://bitbucket.org/CrazyCat/media_build"
PKG_URL="https://github.com/CoreELEC/crazycat/archive/$PKG_VERSION.tar.gz"
PKG_SOURCE_DIR="crazycat-$PKG_VERSION*"
PKG_DEPENDS_TARGET="toolchain linux"
PKG_NEED_UNPACK="$LINUX_DEPENDS"
PKG_SECTION="driver.dvb"
PKG_LONGDESC="DVB driver for TBS cards with CrazyCats additions."

PKG_IS_ADDON="embedded"
PKG_IS_KERNEL_PKG="yes"
PKG_ADDON_IS_STANDALONE="yes"
PKG_ADDON_NAME="DVB drivers for TBS (CrazyCat)"
PKG_ADDON_TYPE="xbmc.service"
PKG_ADDON_VERSION="${ADDON_VERSION}.${PKG_REV}"

if [ $LINUX = "amlogic-3.10" ]; then
  PKG_PATCH_DIRS="amlogic-3.10"
elif [ $LINUX = "amlogic-3.14" ]; then
  PKG_PATCH_DIRS="amlogic-3.14"
elif [ $LINUX = "rockchip-4.4" ]; then
  PKG_PATCH_DIRS="rockchip"
fi

pre_make_target() {
  export KERNEL_VER=$(get_module_dir)
  export LDFLAGS=""
}

make_target() {
  if listcontains "$ADDITIONAL_DRIVERS" "wetekdvb"; then
    kernel_make SRCDIR=$(kernel_path) WETEKSRCDIR=$(get_build_dir wetekdvb) untar
  else
    kernel_make SRCDIR=$(kernel_path) untar
  fi

  # copy config file
  if [ "$PROJECT" = Generic ]; then
    if [ -f $PKG_DIR/config/generic.config ]; then
      cp $PKG_DIR/config/generic.config v4l/.config
    fi
  else
    if [ -f $PKG_DIR/config/usb.config ]; then
      cp $PKG_DIR/config/usb.config v4l/.config
    fi
  fi

  # hack to workaround media_build bug
  if [ $LINUX = "amlogic-3.14" -o $LINUX = "amlogic-3.10" ]; then
    sed -e 's/CONFIG_VIDEO_TVP5150=m/# CONFIG_VIDEO_TVP5150 is not set/g' -i v4l/.config
    sed -e 's/CONFIG_DVB_LGDT3306A=m/# CONFIG_DVB_LGDT3306A is not set/g' -i v4l/.config
    sed -e 's/CONFIG_DVB_AF9013=m/# CONFIG_DVB_AF9013 is not set/g' -i v4l/.config
    if [ $LINUX = "amlogic-3.10" ]; then
      sed -e 's/CONFIG_IR_NUVOTON=m/# CONFIG_IR_NUVOTON is not set/g' -i v4l/.config
    fi
  elif [ "$PROJECT" = Rockchip ]; then
    sed -e 's/CONFIG_DVB_CXD2820R=m/# CONFIG_DVB_CXD2820R is not set/g' -i v4l/.config
    sed -e 's/CONFIG_DVB_LGDT3306A=m/# CONFIG_DVB_LGDT3306A is not set/g' -i v4l/.config
    sed -e 's/CONFIG_DVB_AF9013=m/# CONFIG_DVB_AF9013 is not set/g' -i v4l/.config
  fi

  # add menuconfig to edit .config
  kernel_make VER=$KERNEL_VER SRCDIR=$(kernel_path)
}

makeinstall_target() {
  install_driver_addon_files "$PKG_BUILD/v4l/"
}
