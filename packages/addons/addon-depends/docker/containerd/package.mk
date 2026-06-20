# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="containerd"
PKG_VERSION="2.3.2"
PKG_SHA256="1a215ae4acb184192668b21f8b8375ceb6e86f8832a97fe6f7ab53ad79bb2cee"
PKG_LICENSE="Apache-2.0"
PKG_SITE="https://containerd.io"
PKG_URL="https://github.com/containerd/containerd/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain go:host"
PKG_LONGDESC="A daemon to control runC, built for performance and density."
PKG_TOOLCHAIN="manual"

# Git commit of the matching release https://github.com/containerd/containerd/releases
export PKG_GIT_COMMIT="fff62f14765df376e5fc36f5a8f8e795b5670f61"

pre_make_target() {

  go_configure

  export CONTAINERD_VERSION="${PKG_VERSION}"
  export CONTAINERD_REVISION="${PKG_GIT_COMMIT}"
  export CONTAINERD_PKG="github.com/containerd/containerd/v2"
  export LDFLAGS="-w -extldflags -static -X ${CONTAINERD_PKG}/version.Version=${CONTAINERD_VERSION} -X ${CONTAINERD_PKG}/version.Revision=${CONTAINERD_REVISION} -X ${CONTAINERD_PKG}/version.Package=${CONTAINERD_PKG} -extld ${CC}"
  export GO111MODULE=off

  mkdir -p ${GOPATH}
  if [ -d ${PKG_BUILD}/vendor ]; then
    mv ${PKG_BUILD}/vendor ${GOPATH}/src
  fi

  mv ${GOPATH}/src/github.com/containerd/containerd/api ${PKG_BUILD}/api-vendor-duplicate
  ln -fs ${PKG_BUILD} ${GOPATH}/src/github.com/containerd/containerd/v2
  ln -fs ${PKG_BUILD}/api ${GOPATH}/src/github.com/containerd/containerd/api

  sed -i "s#/etc/containerd#/storage/.kodi/userdata/addon_data/service.system.docker/config#" \
    ${PKG_BUILD}/defaults/defaults_unix.go
}

make_target() {
  mkdir -p bin
  ${GOLANG} build -v -o bin/containerd              -a -tags "static_build no_cri no_btrfs no_devmapper no_zfs" -ldflags "${LDFLAGS}" ./cmd/containerd
  ${GOLANG} build -v -o bin/containerd-shim-runc-v2 -a -tags "static_build no_cri no_btrfs no_devmapper no_zfs" -ldflags "${LDFLAGS}" ./cmd/containerd-shim-runc-v2
}
