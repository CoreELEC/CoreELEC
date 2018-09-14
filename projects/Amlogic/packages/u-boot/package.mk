# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="u-boot"
PKG_SITE="https://www.denx.de/wiki/U-Boot"
PKG_DEPENDS_TARGET="toolchain gcc-linaro-aarch64-elf:host gcc-linaro-arm-eabi:host"
PKG_ARCH="arm aarch64"
PKG_LICENSE="GPL"
PKG_SECTION="tools"
PKG_SHORTDESC="u-boot: Universal Bootloader project"
PKG_LONGDESC="Das U-Boot is a cross-platform bootloader for embedded systems, used as the default boot loader by several board vendors. It is intended to be easy to port and to debug, and runs on many supported architectures, including PPC, ARM, MIPS, x86, m68k, NIOS, and Microblaze."

case "$DEVICE" in
  "Odroid_C2")
    PKG_VERSION="95264d19d04930f67f10f162df70de447659329d"
    PKG_URL="https://github.com/hardkernel/u-boot/archive/$PKG_VERSION.tar.gz"
    PKG_SHA256="15fa9539af6c88d930ddda4c5b6e1661f16516030bd3b849370212e307529060"
    # Add the dependency of the hardkernel bl301 firmware
    PKG_DEPENDS_TARGET="$PKG_DEPENDS_TARGET u-boot_firmware"
    ;;
  "KVIM"*)
    PKG_VERSION="ffc14fcca35f499ba1489400dfe801901683ee60"
    PKG_URL="https://github.com/khadas/u-boot/archive/$PKG_VERSION.tar.gz"
    PKG_SHA256="1326126ca7962d314cb522d95e657dbf71966e74c84fb093181910f9e4f2c1fa"
    ;;
  "LePotato")
    PKG_VERSION="a43076c24e8d9500f471b671e07fa7d1bb72a678"
    PKG_URL="https://github.com/BayLibre/u-boot/archive/$PKG_VERSION.tar.gz"
    PKG_SHA256="0ae5fd97ba86fcd6cc7b2722580745a0ddbf651ffa0cc0bd188a05a9b668373f"
    ;;
  *)
    PKG_TOOLCHAIN="manual"
    ;;
esac

PKG_NEED_UNPACK="$PROJECT_DIR/$PROJECT/bootloader"
[ -n "$DEVICE" ] && PKG_NEED_UNPACK+=" $PROJECT_DIR/$PROJECT/devices/$DEVICE/bootloader"

pre_make_target() {
  case "$DEVICE" in
    "Odroid_C2")
        cp -r $(get_build_dir u-boot_firmware)/* $PKG_BUILD
      ;;
  esac

  sed -i "s|arm-none-eabi-|arm-eabi-|g" $PKG_BUILD/Makefile $PKG_BUILD/arch/arm/cpu/armv8/gx*/firmware/scp_task/Makefile 2>/dev/null || true
}

make_target() {
  if [ -n "$PKG_URL" ]; then
    [ "${BUILD_WITH_DEBUG}" = "yes" ] && PKG_DEBUG=1 || PKG_DEBUG=0
    export PATH=$TOOLCHAIN/lib/gcc-linaro-aarch64-elf/bin/:$TOOLCHAIN/lib/gcc-linaro-arm-eabi/bin/:$PATH
    DEBUG=${PKG_DEBUG} CROSS_COMPILE=aarch64-elf- ARCH=arm CFLAGS="" LDFLAGS="" make mrproper
    DEBUG=${PKG_DEBUG} CROSS_COMPILE=aarch64-elf- ARCH=arm CFLAGS="" LDFLAGS="" make $UBOOT_CONFIG
    DEBUG=${PKG_DEBUG} CROSS_COMPILE=aarch64-elf- ARCH=arm CFLAGS="" LDFLAGS="" make HOSTCC="$HOST_CC" HOSTSTRIP="true"
  fi
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/share/bootloader

    # Only install u-boot.img et al when building a board specific image
    find_file_path bootloader/install && . ${FOUND_PATH}

    # Always install the update script
    find_file_path bootloader/update.sh && cp -av ${FOUND_PATH} $INSTALL/usr/share/bootloader

    # Always install the canupdate script
    if find_file_path bootloader/canupdate.sh; then
      cp -av ${FOUND_PATH} $INSTALL/usr/share/bootloader
      sed -e "s/@PROJECT@/${DEVICE:-$PROJECT}/g" \
          -i $INSTALL/usr/share/bootloader/canupdate.sh
    fi

    find_file_path bootloader/boot.ini && cp -av ${FOUND_PATH} $INSTALL/usr/share/bootloader
    find_file_path bootloader/config.ini && cp -av ${FOUND_PATH} $INSTALL/usr/share/bootloader

    case "$DEVICE" in
      "Odroid_C2")
        cp -av $PKG_BUILD/u-boot.bin $INSTALL/usr/share/bootloader/u-boot
        find_file_path splash/boot-logo.bmp.gz && cp -av ${FOUND_PATH} $INSTALL/usr/share/bootloader
        ;;
      "KVIM"*|"LePotato")
        cp -av $PKG_BUILD/fip/u-boot.bin.sd.bin $INSTALL/usr/share/bootloader/u-boot
        ;;
    esac

    # Replace partition names in update.sh
    if [ -f "$INSTALL/usr/share/bootloader/update.sh" ] ; then
      sed -e "s/@BOOT_LABEL@/$DISTRO_BOOTLABEL/g" \
          -e "s/@DISK_LABEL@/$DISTRO_DISKLABEL/g" \
          -i $INSTALL/usr/share/bootloader/update.sh
    fi

    # Replace labels in boot.ini
    if [ -f "$INSTALL/usr/share/bootloader/boot.ini" ] ; then
      sed -e "s/@BOOT_LABEL@/$DISTRO_BOOTLABEL/g" \
          -e "s/@DISK_LABEL@/$DISTRO_DISKLABEL/g" \
          -i $INSTALL/usr/share/bootloader/boot.ini
    fi
}
