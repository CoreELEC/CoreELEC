# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Lukas Rusak (lrusak@libreelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="go"
PKG_VERSION="1.10.5"
PKG_SHA256="afe38b3c6a91e7e6c2bae0cf7427d1080ed3b7d8c044504400db56c3b72bb082"
PKG_LICENSE="BSD"
PKG_SITE="https://golang.org"
PKG_URL="https://github.com/golang/go/archive/${PKG_NAME}${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="toolchain"
PKG_LONGDESC="An programming language that makes it easy to build simple, reliable, and efficient software."
PKG_TOOLCHAIN="manual"

####################################################################
# On Fedora `dnf install golang` will install go to /usr/lib/golang
#
# On Ubuntu you need to install golang:
# $ sudo apt install golang-go
####################################################################

configure_host() {
  export GOOS=linux
  export GOROOT_FINAL=$TOOLCHAIN/lib/golang
  if [ -x /usr/lib/go/bin/go ]; then
    export GOROOT_BOOTSTRAP=/usr/lib/go
  elif [ -x /usr/lib/go-1.10/bin/go ]; then
    export GOROOT_BOOTSTRAP=/usr/lib/go-1.10
  else
    export GOROOT_BOOTSTRAP=/usr/lib/golang
  fi
  export GOARCH=amd64
}

make_host() {
  cd $PKG_BUILD/src
  bash make.bash --no-banner
}

pre_makeinstall_host() {
  # need to cleanup old golang version when updating to a new version
  rm -rf $TOOLCHAIN/lib/golang
}

makeinstall_host() {
  mkdir -p $TOOLCHAIN/lib/golang
  cp -av $PKG_BUILD/* $TOOLCHAIN/lib/golang/
}
