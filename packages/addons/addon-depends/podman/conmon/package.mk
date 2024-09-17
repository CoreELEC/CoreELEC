# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="conmon"
PKG_VERSION="2.1.12"
PKG_SHA256="842f0b5614281f7e35eec2a4e35f9f7b9834819aa58ecdad8d0ff6a84f6796a6"
PKG_LICENSE="Apache-2.0"
PKG_SITE="https://github.com/containers/conmon"
PKG_URL="https://github.com/containers/conmon/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain glib libseccomp systemd"
PKG_LONGDESC="An OCI container runtime monitor"

# Git commit of the matching release https://github.com/containers/conmon
export PKG_GIT_COMMIT="e8896631295ccb0bfdda4284f1751be19b483264"

pre_configure_target() {
  export PKG_CONFIG_PATH="$(get_install_dir libseccomp)/usr/lib/pkgconfig:${PKG_CONFIG_PATH}"
  export GIT_COMMIT=${PKG_GIT_COMMIT}
}
