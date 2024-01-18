# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="polkit"
PKG_VERSION="124"
PKG_SHA256="a4693bb00a8eaa6fbf766b9771dd9e1e11343678dee7e14539b9d6a808f00166"
PKG_LICENSE="GPL"
PKG_SITE="https://www.freedesktop.org/software/polkit/docs/latest/"
PKG_URL="https://gitlab.freedesktop.org/polkit/polkit/-/archive/${PKG_VERSION}/polkit-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_TARGET="toolchain expat glib"
PKG_LONGDESC="polkit provides an authorization API intended to be used by privileged programs offering service to unprivileged programs"
PKG_BUILD_FLAGS="-sysroot"

PKG_MESON_OPTS_TARGET="-Dauthfw=shadow \
                       -Dlibs-only=true \
                       -Dintrospection=false"
