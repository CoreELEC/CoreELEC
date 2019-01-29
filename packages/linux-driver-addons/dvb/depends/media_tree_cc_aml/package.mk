# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="media_tree_cc_aml"
PKG_VERSION="2018-09-23"
PKG_SHA256="3b0cf3699317c04d9184b7e25056065fd374b20b851ee86a63ea2c70e219ee9e"
PKG_LICENSE="GPL"
PKG_SITE="https://bitbucket.org/CrazyCat/media_build/downloads/"
PKG_URL="$DISTRO_SRC/$PKG_NAME-$PKG_VERSION.tar.xz"
PKG_DEPENDS_TARGET="toolchain media_tree_aml"
PKG_NEED_UNPACK="$LINUX_DEPENDS media_tree_aml"
PKG_LONGDESC="Source of Linux Kernel media_tree subsystem to build with media_build."
PKG_TOOLCHAIN="manual"

unpack() {
  mkdir -p $PKG_BUILD/
  tar -xf $SOURCES/$PKG_NAME/$PKG_NAME-$PKG_VERSION.tar.xz -C $PKG_BUILD/

  # hack/workaround for borked upstream kernel/media_build
  # without removing atomisp there a lot additional includes that
  # slowdown build process after modpost from 3min to 6min
  # even if atomisp is disabled via kernel.conf
  rm -rf $PKG_BUILD/drivers/staging/media/atomisp
  sed -i 's|^.*drivers/staging/media/atomisp.*$||' \
    $PKG_BUILD/drivers/staging/media/Kconfig
}

configure() {
  rm -rf $PKG_BUILD/drivers/media/platform/meson/dvb
  cp -Lr $(get_build_dir media_tree_aml)/* $PKG_BUILD/
  echo 'source "drivers/media/platform/meson/dvb/Kconfig"' >>  "$PKG_BUILD/drivers/media/platform/Kconfig"
  echo 'source "drivers/media/platform/meson/video_dev/Kconfig"' >>  "$PKG_BUILD/drivers/media/platform/Kconfig"
}
