# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="scraper"
PKG_VERSION="509443bf66d9fccb1d6aa2417902124bd48f2141"
PKG_SHA256="5784ac4aa35919233774c3c0210d5cae4aa296ec30165b6b7a7cb41a7d98cb6d"
PKG_REV="2"
PKG_ARCH="any"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/sselph/scraper"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain go:host"
PKG_PRIORITY="optional"
PKG_SECTION="emuelec"
PKG_LONGDESC="A scraper for EmulationStation written in Go using hashing"
PKG_TOOLCHAIN="manual"

configure_target() {

  case ${TARGET_ARCH} in
    x86_64)
      export GOARCH=amd64
      ;;
    arm)
      export GOARCH=arm

      case ${TARGET_CPU} in
        arm1176jzf-s)
          export GOARM=6
          ;;
        *)
          export GOARM=7
          ;;
      esac
      ;;
    aarch64)
      export GOARCH=arm64
      ;;
  esac

  export GOOS=linux
  export GOLANG=${TOOLCHAIN}/lib/golang/bin/go
  export LDFLAGS="-w -extldflags -static -X main.gitCommit=${PKG_VERSION} -X main.versionStr=${PKG_VERSION:0:7} -extld $CC"
}

make_target() {
  mkdir -p bin
  cd $PKG_BUILD
  ${GOLANG} get github.com/sselph/scraper
  ${GOLANG} build -ldflags "$LDFLAGS" github.com/sselph/scraper
}

makeinstall_target() {
mkdir -p $INSTALL/usr/bin/
    cp $PKG_BUILD/scraper $INSTALL/usr/bin/
}
