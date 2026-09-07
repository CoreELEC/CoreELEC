# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="xmlstarlet"
PKG_VERSION="1.7.0"
PKG_SHA256="df4c93458fc6b2ad144d65dccf3ef251349871e0578c5bc59edbfeecf9bf3454"
PKG_LICENSE="MIT"
PKG_SITE="https://xmlstarlet.github.io/"
PKG_URL="https://github.com/xmlstarlet/xmlstarlet/releases/download/${PKG_VERSION}/${PKG_NAME}-${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="libxml2:host libxslt:host"
PKG_DEPENDS_TARGET="toolchain libxml2 libxslt"
PKG_LONGDESC="XMLStarlet is a command-line XML utility which allows the modification and validation of XML documents."
PKG_BUILD_FLAGS="-cfg-libs -cfg-libs:host"

post_makeinstall_host() {
  ln -sf xml ${TOOLCHAIN}/bin/xmlstarlet
}

post_makeinstall_target() {
  ln -sf xml ${INSTALL}/usr/bin/xmlstarlet
}
