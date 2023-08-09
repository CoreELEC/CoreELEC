# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="aml-vnc"
PKG_VERSION="3d9fa1b080cafca5ea5704323d104741aaa76ad8"
PKG_SHA256="b502a5fa734c473cdb16e8e133d2a7f3a6f53c5bfd2e6fc541ce8585e077f233"
PKG_REV="1"
PKG_ARCH="arm aarch64"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/CoreELEC/aml-vnc"
PKG_URL="https://github.com/CoreELEC/aml-vnc/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain libvncserver"
PKG_SECTION="service"
PKG_SHORTDESC="Amlogic VNC server"
PKG_LONGDESC="Amlogic VNC server is a Virtual Network Computing (VNC) server for Amlogic devices"

PKG_IS_ADDON="yes"
PKG_ADDON_NAME="Amlogic VNC"
PKG_ADDON_TYPE="xbmc.service"

unpack() {
  mkdir -p ${PKG_BUILD}/addon
  tar --strip-components=3 -xf $SOURCES/${PKG_NAME}/${PKG_NAME}-${PKG_VERSION}.tar.gz \
    -C ${PKG_BUILD}       ${PKG_NAME}-${PKG_VERSION}/${PKG_NAME}/sources
  tar --strip-components=3 -xf $SOURCES/${PKG_NAME}/${PKG_NAME}-${PKG_VERSION}.tar.gz \
    -C ${PKG_BUILD}/addon ${PKG_NAME}-${PKG_VERSION}/${PKG_NAME}/source
}

pre_configure_target() {
  export CFLAGS+=" -Wno-stringop-truncation"
}

makeinstall_target() {
  :
}

addon() {
  mkdir -p ${ADDON_BUILD}/${PKG_ADDON_ID}/bin
    cp -P ${PKG_BUILD}/aml-vnc ${ADDON_BUILD}/${PKG_ADDON_ID}/bin

  mkdir -p ${ADDON_BUILD}/${PKG_ADDON_ID}/lib
    cp $(get_build_dir libvncserver)/.${TARGET_NAME}/libvncserver.so.? ${ADDON_BUILD}/${PKG_ADDON_ID}/lib

  cp -PR ${PKG_BUILD}/addon/* ${ADDON_BUILD}/${PKG_ADDON_ID}
}
