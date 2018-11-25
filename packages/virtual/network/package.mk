# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)

PKG_NAME="network"
PKG_VERSION=""
PKG_LICENSE="various"
PKG_SITE="http://www.openelec.tv"
PKG_URL=""
PKG_DEPENDS_TARGET="toolchain connman netbase ethtool openssh"
PKG_SECTION="virtual"
PKG_LONGDESC="Metapackage for various packages to install network support"

if [ "$BLUETOOTH_SUPPORT" = "yes" ]; then
  PKG_DEPENDS_TARGET="$PKG_DEPENDS_TARGET bluez"
fi

if [ "$SAMBA_SERVER" = "yes" ] || [ "$SAMBA_SUPPORT" = "yes" ]; then
  PKG_DEPENDS_TARGET="$PKG_DEPENDS_TARGET samba"
fi

if [ "$OPENVPN_SUPPORT" = "yes" ]; then
  PKG_DEPENDS_TARGET="$PKG_DEPENDS_TARGET openvpn"
fi

# nss needed by inputstream.adaptive, chromium etc.
if [ "$TARGET_ARCH" = "x86_64" ] || [ "$TARGET_ARCH" = "arm" ]; then
  PKG_DEPENDS_TARGET="$PKG_DEPENDS_TARGET nss"
fi
