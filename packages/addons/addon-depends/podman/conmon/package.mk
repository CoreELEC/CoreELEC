# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="conmon"
PKG_VERSION="2.2.1"
PKG_SHA256="814fb5979a3a4b8576b1f901e606b482bebb41cb7e57926e6d5765ee786b96d3"
PKG_LICENSE="Apache-2.0"
PKG_SITE="https://github.com/containers/conmon"
PKG_URL="https://github.com/containers/conmon/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain glib libseccomp systemd"
PKG_LONGDESC="An OCI container runtime monitor"

# Git commit of the matching release https://github.com/containers/conmon
export PKG_GIT_COMMIT="c8cc2c4db27531bd4e084ce7857f73cd21ee639d"

pre_configure_target() {
  export PKG_CONFIG_PATH="$(get_install_dir libseccomp)/usr/lib/pkgconfig:${PKG_CONFIG_PATH}"
  export GIT_COMMIT=${PKG_GIT_COMMIT}
}
