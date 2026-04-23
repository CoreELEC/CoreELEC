# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2020-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="sed"
PKG_VERSION="4.10"
PKG_SHA256="b8e72182b2ec96a3574e2998c47b7aaa64cc20ce000d8e9ac313cc07cecf28c7"
PKG_LICENSE="GPL"
PKG_SITE="http://www.gnu.org/software/sed/"
PKG_URL="https://mirrors.kernel.org/gnu/sed/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_HOST="ccache:host"
PKG_LONGDESC="The sed (Stream EDitor) editor is a stream or batch (non-interactive) editor."
PKG_BUILD_FLAGS="-cfg-libs:host"

PKG_CONFIGURE_OPTS_HOST="--disable-nls --disable-acl --without-selinux"
