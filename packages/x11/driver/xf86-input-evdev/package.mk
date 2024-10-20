# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="xf86-input-evdev"
PKG_VERSION="2.11.0"
PKG_SHA256="730022de934cc366bb12439daf202a7bfff52a028cf4573e457642e25a071315"
PKG_LICENSE="OSS"
PKG_SITE="https://www.X.org"
PKG_URL="https://xorg.freedesktop.org/archive/individual/driver/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain xorg-server util-macros libevdev mtdev systemd"
PKG_LONGDESC="Evdev is an Xorg input driver for Linux's generic event devices."
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_TARGET="--disable-silent-rules \
                           --with-xorg-module-dir=${XORG_PATH_MODULES} \
                           --with-gnu-ld"
