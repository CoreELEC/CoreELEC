# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2021-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="wsdd2"
PKG_VERSION="d7c9e1c5626010d406f36a05c7d51314deb7868c" # 1.8.7 + fixes
PKG_SHA256="c96feddef2d95f9e7211a1aacf4e80ffc6aa660e827c5283cbf0c678d7e33e76"
PKG_LICENSE="GPL-3.0-or-later"
PKG_SITE="https://github.com/oldium/wsdd2"
PKG_URL="https://github.com/oldium/wsdd2/archive/${PKG_VERSION}.tar.gz"
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
