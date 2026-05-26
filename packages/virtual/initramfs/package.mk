# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)
# Copyright (C) 2020-present Team CoreELEC (https://coreelec.org)

PKG_NAME="initramfs"
PKG_VERSION=""
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://www.libreelec.tv"
PKG_URL=""
PKG_DEPENDS_INIT="libc:init busybox:init splash-image:init util-linux:init e2fsprogs:init dosfstools:init bkeymaps:init fakeroot:host"
PKG_DEPENDS_TARGET="toolchain ${PKG_NAME}:init"
PKG_SECTION="virtual"
PKG_LONGDESC="Metapackage for installing initramfs"

if [ "${INITRAMFS_PARTED_SUPPORT}" = yes ]; then
  PKG_DEPENDS_INIT+=" parted:init"
fi

for i in ${PKG_DEPENDS_INIT}; do
  PKG_NEED_UNPACK+=" $(get_pkg_directory ${i})"
done

post_install() {
  if [ "${BUILD_ANDROID_BOOTIMG}" = "yes" ]; then
  (
    cd ${BUILD}/${PKG_NAME}

    ln -sfn /usr/lib  ${BUILD}/${PKG_NAME}/lib
    ln -sfn /usr/bin  ${BUILD}/${PKG_NAME}/bin
    ln -sfn /usr/sbin ${BUILD}/${PKG_NAME}/sbin

    mkdir -p ${BUILD}/image
    fakeroot -- sh -c \
      "mkdir -p dev; mknod -m 600 dev/console c 5 1; find . | cpio -H newc -ov -R 0:0 > ${BUILD}/image/${PKG_NAME}.cpio"
  )
  fi
}
