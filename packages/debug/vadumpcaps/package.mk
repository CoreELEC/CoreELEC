# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2020-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="vadumpcaps"
PKG_VERSION="792c27f95ef205a14f682f2261a634b48d65560d"
PKG_SHA256="4a5a6807f0741e0e283372f394b7cced27df3dac2af78dd8f979b6a87607f37b"
PKG_LICENSE="GPL-3.0-or-later"
PKG_SITE="https://github.com/fhvwy/vadumpcaps"
PKG_URL="https://github.com/fhvwy/vadumpcaps/archive/${PKG_VERSION}.tar.gz"
PKG_LONGDESC="This is a utility to show all capabilities of a VAAPI device/driver."
PKG_DEPENDS_TARGET="toolchain libva"

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/bin
  cp vadumpcaps ${INSTALL}/usr/bin
}
