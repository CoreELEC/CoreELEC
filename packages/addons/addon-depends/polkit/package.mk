# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="polkit"
PKG_VERSION="128"
PKG_SHA256="6edb06bf1448e49e428f9597a97af6a05d62146dd0b58f86eeaebcee3f5ecfaa"
PKG_LICENSE="LGPL-2.0-or-later"
PKG_SITE="https://polkit.pages.freedesktop.org/polkit"
PKG_URL="https://github.com/polkit-org/polkit/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain expat glib systemd"
PKG_LONGDESC="polkit provides an authorization API intended to be used by privileged programs offering service to unprivileged programs"
PKG_BUILD_FLAGS="-sysroot"

PKG_MESON_OPTS_TARGET="-Dauthfw=shadow \
                       -Dsession_tracking=logind \
                       -Dlibs-only=true \
                       -Dintrospection=false"
