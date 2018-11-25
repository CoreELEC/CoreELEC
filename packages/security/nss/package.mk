# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2012 Stephan Raue (stephan@openelec.tv)

PKG_NAME="nss"
PKG_VERSION="3.37.1"
PKG_SHA256="5ac4a388b06b2785fb7f4f0ae3c909d8cbb2ab370147ff61fe2517cc5bf3c685"
PKG_LICENSE="Mozilla Public License"
PKG_SITE="http://ftp.mozilla.org/"
PKG_URL="http://ftp.mozilla.org/pub/security/nss/releases/NSS_3_37_1_RTM/src/nss-3.37.1-with-nspr-4.19.tar.gz"
PKG_DEPENDS_HOST="nspr:host zlib:host"
PKG_DEPENDS_TARGET="toolchain nss:host nspr zlib sqlite"
PKG_LONGDESC="The Network Security Services (NSS) package is a set of libraries designed to support cross-platform development of security-enabled client and server applications"
PKG_TOOLCHAIN="manual"
PKG_BUILD_FLAGS="-parallel"

make_host() {
  cd $PKG_BUILD/nss

  make clean || true
  rm -rf $PKG_BUILD/dist

  INCLUDES="-I$TOOLCHAIN/include" \
  make BUILD_OPT=1 USE_64=1 \
     PREFIX=$TOOLCHAIN \
     NSPR_INCLUDE_DIR=$TOOLCHAIN/include/nspr \
     USE_SYSTEM_ZLIB=1 ZLIB_LIBS="-lz -L$TOOLCHAIN/lib" \
     SKIP_SHLIBSIGN=1 \
     NSS_TESTS="dummy" \
     CC=$CC LDFLAGS="$LDFLAGS -L$TOOLCHAIN/lib" \
     V=1
}

makeinstall_host() {
  cd $PKG_BUILD
  $STRIP dist/Linux*/lib/*.so
  cp -L dist/Linux*/lib/*.so $TOOLCHAIN/lib
  cp -L dist/Linux*/lib/libcrmf.a $TOOLCHAIN/lib
  mkdir -p $TOOLCHAIN/include/nss
  cp -RL dist/{public,private}/nss/* $TOOLCHAIN/include/nss
  cp -L dist/Linux*/lib/pkgconfig/nss.pc $TOOLCHAIN/lib/pkgconfig
  cp -L nss/coreconf/nsinstall/*/nsinstall $TOOLCHAIN/bin
}

make_target() {
  cd $PKG_BUILD/nss

  local TARGET_USE_64=""
  [ "$TARGET_ARCH" = "x86_64" -o "$TARGET_ARCH" = "aarch64" ] && TARGET_USE_64="USE_64=1"

  make clean || true
  rm -rf $PKG_BUILD/dist

  make BUILD_OPT=1 $TARGET_USE_64 \
     NSS_USE_SYSTEM_SQLITE=1 \
     NSPR_INCLUDE_DIR=$SYSROOT_PREFIX/usr/include/nspr \
     NSS_USE_SYSTEM_SQLITE=1 \
     USE_SYSTEM_ZLIB=1 ZLIB_LIBS=-lz \
     SKIP_SHLIBSIGN=1 \
     OS_TEST=$TARGET_ARCH \
     NSS_TESTS="dummy" \
     NSINSTALL=$TOOLCHAIN/bin/nsinstall \
     CPU_ARCH_TAG=$TARGET_ARCH \
     CC=$CC \
     LDFLAGS="$LDFLAGS -L$SYSROOT_PREFIX/usr/lib" \
     V=1
}

makeinstall_target() {
  cd $PKG_BUILD
  $STRIP dist/Linux*/lib/*.so
  cp -L dist/Linux*/lib/*.so $SYSROOT_PREFIX/usr/lib
  cp -L dist/Linux*/lib/libcrmf.a $SYSROOT_PREFIX/usr/lib
  mkdir -p $SYSROOT_PREFIX/usr/include/nss
  cp -RL dist/{public,private}/nss/* $SYSROOT_PREFIX/usr/include/nss
  cp -L dist/Linux*/lib/pkgconfig/nss.pc $SYSROOT_PREFIX/usr/lib/pkgconfig

  mkdir -p .install_pkg/usr/lib
    cp -PL dist/Linux*/lib/*.so .install_pkg/usr/lib
}
