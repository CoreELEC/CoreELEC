# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2017-2018 Team LibreELEC (https://libreelec.tv)
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="linux"
PKG_LICENSE="GPL"
PKG_SITE="http://www.kernel.org"
PKG_DEPENDS_HOST="ccache:host openssl:host"
PKG_DEPENDS_TARGET="toolchain linux:host cpio:host kmod:host xz:host wireless-regdb keyutils $KERNEL_EXTRA_DEPENDS_TARGET"
PKG_DEPENDS_INIT="toolchain"
PKG_NEED_UNPACK="$LINUX_DEPENDS $(get_pkg_directory busybox) $PROJECT_DIR/$PROJECT/initramfs"
PKG_LONGDESC="This package contains a precompiled kernel image and the modules."
PKG_IS_KERNEL_PKG="yes"
PKG_STAMP="$KERNEL_TARGET $KERNEL_MAKE_EXTRACMD $KERNEL_UBOOT_EXTRA_TARGET"

PKG_PATCH_DIRS="$LINUX"

case "$LINUX" in
  amlogic-3.14)
    PKG_VERSION="07d26b4ce91cf934d65a64e2da7ab3bc75e59fcc"
    PKG_SHA256="682f93c0bb8ad888a681e93882bc169007bacb880714b980af00ca34fb5b8365"
    PKG_URL="https://github.com/CoreELEC/linux-amlogic/archive/$PKG_VERSION.tar.gz"
    PKG_SOURCE_NAME="linux-$LINUX-$PKG_VERSION.tar.gz"
    PKG_DEPENDS_TARGET="$PKG_DEPENDS_TARGET aml-dtbtools:host"
    PKG_BUILD_PERF="no"
    ;;
  amlogic-4.9)
    PKG_VERSION="efed18a6441bc3c1df51c22e7832cfcbf7481d8a"
    PKG_SHA256="6282a318c25c2cfebb0e7813e9ccee040c2827ae34f659d3998ab01d825a23c6"
    PKG_URL="https://github.com/CoreELEC/linux-amlogic/archive/$PKG_VERSION.tar.gz"
    PKG_SOURCE_NAME="linux-$LINUX-$PKG_VERSION.tar.gz"
    PKG_DEPENDS_TARGET="$PKG_DEPENDS_TARGET aml-dtbtools:host"
    PKG_DEPENDS_UNPACK="media_modules-aml"
    PKG_NEED_UNPACK="$PKG_NEED_UNPACK $(get_pkg_directory media_modules-aml)"
    PKG_BUILD_PERF="no"
    PKG_GIT_BRANCH="amlogic-4.9"
    ;;
  rockchip-4.4)
    PKG_VERSION="aa8bacf821e5c8ae6dd8cae8d64011c741659945"
    PKG_SHA256="a2760fe89a15aa7be142fd25fb08ebd357c5d855c41f1612cf47c6e89de39bb3"
    PKG_URL="https://github.com/rockchip-linux/kernel/archive/$PKG_VERSION.tar.gz"
    PKG_SOURCE_NAME="linux-$LINUX-$PKG_VERSION.tar.gz"
    ;;
  raspberrypi)
    PKG_VERSION="abaa3760da89d6fb38e55473fffc9a31dd0b1d7a" # 4.19.127
    PKG_SHA256="b7345333ee90949dabc8e7fa184c443dc43781bdd3703e2203ad084274b50f24"
    PKG_URL="https://github.com/raspberrypi/linux/archive/$PKG_VERSION.tar.gz"
    PKG_SOURCE_NAME="linux-$LINUX-$PKG_VERSION.tar.gz"
    ;;
  *)
    PKG_VERSION="5.1.16"
    PKG_SHA256="8a3e55be3e788700836db6f75875b4d3b824a581d1eacfc2fcd29ed4e727ba3e"
    PKG_URL="https://www.kernel.org/pub/linux/kernel/v5.x/$PKG_NAME-$PKG_VERSION.tar.xz"
    PKG_PATCH_DIRS="default"
    ;;
esac

PKG_KERNEL_CFG_FILE=$(kernel_config_path) || die

if [ -n "$KERNEL_TOOLCHAIN" ]; then
  PKG_DEPENDS_HOST="$PKG_DEPENDS_HOST gcc-arm-$KERNEL_TOOLCHAIN:host"
  PKG_DEPENDS_TARGET="$PKG_DEPENDS_TARGET gcc-arm-$KERNEL_TOOLCHAIN:host"
  HEADERS_ARCH=$TARGET_ARCH
fi

if [ "$PKG_BUILD_PERF" != "no" ] && grep -q ^CONFIG_PERF_EVENTS= $PKG_KERNEL_CFG_FILE ; then
  PKG_BUILD_PERF="yes"
  PKG_DEPENDS_TARGET="$PKG_DEPENDS_TARGET binutils elfutils libunwind zlib openssl"
fi

if [ "$TARGET_ARCH" = "x86_64" ]; then
  PKG_DEPENDS_TARGET="$PKG_DEPENDS_TARGET intel-ucode:host kernel-firmware elfutils:host pciutils"
fi

if [[ "$KERNEL_TARGET" = uImage* ]]; then
  PKG_DEPENDS_TARGET="$PKG_DEPENDS_TARGET u-boot-tools:host"
fi

if [ "$BUILD_ANDROID_BOOTIMG" = "yes" ]; then
  PKG_DEPENDS_TARGET="$PKG_DEPENDS_TARGET mkbootimg:host"
fi

post_patch() {
  cp $PKG_KERNEL_CFG_FILE $PKG_BUILD/.config

  # set default hostname based on $DISTRONAME
    sed -i -e "s|@DISTRONAME@|$DISTRONAME|g" $PKG_BUILD/.config

  # disable swap support if not enabled
  if [ ! "$SWAP_SUPPORT" = yes ]; then
    sed -i -e "s|^CONFIG_SWAP=.*$|# CONFIG_SWAP is not set|" $PKG_BUILD/.config
  fi

  # disable nfs support if not enabled
  if [ ! "$NFS_SUPPORT" = yes ]; then
    sed -i -e "s|^CONFIG_NFS_FS=.*$|# CONFIG_NFS_FS is not set|" $PKG_BUILD/.config
  fi

  # disable cifs support if not enabled
  if [ ! "$SAMBA_SUPPORT" = yes ]; then
    sed -i -e "s|^CONFIG_CIFS=.*$|# CONFIG_CIFS is not set|" $PKG_BUILD/.config
  fi

  # disable iscsi support if not enabled
  if [ ! "$ISCSI_SUPPORT" = yes ]; then
    sed -i -e "s|^CONFIG_SCSI_ISCSI_ATTRS=.*$|# CONFIG_SCSI_ISCSI_ATTRS is not set|" $PKG_BUILD/.config
    sed -i -e "s|^CONFIG_ISCSI_TCP=.*$|# CONFIG_ISCSI_TCP is not set|" $PKG_BUILD/.config
    sed -i -e "s|^CONFIG_ISCSI_BOOT_SYSFS=.*$|# CONFIG_ISCSI_BOOT_SYSFS is not set|" $PKG_BUILD/.config
    sed -i -e "s|^CONFIG_ISCSI_IBFT_FIND=.*$|# CONFIG_ISCSI_IBFT_FIND is not set|" $PKG_BUILD/.config
    sed -i -e "s|^CONFIG_ISCSI_IBFT=.*$|# CONFIG_ISCSI_IBFT is not set|" $PKG_BUILD/.config
  fi

  # install extra dts files
  for f in $PROJECT_DIR/$PROJECT/config/*-overlay.dts; do
    [ -f "$f" ] && cp -v $f $PKG_BUILD/arch/$TARGET_KERNEL_ARCH/boot/dts/overlays || true
  done
  if [ -n "$DEVICE" ]; then
    for f in $PROJECT_DIR/$PROJECT/devices/$DEVICE/config/*-overlay.dts; do
      [ -f "$f" ] && cp -v $f $PKG_BUILD/arch/$TARGET_KERNEL_ARCH/boot/dts/overlays || true
    done
  fi
}

make_host() {
  make \
    ARCH=${HEADERS_ARCH:-$TARGET_KERNEL_ARCH} \
    HOSTCC="$TOOLCHAIN/bin/host-gcc" \
    HOSTCXX="$TOOLCHAIN/bin/host-g++" \
    HOSTCFLAGS="$HOST_CFLAGS" \
    HOSTCXXFLAGS="$HOST_CXXFLAGS" \
    HOSTLDFLAGS="$HOST_LDFLAGS" \
    headers_check
}

makeinstall_host() {
  make \
    ARCH=${HEADERS_ARCH:-$TARGET_KERNEL_ARCH} \
    HOSTCC="$TOOLCHAIN/bin/host-gcc" \
    HOSTCXX="$TOOLCHAIN/bin/host-g++" \
    HOSTCFLAGS="$HOST_CFLAGS" \
    HOSTCXXFLAGS="$HOST_CXXFLAGS" \
    HOSTLDFLAGS="$HOST_LDFLAGS" \
    INSTALL_HDR_PATH=dest \
    headers_install
  mkdir -p $SYSROOT_PREFIX/usr/include
    cp -R dest/include/* $SYSROOT_PREFIX/usr/include
}

pre_make_target() {
  if [ "$TARGET_ARCH" = "x86_64" ]; then
    # copy some extra firmware to linux tree
    mkdir -p $PKG_BUILD/external-firmware
      cp -a $(get_build_dir kernel-firmware)/{amdgpu,amd-ucode,i915,radeon,e100,rtl_nic} $PKG_BUILD/external-firmware

    cp -a $(get_build_dir intel-ucode)/intel-ucode $PKG_BUILD/external-firmware

    FW_LIST="$(find $PKG_BUILD/external-firmware \( -type f -o -type l \) \( -iname '*.bin' -o -iname '*.fw' -o -path '*/intel-ucode/*' \) | sed 's|.*external-firmware/||' | sort | xargs)"
    sed -i "s|CONFIG_EXTRA_FIRMWARE=.*|CONFIG_EXTRA_FIRMWARE=\"${FW_LIST}\"|" $PKG_BUILD/.config
  fi

  kernel_make oldconfig

  # regdb (backward compatability with pre-4.15 kernels)
  if grep -q ^CONFIG_CFG80211_INTERNAL_REGDB= $PKG_BUILD/.config ; then
    cp $(get_build_dir wireless-regdb)/db.txt $PKG_BUILD/net/wireless/db.txt
  fi

  # copy video firmware (kernel won't compile without it)
  [ "$LINUX" = "amlogic-4.9" ] && cp -PR $(get_build_dir media_modules-aml)/firmware $PKG_BUILD/firmware/video || :
}

make_target() {
  kernel_make modules
  kernel_make INSTALL_MOD_PATH=$INSTALL/$(get_kernel_overlay_dir) modules_install
  rm -f $INSTALL/$(get_kernel_overlay_dir)/lib/modules/*/build
  rm -f $INSTALL/$(get_kernel_overlay_dir)/lib/modules/*/source

  if [ "$PKG_BUILD_PERF" = "yes" ] ; then
    ( cd tools/perf

      # arch specific perf build args
      case "$TARGET_ARCH" in
        x86_64)
          PERF_BUILD_ARGS="ARCH=x86"
          ;;
        aarch64)
          PERF_BUILD_ARGS="ARCH=arm64"
          ;;
        *)
          PERF_BUILD_ARGS="ARCH=$TARGET_ARCH"
          ;;
      esac

      WERROR=0 \
      NO_LIBPERL=1 \
      NO_LIBPYTHON=1 \
      NO_SLANG=1 \
      NO_GTK2=1 \
      NO_LIBNUMA=1 \
      NO_LIBAUDIT=1 \
      NO_LZMA=1 \
      NO_SDT=1 \
      CROSS_COMPILE="$TARGET_PREFIX" \
      JOBS="$CONCURRENCY_MAKE_LEVEL" \
        make $PERF_BUILD_ARGS
      mkdir -p $INSTALL/usr/bin
        cp perf $INSTALL/usr/bin
    )
  fi

  ( cd $ROOT
    rm -rf $BUILD/initramfs
    $SCRIPTS/install initramfs
  )
  pkg_lock_status "ACTIVE" "linux:target" "build"

  if [ "$BOOTLOADER" = "u-boot" -a -n "$KERNEL_UBOOT_EXTRA_TARGET" ]; then
    for extra_target in "$KERNEL_UBOOT_EXTRA_TARGET"; do
      kernel_make $extra_target
    done
  fi

  # arm64 target does not support creating uImage.
  # Build Image first, then wrap it using u-boot's mkimage.
  if [[ "$TARGET_KERNEL_ARCH" == "arm64" && "$KERNEL_TARGET" == uImage* ]]; then
    if [ -z "$KERNEL_UIMAGE_LOADADDR" -o -z "$KERNEL_UIMAGE_ENTRYADDR" ]; then
      die "ERROR: KERNEL_UIMAGE_LOADADDR and KERNEL_UIMAGE_ENTRYADDR have to be set to build uImage - aborting"
    fi
    KERNEL_UIMAGE_TARGET="$KERNEL_TARGET"
    KERNEL_TARGET="${KERNEL_TARGET/uImage/Image}"
  fi

  # the modules target is required to get a proper Module.symvers
  # file with symbols from built-in and external modules.
  # Without that it'll contain only the symbols from the kernel
  kernel_make $KERNEL_TARGET $KERNEL_MAKE_EXTRACMD modules

  for ce_dtb in arch/$TARGET_KERNEL_ARCH/boot/dts/amlogic/coreelec-*; do
    if [ -d $ce_dtb ]; then
      cp $ce_dtb/*.dtb arch/$TARGET_KERNEL_ARCH/boot/dts/amlogic 2>/dev/null
    fi
  done

  if [ "$BUILD_ANDROID_BOOTIMG" = "yes" ]; then
    find_file_path bootloader/mkbootimg && source ${FOUND_PATH}
    mv -f arch/$TARGET_KERNEL_ARCH/boot/boot.img arch/$TARGET_KERNEL_ARCH/boot/$KERNEL_TARGET
  fi

  if [ -n "$KERNEL_UIMAGE_TARGET" ] ; then
    # determine compression used for kernel image
    KERNEL_UIMAGE_COMP=${KERNEL_UIMAGE_TARGET:7}
    KERNEL_UIMAGE_COMP=${KERNEL_UIMAGE_COMP:-none}

    # calculate new load address to make kernel Image unpack to memory area after compressed image
    if [ "$KERNEL_UIMAGE_COMP" != "none" ] ; then
      COMPRESSED_SIZE=$(stat -t "arch/$TARGET_KERNEL_ARCH/boot/$KERNEL_TARGET" | awk '{print $2}')
      # align to 1 MiB
      COMPRESSED_SIZE=$(( (($COMPRESSED_SIZE - 1 >> 20) + 1) << 20 ))
      PKG_KERNEL_UIMAGE_LOADADDR=$(printf '%X' "$(( $KERNEL_UIMAGE_LOADADDR + $COMPRESSED_SIZE ))")
      PKG_KERNEL_UIMAGE_ENTRYADDR=$(printf '%X' "$(( $KERNEL_UIMAGE_ENTRYADDR + $COMPRESSED_SIZE ))")
    else
      PKG_KERNEL_UIMAGE_LOADADDR=${KERNEL_UIMAGE_LOADADDR}
      PKG_KERNEL_UIMAGE_ENTRYADDR=${KERNEL_UIMAGE_ENTRYADDR}
    fi

    mkimage -A $TARGET_KERNEL_ARCH \
            -O linux \
            -T kernel \
            -C $KERNEL_UIMAGE_COMP \
            -a $PKG_KERNEL_UIMAGE_LOADADDR \
            -e $PKG_KERNEL_UIMAGE_ENTRYADDR \
            -d arch/$TARGET_KERNEL_ARCH/boot/$KERNEL_TARGET \
               arch/$TARGET_KERNEL_ARCH/boot/$KERNEL_UIMAGE_TARGET

    KERNEL_TARGET="${KERNEL_UIMAGE_TARGET}"
  fi
}

makeinstall_target() {
  if [ "$BOOTLOADER" = "u-boot" ]; then
    mkdir -p $INSTALL/usr/share/bootloader/device_trees
    if [ -d arch/$TARGET_KERNEL_ARCH/boot/dts/amlogic ]; then
      cp arch/$TARGET_KERNEL_ARCH/boot/*dtb.img $INSTALL/usr/share/bootloader/ 2>/dev/null || :
      [ "$PROJECT" = "Amlogic-ng" ] && cp arch/$TARGET_KERNEL_ARCH/boot/dts/amlogic/*.dtb $INSTALL/usr/share/bootloader/device_trees 2>/dev/null || :
    fi
  elif [ "$BOOTLOADER" = "bcm2835-bootloader" ]; then
    mkdir -p $INSTALL/usr/share/bootloader/overlays

    # install platform dtbs, but remove upstream kernel dtbs (i.e. without downstream
    # drivers and decent USB support) as these are not required by LibreELEC
    cp -p arch/$TARGET_KERNEL_ARCH/boot/dts/*.dtb $INSTALL/usr/share/bootloader
    rm -f $INSTALL/usr/share/bootloader/bcm283*.dtb

    # install overlay dtbs
    for dtb in arch/$TARGET_KERNEL_ARCH/boot/dts/overlays/*.dtbo; do
      cp $dtb $INSTALL/usr/share/bootloader/overlays 2>/dev/null || :
    done
    cp -p arch/$TARGET_KERNEL_ARCH/boot/dts/overlays/README $INSTALL/usr/share/bootloader/overlays
  fi
}

make_init() {
 : # reuse make_target()
}

makeinstall_init() {
  if [ -n "$INITRAMFS_MODULES" ]; then
    mkdir -p $INSTALL/etc
    mkdir -p $INSTALL/usr/lib/modules

    for i in $INITRAMFS_MODULES; do
      module=`find .install_pkg/$(get_full_module_dir)/kernel -name $i.ko`
      if [ -n "$module" ]; then
        echo $i >> $INSTALL/etc/modules
        cp $module $INSTALL/usr/lib/modules/`basename $module`
      fi
    done
  fi

  if [ "$UVESAFB_SUPPORT" = yes ]; then
    mkdir -p $INSTALL/usr/lib/modules
      uvesafb=`find .install_pkg/$(get_full_module_dir)/kernel -name uvesafb.ko`
      cp $uvesafb $INSTALL/usr/lib/modules/`basename $uvesafb`
  fi
}

post_install() {
  mkdir -p $INSTALL/$(get_full_firmware_dir)/

  # regdb and signature is now loaded as firmware by 4.15+
    if grep -q ^CONFIG_CFG80211_REQUIRE_SIGNED_REGDB= $PKG_BUILD/.config; then
      cp $(get_build_dir wireless-regdb)/regulatory.db{,.p7s} $INSTALL/$(get_full_firmware_dir)
    fi
}
