# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2021-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="foot"
PKG_VERSION="1.27.0"
PKG_SHA256="4e6131cc859ec6a36569f1978cf3617cc3836a681d13d228ded1b4885dab7770"
PKG_LICENSE="MIT"
PKG_SITE="https://codeberg.org/dnkl/foot/"
PKG_URL="https://codeberg.org/dnkl/foot/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain ncurses wayland wayland-protocols pixman fontconfig libxkbcommon fcft"
PKG_LONGDESC="A fast, lightweight and minimalistic Wayland terminal emulator"

if [ "${DISPLAYSERVER}" != "wl" ]; then
  PKG_BUILD_FLAGS="-sysroot"
  PKG_DEPENDS_CONFIG="wayland"
fi

PKG_MESON_OPTS_TARGET="-Ddocs=disabled \
                       -Dthemes=false \
                       -Dime=false \
                       -Dgrapheme-clustering=auto \
                       -Dterminfo=disabled \
                       -Ddefault-terminfo=xterm"

post_makeinstall_target() {
  # clean up
  safe_remove ${INSTALL}/usr/share/*

  # install scripts
  mkdir -p ${INSTALL}/usr/bin
    cp ${PKG_DIR}/scripts/foot.sh ${INSTALL}/usr/bin

  # install config
  mkdir -p ${INSTALL}/usr/share/foot
    cp ${PKG_DIR}/config/foot.ini ${INSTALL}/usr/share/foot
}
