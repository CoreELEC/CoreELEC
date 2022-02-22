# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2022-present Team CoreELEC (https://coreelec.org)

PKG_NAME="w1-aml"
PKG_VERSION="c0844eed41989bbb10451e58248a175306475e7e"
PKG_SHA256="735e5ad43c766b046961f1e8b70a4530111bef47a5fcb3ca15394744746708fa"
PKG_ARCH="arm aarch64"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/CoreELEC/w1-aml"
PKG_URL="https://github.com/CoreELEC/w1-aml/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain linux"
PKG_NEED_UNPACK="${LINUX_DEPENDS}"
PKG_LONGDESC="Amlogic W150S1 Linux driver"
PKG_IS_KERNEL_PKG="yes"
PKG_TOOLCHAIN="manual"

make_target() {
  kernel_make -C $(kernel_path) M=${PKG_BUILD}/project_w1/vmac \
    subdir-ccflags-y="-mno-outline-atomics"
}

makeinstall_target() {
  mkdir -p ${INSTALL}/$(get_full_module_dir)/${PKG_NAME}
    find ${PKG_BUILD}/project_w1/vmac/ -name \*.ko -not -path '*/\.*' -exec cp {} ${INSTALL}/$(get_full_module_dir)/${PKG_NAME} \;

  mkdir -p ${INSTALL}/$(get_full_firmware_dir)/w1
    cp ${PKG_BUILD}/project_w1/vmac/aml_wifi*.txt ${INSTALL}/$(get_full_firmware_dir)/w1
}
