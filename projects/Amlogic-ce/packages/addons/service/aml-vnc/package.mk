# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="aml-vnc"
PKG_VERSION="1.4.0_beta16"
PKG_SHA256="e8bac94733f7146f2ff9bbe6d5c7be68dab931f3379fa0249966b44bea83e3a9"
PKG_REV="0~beta-16"
PKG_ARCH="arm aarch64"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/dtechsrv/aml-vnc-server/"
PKG_URL="https://github.com/dtechsrv/aml-vnc-server/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libvncserver libdrm"
PKG_SECTION="service"
PKG_SHORTDESC="Amlogic VNC server"
PKG_LONGDESC="Amlogic VNC server is a Virtual Network Computing (VNC) server for Amlogic devices"

PKG_IS_ADDON="yes"
PKG_ADDON_NAME="Amlogic VNC Server"
PKG_ADDON_TYPE="xbmc.service"

makeinstall_target() {
  :
}

addon() {
  mkdir -p ${ADDON_BUILD}/${PKG_ADDON_ID}/bin
  cp -P ${PKG_BUILD}/aml-vnc ${ADDON_BUILD}/${PKG_ADDON_ID}/bin

  mkdir -p ${ADDON_BUILD}/${PKG_ADDON_ID}/lib
  cp $(get_build_dir libvncserver)/.${TARGET_NAME}/libvncserver.so.? ${ADDON_BUILD}/${PKG_ADDON_ID}/lib
}
