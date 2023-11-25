# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="polkit"
PKG_VERSION="123"
PKG_SHA256="72d9119b0aa35da871fd0660601d812c7a3d6af7e4e53e237840b71bb43d0c63"
PKG_LICENSE="GPL"
PKG_SITE="https://www.freedesktop.org/software/polkit/docs/latest/"
PKG_URL="https://gitlab.freedesktop.org/polkit/polkit/-/archive/${PKG_VERSION}/polkit-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_TARGET="toolchain expat glib"
PKG_LONGDESC="polkit provides an authorization API intended to be used by privileged programs offering service to unprivileged programs"
PKG_BUILD_FLAGS="-sysroot"

PKG_MESON_OPTS_TARGET="-Dauthfw=shadow \
                       -Dlibs-only=true \
                       -Dintrospection=false"
