# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="cli"
PKG_VERSION="$(get_pkg_version moby)"
PKG_SHA256="47d6dae52c7fff35cf9e41d465c57626564c8ba58447b4ef345901977f63ac8a"
PKG_LICENSE="ASL"
PKG_SITE="https://github.com/docker/cli"
PKG_URL="https://github.com/docker/cli/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain go:host"
PKG_LONGDESC="The Docker CLI"
PKG_TOOLCHAIN="manual"

# Git commit of the matching tag https://github.com/docker/cli/tags
export PKG_GIT_COMMIT="5927d80c76b3ce5cf782be818922966e8a0d87a3"

configure_target() {
  go_configure

  export LDFLAGS="-w -linkmode external -extldflags -Wl,--unresolved-symbols=ignore-in-shared-libs -extld ${CC}"

  # used for docker version
  export GITCOMMIT=${PKG_GIT_COMMIT}
  export VERSION=${PKG_VERSION}
  export BUILDTIME="$(date --utc)"

  cat >"${PKG_BUILD}/go.mod" <<EOF
module github.com/docker/cli

go 1.18
EOF

  GO111MODULE=auto ${GOLANG} mod tidy -modfile 'vendor.mod' -compat 1.18
  GO111MODULE=auto ${GOLANG} mod vendor -modfile vendor.mod
}

make_target() {
  mkdir -p bin
  PKG_CLI_FLAGS="-X 'github.com/docker/cli/cli/version.Version=${VERSION}'"
  PKG_CLI_FLAGS+=" -X 'github.com/docker/cli/cli/version.GitCommit=${GITCOMMIT}'"
  PKG_CLI_FLAGS+=" -X 'github.com/docker/cli/cli/version.BuildTime=${BUILDTIME}'"
  ${GOLANG} build -mod=mod -modfile=vendor.mod -v -o bin/docker -a -tags "${PKG_DOCKER_BUILDTAGS}" -ldflags "${LDFLAGS} ${PKG_CLI_FLAGS}" ./cmd/docker
}

makeinstall_target() {
  :
}
