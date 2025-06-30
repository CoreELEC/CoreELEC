# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2022-present Team CoreELEC (https://coreelec.org)

PKG_NAME="CoreELEC-settings"
PKG_VERSION="a24cd5ad7068e101f8328f0514e2807427d6d6d5"
PKG_SHA256="121c9f8bd24b2dcf258ade8caed63b32c2dfd8eaa9e42288cc5d01badaf122c0"
PKG_LICENSE="GPL"
PKG_SITE="https://coreelec.org"
PKG_URL="https://github.com/CoreELEC/service.coreelec.settings/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain Python3 connman pygobject dbus-python"
PKG_LONGDESC="CoreELEC-settings: is a settings dialog for CoreELEC"

PKG_MAKE_OPTS_TARGET="DISTRONAME=${DISTRONAME} ADDON_VERSION=${ADDON_VERSION} ROOT_PASSWORD=${ROOT_PASSWORD}"

if [ "${DISPLAYSERVER}" = "x11" ]; then
  PKG_DEPENDS_TARGET="${PKG_DEPENDS_TARGET} setxkbmap"
else
  PKG_DEPENDS_TARGET="${PKG_DEPENDS_TARGET} bkeymaps"
fi

post_makeinstall_target() {
  mkdir -p ${INSTALL}/usr/lib/coreelec
    cp ${PKG_DIR}/scripts/* ${INSTALL}/usr/lib/coreelec

  ADDON_INSTALL_DIR=${INSTALL}/usr/share/kodi/addons/service.coreelec.settings

  ${TOOLCHAIN}/bin/python -Wi -t -B ${TOOLCHAIN}/lib/${PKG_PYTHON_VERSION}/compileall.py ${ADDON_INSTALL_DIR}/resources/lib/ -f
  rm -rf $(find ${ADDON_INSTALL_DIR}/resources/lib/ -name "*.py")

  ${TOOLCHAIN}/bin/python -Wi -t -B ${TOOLCHAIN}/lib/${PKG_PYTHON_VERSION}/compileall.py ${ADDON_INSTALL_DIR}/oe.py -f
  rm -rf ${ADDON_INSTALL_DIR}/oe.py
}

post_install() {
  enable_service backup-restore.service
  enable_service factory-reset.service
}
