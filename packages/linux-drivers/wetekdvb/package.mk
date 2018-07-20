# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)

PKG_NAME="wetekdvb"
PKG_VERSION="20170608"
PKG_SHA256="e24edb695e0decfc027121833e960346752631b71aa082787b2cd9fdca263ed6"
PKG_ARCH="arm aarch64"
PKG_LICENSE="nonfree"
PKG_SITE="http://www.wetek.com/"
PKG_URL="$DISTRO_SRC/$PKG_NAME-$PKG_VERSION.tar.xz"
PKG_DEPENDS_TARGET="toolchain linux"
PKG_NEED_UNPACK="$LINUX_DEPENDS"
PKG_SECTION="driver"
PKG_SHORTDESC="wetekdvb: Wetek DVB driver"
PKG_LONGDESC="These package contains Wetek's DVB driver "
PKG_IS_KERNEL_PKG="yes"
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  if [ $PROJECT = "WeTek_Play_2" -o  $DEVICE = "S905" ]; then
    mkdir -p $INSTALL/$(get_full_firmware_dir)
    cp firmware/* $INSTALL/$(get_full_firmware_dir)
  fi
}

post_install() {
  enable_service wetekdvb.service
}

