# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="fping"
PKG_VERSION="4.2"
PKG_SHA256="7d339674b6a95aae1d8ad487ff5056fd95b474c3650938268f6a905c3771b64a"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="http://fping.org/"
PKG_URL="http://fping.org/dist/fping-$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_IS_ADDON="no"
PKG_AUTORECONF="no"
PKG_TOOLCHAIN="auto"

PKG_CONFIGURE_OPTS_TARGET="--sbindir=/usr/bin"
