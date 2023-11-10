# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="xf86-video-ati"
PKG_VERSION="22.0.0"
PKG_SHA256="c8c8bb56d3f6227c97e59c3a3c85a25133584ceb82ab5bc05a902a743ab7bf6d"
PKG_ARCH="x86_64"
PKG_LICENSE="OSS"
PKG_SITE="https://www.x.org/wiki/RadeonFeature/"
PKG_URL="http://xorg.freedesktop.org/archive/individual/driver/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain xorg-server"
PKG_LONGDESC="ATI/AMD Radeon video driver for the Xorg X server."
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_TARGET="--enable-glamor \
                           --with-xorg-module-dir=${XORG_PATH_MODULES}"

post_configure_target() {
  libtool_remove_rpath libtool
}

post_makeinstall_target() {
  mkdir -p ${INSTALL}/etc/X11
    cp ${PKG_DIR}/config/*.conf ${INSTALL}/etc/X11
}
