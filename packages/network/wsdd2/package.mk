# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2021-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="wsdd2"
PKG_VERSION="1.9.2"
PKG_SHA256="4d1bec3c68fea57c072af49a7649781ece7280f809b18fc1576bb654b263b181"
PKG_LICENSE="GPL-3.0-or-later"
PKG_SITE="https://github.com/pesa1234/wsdd2"
PKG_URL="https://github.com/pesa1234/wsdd2/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="make:host gcc:host"
PKG_LONGDESC="WSD/LLMNR Discovery/Name Service Daemon"
PKG_BUILD_FLAGS="+size"

post_makeinstall_target() {
  # our own unit is installed from system.d - upstream's has no ordering,
  # no guard on the generated smb.conf and is wanted by multi-user.target
  safe_remove ${INSTALL}/usr/lib/systemd/system/wsdd2.service
}

post_install() {
  sed -e "/^ExecStart=/s|@MODEL@|${DEVICE:-${PROJECT}}|" \
      -i ${INSTALL}/usr/lib/systemd/system/wsdd2.service

  enable_service wsdd2.service
}
