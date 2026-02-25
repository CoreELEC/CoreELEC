# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="podman-bin"
PKG_VERSION="5.8.0"
PKG_SHA256="19723cda810e087ded8903fb0f33918b10d81f7fd1d8964880c41ec30d1daa70"
PKG_LICENSE="Apache-2.0"
PKG_SITE="https://podman.io/"
PKG_URL="https://github.com/containers/podman/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain go:host gpgme libassuan libseccomp systemd"
PKG_LONGDESC="Podman: A tool for managing OCI containers and pods."
PKG_TOOLCHAIN="manual"

# Git commit of the matching release https://github.com/containers/podman
export PKG_GIT_COMMIT="07efc23e05c3d9aa15a0f30d57194737bfc4b6b1"

PKG_PODMAN_BUILDTAGS="exclude_graphdriver_devicemapper \
                      exclude_graphdriver_btrfs \
                      seccomp \
                      systemd"

configure_target() {
  export PKG_CONFIG_PATH="$(get_install_dir libassuan)/usr/lib/pkgconfig:$(get_install_dir gpgme)/usr/lib/pkgconfig:$(get_install_dir libseccomp)/usr/lib/pkgconfig:${PKG_CONFIG_PATH}"

  go_configure

  # used for podman commit
  export GIT_COMMIT=${PKG_GIT_COMMIT}
}

make_target() {
  make BUILDTAGS="${PKG_PODMAN_BUILDTAGS}" \
       CGO_CPPFLAGS="-I$(get_install_dir gpgme)/usr/include" \
       GO="${GOLANG}" \
       PREFIX=/storage/.kodi/addons/service.system.podman \
       HELPER_BINARIES_DIR=/storage/.kodi/addons/service.system.podman/bin \
       EXTRA_LDFLAGS="-X github.com/containers/image/v5/sysregistries.systemRegistriesConfPath=/storage/.kodi/userdata/addon_data/service.system.podman/podman/etc/containers/registries.conf -X github.com/containers/image/v5/sysregistries.systemRegistriesConfDirectoryPath=/storage/.kodi/userdata/addon_data/service.system.podman/podman/etc/containers/registries.conf.d -X github.com/containers/image/v5/signature.systemDefaultPolicyPath=/storage/.kodi/addons/service.system.podman/etc/containers/policy.json -extldflags=-L$(get_install_dir gpgme)/usr/lib" \
       binaries
}

makeinstall_target() {
  :
}
