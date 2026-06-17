# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2026-present Team CoreELEC (https://coreelec.org)
#
# MediaTek MT7612U (RT28xx/RT2870 STA lineage) USB Wi-Fi driver.
# Source: caruofc/MT7612U-Driver (4.9/ARM64-patched MediaTek RT28xx STA fork).
#
# Build-tested: `PROJECT=Amlogic-ce DEVICE=Amlogic-ng ARCH=arm ./scripts/build
# mt7612u` produces os/linux/mt7612u.ko (ELF aarch64, vermagic
# "4.9.269 SMP preempt mod_unload modversions aarch64", alias usb:v0E8Dp7612)
# and installs it to lib/modules/<kver>/mt7612u/.

PKG_NAME="mt7612u"
PKG_VERSION="749e6fd1a559e454b17ae91959fe07d6ef58e6af"
PKG_SHA256="a4e3a6652e48bb521ec6b1141ad5d52aeff6d0c3b0097d3ef48808f967203bf1"
PKG_ARCH="arm aarch64"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/caruofc/MT7612U-Driver"
PKG_URL="${PKG_SITE}/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain linux"
PKG_NEED_UNPACK="${LINUX_DEPENDS}"
PKG_LONGDESC="MediaTek MT7612U USB Wi-Fi STA driver (caruofc fork)"
PKG_IS_KERNEL_PKG="yes"
PKG_TOOLCHAIN="manual"

pre_make_target() {
  unset LDFLAGS
}

make_target() {
  # caruofc is an RT28xx two-stage build: its OWN top Makefile generates
  # mt76x2_version.h, copies os/linux/Makefile.6, and sets the -I include paths,
  # THEN recurses into the kernel build via LINUX_SRC. Do NOT use kernel_make
  # (make -C <kernel> M=<pkg>) — that bypasses the prep and the compile fails
  # with "rt_config.h / mt76x2_version.h: No such file or directory".
  make -C ${PKG_BUILD} \
    WIFI_MODE=STA \
    ARCH=${TARGET_KERNEL_ARCH} \
    CROSS_COMPILE=${TARGET_KERNEL_PREFIX} \
    LINUX_SRC=$(kernel_path)
}

makeinstall_target() {
  mkdir -p ${INSTALL}/$(get_full_module_dir)/${PKG_NAME}
  cp ${PKG_BUILD}/os/linux/mt7612u.ko ${INSTALL}/$(get_full_module_dir)/${PKG_NAME}

  # FIRMWARE: held back from PR #1 pending maintainer policy (see PR body).
  # If maintainers approve shipping the blobs in the driver package, add:
  #   mkdir -p ${INSTALL}/$(get_full_firmware_dir)
  #   cp ${PKG_BUILD}/mcu/bin/mt7662_patch_e3_hdr.bin      ${INSTALL}/$(get_full_firmware_dir)
  #   cp ${PKG_BUILD}/mcu/bin/mt7662_firmware_e3_tvbox.bin ${INSTALL}/$(get_full_firmware_dir)
}
