# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="podman-bin"
PKG_VERSION="5.8.2"
PKG_SHA256="b20ea65afc5a58ea1cea019bd51a5d84eb9042d25d3eb82c55010c8815732d84"
PKG_LICENSE="Apache-2.0"
PKG_SITE="https://podman.io/"
PKG_URL="https://github.com/containers/podman/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain go:host gpgme libassuan libseccomp systemd"
PKG_DEPENDS_CONFIG="gpgme libassuan libseccomp"
PKG_LONGDESC="Podman: A tool for managing OCI containers and pods."
PKG_TOOLCHAIN="manual"

# Git commit of the matching release https://github.com/containers/podman
export PKG_GIT_COMMIT="5b263b5f5b48004a87caac44e67349a8266d9ef4"

PKG_PODMAN_BUILDTAGS="exclude_graphdriver_devicemapper \
                      exclude_graphdriver_btrfs \
                      seccomp \
                      systemd"

configure_target() {
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
