# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Lukas Rusak (lrusak@libreelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="containerd"
PKG_VERSION="1.7.19"
PKG_SHA256="152e23d017b053ee786080ec0410c98aa91967f3aaebdcfaaf4521ac2172ef62"
PKG_LICENSE="APL"
PKG_SITE="https://containerd.io"
PKG_URL="https://github.com/containerd/containerd/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain go:host"
PKG_LONGDESC="A daemon to control runC, built for performance and density."
PKG_TOOLCHAIN="manual"

# Git commit of the matching release https://github.com/containerd/containerd/releases
export PKG_GIT_COMMIT="2bf793ef6dc9a18e00cb12efb64355c2c9d5eb41"

pre_make_target() {

  go_configure

  export CONTAINERD_VERSION="${PKG_VERSION}"
  export CONTAINERD_REVISION="${PKG_GIT_COMMIT}"
  export CONTAINERD_PKG="github.com/containerd/containerd"
  export LDFLAGS="-w -extldflags -static -X ${CONTAINERD_PKG}/version.Version=${CONTAINERD_VERSION} -X ${CONTAINERD_PKG}/version.Revision=${CONTAINERD_REVISION} -X ${CONTAINERD_PKG}/version.Package=${CONTAINERD_PKG} -extld ${CC}"
  export GO111MODULE=off

  mkdir -p ${GOPATH}
  if [ -d ${PKG_BUILD}/vendor ]; then
    mv ${PKG_BUILD}/vendor ${GOPATH}/src
  fi

  mv ${GOPATH}/src/github.com/containerd/containerd/api ${PKG_BUILD}/api-vendor-duplicate
  rmdir ${GOPATH}/src/github.com/containerd/containerd
  ln -fs ${PKG_BUILD} ${GOPATH}/src/github.com/containerd/containerd
}

make_target() {
  mkdir -p bin
  ${GOLANG} build -v -o bin/containerd              -a -tags "static_build no_btrfs" -ldflags "${LDFLAGS}" ./cmd/containerd
  ${GOLANG} build -v -o bin/containerd-shim         -a -tags "static_build no_btrfs" -ldflags "${LDFLAGS}" ./cmd/containerd-shim
  ${GOLANG} build -v -o bin/containerd-shim-runc-v2 -a -tags "static_build no_btrfs" -ldflags "${LDFLAGS}" ./cmd/containerd-shim-runc-v2
}
