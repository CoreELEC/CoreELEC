# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2020-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="setxkbmap"
PKG_VERSION="1.3.4"
PKG_SHA256="be8d8554d40e981d1b93b5ff82497c9ad2259f59f675b38f1b5e84624c07fade"
PKG_LICENSE="OSS"
PKG_SITE="https://www.X.org"
PKG_URL="https://xorg.freedesktop.org/archive/individual/app/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain libX11 libxkbfile xrandr"
PKG_LONGDESC="Setxkbmap sets the keyboard using the X Keyboard Extension."
