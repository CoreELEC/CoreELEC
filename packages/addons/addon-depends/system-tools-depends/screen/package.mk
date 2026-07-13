# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="screen"
PKG_VERSION="5.0.2"
PKG_SHA256="ca9a2c7e240919bc7ac12124593ae4529bb4eb5f7349d8857829b7e3f0b3b332"
PKG_LICENSE="GPL-3.0-or-later"
PKG_SITE="https://www.gnu.org/software/screen/"
PKG_URL="https://ftpmirror.gnu.org/screen/${PKG_NAME}-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain ncurses"
PKG_LONGDESC="Screen is a window manager that multiplexes a physical terminal between several processes"
PKG_BUILD_FLAGS="-sysroot -parallel -cfg-libs"
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_TARGET="ac_cv_header_utempter_h=no \
                           --disable-pam \
                           --disable-telnet \
                           --disable-socket-dir"
