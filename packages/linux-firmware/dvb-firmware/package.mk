# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="dvb-firmware"
PKG_VERSION="90261ae2934329f6ca84dd6c72d10d0777bf4b0e"
PKG_SHA256="18a079bd123d9c01ba93c9ba0692b195f1bc66c367dd7f6f36e19cd8e18c643e"
PKG_LICENSE="LicenseRef-Free-to-use"
PKG_SITE="https://github.com/LibreELEC/dvb-firmware"
PKG_URL="https://github.com/LibreELEC/dvb-firmware/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="dvb-firmware: firmwares for various DVB drivers"
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  PKG_FW_DIR="${INSTALL}/$(get_kernel_overlay_dir)/lib/firmware"
  mkdir -p "${PKG_FW_DIR}"
    cp -a "${PKG_BUILD}/firmware/"* "${PKG_FW_DIR}"
}
