# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)

PKG_NAME="remote"
PKG_VERSION="1"
PKG_LICENSE="GPL"
PKG_SITE="http://www.openelec.tv"
PKG_URL=""
PKG_DEPENDS_TARGET="toolchain eventlircd libirman v4l-utils"
PKG_SECTION="virtual"
PKG_LONGDESC="Meta package for installing various tools needed for remote support"

if [ "$ATVCLIENT_SUPPORT" = "yes" ]; then
  PKG_DEPENDS_TARGET="$PKG_DEPENDS_TARGET atvclient"
fi

if [ "$AMREMOTE_SUPPORT" = "yes" ]; then
  PKG_DEPENDS_TARGET="$PKG_DEPENDS_TARGET amremote"
fi
