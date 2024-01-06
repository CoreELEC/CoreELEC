# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Lukas Rusak (lrusak@libreelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="runc"
PKG_VERSION="1.1.11"
PKG_SHA256="129ad7cd470e4bdc82cdd3d5f0aa93e43674bf8ea115da2b42a27361885fe3ee"
PKG_LICENSE="APL"
PKG_SITE="https://github.com/opencontainers/runc"
PKG_URL="https://github.com/opencontainers/runc/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain go:host libseccomp"
PKG_LONGDESC="A CLI tool for spawning and running containers according to the OCI specification."
PKG_TOOLCHAIN="manual"

# Git commit of the matching release https://github.com/opencontainers/runc/releases
PKG_GIT_COMMIT="4bccb38cc9cf198d52bebf2b3a90cd14e7af8c06"

pre_make_target() {
  go_configure

  export LDFLAGS="-w -extldflags -static -X main.gitCommit=${PKG_GIT_COMMIT} -X main.version=$(cat ./VERSION) -extld ${CC}"
  export PKG_CONFIG_PATH="$(get_install_dir libseccomp)/usr/lib/pkgconfig:${PKG_CONFIG_PATH}"

  mkdir -p ${GOPATH}
  if [ -d ${PKG_BUILD}/vendor ]; then
    mv ${PKG_BUILD}/vendor ${GOPATH}/src
  fi

  ln -fs ${PKG_BUILD} ${GOPATH}/src/github.com/opencontainers/runc
}

make_target() {
  mkdir -p bin
  ${GOLANG} build -v -o bin/runc -a -tags "cgo seccomp static_build" -ldflags "${LDFLAGS}" ./
}
