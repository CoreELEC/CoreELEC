# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="efibootmgr"
PKG_VERSION="0a85e9baaac8a34e4a0bb9c23dfcc9c4f759e061"
PKG_SHA256="05c621b1c08f3fdade8ddd4403240eb528705cee9e65d1bce937b0dc43c4fee9"
PKG_ARCH="x86_64"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/rhboot/efibootmgr"
PKG_URL="https://github.com/rhboot/efibootmgr/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain efivar pciutils zlib"
PKG_LONGDESC="Tool to modify UEFI Firmware Boot Manager Variables."
PKG_BUILD_FLAGS="-sysroot"

make_target() {
  export CFLAGS="${CFLAGS} -I${SYSROOT_PREFIX}/usr/include -I${SYSROOT_PREFIX}/usr/include/efivar -fgnu89-inline -Wno-pointer-sign"
  export LDFLAGS="${LDFLAGS} -L${SYSROOT_PREFIX}/usr/lib -ludev -ldl"

  make EFIDIR=BOOT EFI_LOADER=bootx64.efi PKG_CONFIG=true \
    LDLIBS="-lefiboot -lefivar" \
    efibootmgr
}

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/bin
  cp -p src/efibootmgr ${INSTALL}/usr/bin
}
