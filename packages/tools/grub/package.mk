# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="grub"
PKG_VERSION="2.14"
PKG_SHA256="bc8d3c73535b8838d8c8e2654d73edc4e6ae8c8acdb45d5df5dc9a1547446d43"
PKG_ARCH="x86_64"
PKG_LICENSE="GPL-3.0-or-later"
PKG_SITE="https://www.gnu.org/software/grub/index.html"
PKG_URL="https://ftp.gnu.org/gnu/grub/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_HOST="toolchain:host"
PKG_DEPENDS_TARGET="toolchain flex freetype:host gettext:host grub:host"
PKG_LONGDESC="GRUB is a Multiboot boot loader."
PKG_TOOLCHAIN="configure"
PKG_BUILD_FLAGS="-cfg-libs -cfg-libs:host"

pre_configure_host() {
  unset CFLAGS
  unset CPPFLAGS
  unset CXXFLAGS
  unset LDFLAGS
  unset CPP
}

pre_configure_target() {
  PKG_CONFIGURE_OPTS_TARGET="--target=i386-pc-linux \
                             --disable-nls \
                             --with-platform=efi"

  unset CFLAGS
  unset CPPFLAGS
  unset CXXFLAGS
  unset LDFLAGS
  unset CPP

  # configure requires explicit TARGET_PREFIX binaries when cross compiling.
  export TARGET_CC="${TARGET_PREFIX}gcc"
  export TARGET_OBJCOPY="${TARGET_PREFIX}objcopy"
  export TARGET_STRIP="${TARGET_PREFIX}strip"
  export TARGET_NM="${TARGET_PREFIX}nm"
  export TARGET_RANLIB="${TARGET_PREFIX}ranlib"
}

make_target() {
  make CC=${CC} \
       AR=${AR} \
       RANLIB=${RANLIB} \
       CFLAGS="-I${SYSROOT_PREFIX}/usr/include -fomit-frame-pointer -D_FILE_OFFSET_BITS=64" \
       LDFLAGS="-L${SYSROOT_PREFIX}/usr/lib"
}

makeinstall_target() {
  ${PKG_BUILD}/.${HOST_NAME}/grub-mkimage -d grub-core -o bootia32.efi -O i386-efi -p /EFI/BOOT \
    boot chain configfile ext2 fat linux search efi_gop \
    efi_uga part_gpt gzio gettext loadenv loadbios memrw

  mkdir -p ${INSTALL}/usr/share/grub
     cp -P bootia32.efi ${INSTALL}/usr/share/grub

  mkdir -p ${TOOLCHAIN}/share/grub
     cp -P bootia32.efi ${TOOLCHAIN}/share/grub
}
