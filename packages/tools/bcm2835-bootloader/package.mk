# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="bcm2835-bootloader"
PKG_VERSION="9c78bf4"
PKG_SHA256="b6ea09f5bf45a36861ebeae83ad74f384603c4d0be20862e8b639f568956ee7d"
PKG_ARCH="arm"
PKG_LICENSE="nonfree"
PKG_SITE="http://www.broadcom.com"
PKG_URL="$DISTRO_SRC/$PKG_NAME-$PKG_VERSION.tar.xz"
PKG_DEPENDS_TARGET="toolchain linux"
PKG_SECTION="tools"
PKG_SHORTDESC="bcm2835-bootloader: Tool to create a bootable kernel for RaspberryPi"
PKG_LONGDESC="bcm2835-bootloader: Tool to create a bootable kernel for RaspberryPi"
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  mkdir -p $INSTALL/usr/share/bootloader
    cp -PRv LICENCE* $INSTALL/usr/share/bootloader
    cp -PRv bootcode.bin $INSTALL/usr/share/bootloader
    cp -PRv fixup_x.dat $INSTALL/usr/share/bootloader/fixup.dat
    cp -PRv start_x.elf $INSTALL/usr/share/bootloader/start.elf

    find_file_path config/dt-blob.bin && cp -PRv $FOUND_PATH $INSTALL/usr/share/bootloader

    cp -PRv $PKG_DIR/scripts/update.sh $INSTALL/usr/share/bootloader

    find_file_path config/distroconfig.txt $PKG_DIR/files/3rdparty/bootloader/distroconfig.txt && cp -PRv ${FOUND_PATH} $INSTALL/usr/share/bootloader
    find_file_path config/config.txt $PKG_DIR/files/3rdparty/bootloader/config.txt && cp -PRv ${FOUND_PATH} $INSTALL/usr/share/bootloader
}
