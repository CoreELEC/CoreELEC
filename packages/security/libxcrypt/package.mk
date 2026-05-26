# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libxcrypt"
PKG_VERSION="4.5.2"
PKG_SHA256="71513a31c01a428bccd5367a32fd95f115d6dac50fb5b60c779d5c7942aec071"
PKG_LICENSE="LGPL-2.1-or-later"
PKG_SITE="https://github.com/besser82/libxcrypt"
PKG_URL="https://github.com/besser82/libxcrypt/releases/download/v${PKG_VERSION}/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="glibc"
PKG_LONGDESC="Extended crypt library for descrypt, md5crypt, bcrypt, and others"

if [ "${MOLD_SUPPORT}" = "yes" ]; then
  PKG_DEPENDS_TARGET+=" mold:host"
fi

PKG_CONFIGURE_OPTS_TARGET="--disable-obsolete-api"
