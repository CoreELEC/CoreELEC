# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="weston"
PKG_VERSION="15.0.1"
PKG_SHA256="551d039bfb0c837ba5a4d027cdb8ee16bded0eedb789821f8025d8a64b791f6d"
PKG_LICENSE="MIT"
PKG_SITE="https://wayland.freedesktop.org/"
PKG_URL="https://gitlab.freedesktop.org/wayland/weston/-/releases/${PKG_VERSION}/downloads/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain cairo dbus libdrm libinput libjpeg-turbo libxcb libxkbcommon pango seatd wayland wayland-protocols"
PKG_LONGDESC="Reference implementation of a Wayland compositor"

PKG_BUILD_FLAGS="-ndebug"

PKG_MESON_OPTS_TARGET="-Dbackend-drm=true \
                       -Dbackend-headless=false \
                       -Dbackend-pipewire=false \
                       -Dbackend-rdp=false \
                       -Dbackend-vnc=false \
                       -Dbackend-wayland=false \
                       -Dbackend-x11=false \
                       -Dbackend-default=drm \
                       -Drenderer-gl=true \
                       -Dxwayland=false \
                       -Dsystemd=true \
                       -Dremoting=false \
                       -Dpipewire=false \
                       -Dshell-desktop=true \
                       -Dshell-lua=false \
                       -Dshell-ivi=false \
                       -Dshell-kiosk=false \
                       -Ddesktop-shell-client-default="weston-desktop-shell" \
                       -Dcolor-management-lcms=false \
                       -Dimage-jpeg=true \
                       -Dimage-webp=false \
                       -Dtools=['terminal']
                       -Ddemo-clients=false \
                       -Dsimple-clients=[] \
                       -Dresize-pool=false \
                       -Dwcap-decode=false \
                       -Dtest-junit-xml=false \
                       -Dtest-skip-is-failure=false \
                       -Ddoc=false"

post_makeinstall_target() {
  mkdir -p ${INSTALL}/usr/lib/weston
    cp ${PKG_DIR}/scripts/weston-config ${INSTALL}/usr/lib/weston

  mkdir -p ${INSTALL}/usr/share/weston
    cp ${PKG_DIR}/config/weston.ini ${INSTALL}/usr/share/weston
    find_file_path "splash/splash-2160.png" && cp ${FOUND_PATH} ${INSTALL}/usr/share/weston/libreelec-wallpaper-2160.png

  safe_remove ${INSTALL}/usr/share/wayland-sessions
}

post_install() {
  enable_service weston.service
}
