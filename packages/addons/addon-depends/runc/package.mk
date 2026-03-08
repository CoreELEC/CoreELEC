# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="runc"
PKG_VERSION="1.4.0"
PKG_SHA256="94d566d8b017d6cdffc684560a4f069bb87f86534976c41d768711c85e194884"
PKG_LICENSE="APL"
PKG_SITE="https://github.com/opencontainers/runc"
PKG_URL="https://github.com/opencontainers/runc/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain go:host libseccomp"
PKG_DEPENDS_CONFIG="libseccomp"
PKG_LONGDESC="A CLI tool for spawning and running containers according to the OCI specification."
PKG_TOOLCHAIN="manual"

# Git commit of the matching release https://github.com/opencontainers/runc/releases
export PKG_GIT_COMMIT="8bd78a9977e604c4d5f67a7415d7b8b8c109cdc4"

pre_make_target() {
  go_configure

  export LDFLAGS="-w -extldflags -static -X main.gitCommit=${PKG_GIT_COMMIT} -X main.version=$(cat ./VERSION) -extld ${CC}"

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
