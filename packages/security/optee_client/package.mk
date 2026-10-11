# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="optee_client"
PKG_VERSION="4.10.0"
PKG_SHA256="984084a465f55ed8037e0e27eb4399149b5cec981ed295c2f0ee0dd515f60af9"
PKG_LICENSE="BSD-2-Clause"
PKG_SITE="https://github.com/OP-TEE/optee_client"
PKG_URL="https://github.com/OP-TEE/optee_client/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="OP-TEE normal world client library (libteec) and tee-supplicant daemon"

PKG_CMAKE_OPTS_TARGET="-DCFG_WERROR=OFF \
                       -DWITH_TEEACL=OFF \
                       -DCFG_TEE_SUPP_PLUGINS=OFF \
                       -DCFG_TA_GPROF_SUPPORT=OFF \
                       -DCFG_FTRACE_SUPPORT=OFF \
                       -DCFG_ENABLE_SYSTEMD=OFF \
                       -DCFG_ENABLE_UDEV=OFF \
                       -DRPMB_EMU=ON \
                       -DCFG_TEE_CLIENT_LOAD_PATH=/usr/lib:/lib/firmware \
                       -DCFG_TEE_FS_PARENT_PATH=/storage/.cache/tee"

post_install() {
  enable_service tee-supplicant.service
}
