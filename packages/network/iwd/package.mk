# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="iwd"
PKG_VERSION="2.22"
PKG_SHA256="2c41c5da9924b90f8383b293b0c0b3d0bfb34fdc8822d8d0d37ec100707f263e"
PKG_LICENSE="GPL"
PKG_SITE="https://git.kernel.org/cgit/network/wireless/iwd.git/about/"
PKG_URL="https://www.kernel.org/pub/linux/network/wireless/iwd-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain readline dbus"
PKG_LONGDESC="Wireless daemon for Linux"
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_TARGET="--enable-client \
                           --enable-monitor \
                           --enable-systemd-service \
                           --enable-dbus-policy \
                           --disable-manual-pages"

pre_configure_target() {
  export LIBS="-lncurses"
}

post_makeinstall_target() {
  # ProtectSystem et al seems to break the service when systemd isn't built with seccomp.
  # investigate this more as it might be a systemd problem or kernel problem
  sed -e 's|^\(PrivateTmp=.*\)$|#\1|g' \
      -e 's|^\(NoNewPrivileges=.*\)$|#\1|g' \
      -e 's|^\(PrivateDevices=.*\)$|#\1|g' \
      -e 's|^\(ProtectHome=.*\)$|#\1|g' \
      -e 's|^\(ProtectSystem=.*\)$|#\1|g' \
      -e 's|^\(ReadWritePaths=.*\)$|#\1|g' \
      -e 's|^\(ProtectControlGroups=.*\)$|#\1|g' \
      -e 's|^\(ProtectKernelModules=.*\)$|#\1|g' \
      -e 's|^\(ConfigurationDirectory=.*\)$|#\1|g' \
      -i ${INSTALL}/usr/lib/systemd/system/iwd.service
}

post_install() {
  enable_service iwd.service
}

