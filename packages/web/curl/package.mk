# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

# Notes:
# - build curl with OpenSSL support instead GnuTLS support to
#   work around a long standing bug on Pi where https streams often hang on
#   start. This hang is normally fatal and requires a reboot.
#   see also http://trac.xbmc.org/ticket/14674 .
#   Easiest way to reproduce is to install gdrive addon and play a video from
#   there: http://forum.xbmc.org/showthread.php?tid=177557

PKG_NAME="curl"
PKG_VERSION="7.61.1"
PKG_SHA256="a308377dbc9a16b2e994abd55455e5f9edca4e31666f8f8fcfe7a1a4aea419b9"
PKG_LICENSE="MIT"
PKG_SITE="http://curl.haxx.se"
PKG_URL="http://curl.haxx.se/download/$PKG_NAME-$PKG_VERSION.tar.bz2"
PKG_DEPENDS_TARGET="toolchain zlib openssl rtmpdump"
PKG_LONGDESC="Client and library for (HTTP, HTTPS, FTP, ...) transfers."
PKG_TOOLCHAIN="configure"

PKG_CONFIGURE_OPTS_TARGET="ac_cv_lib_rtmp_RTMP_Init=yes \
                           ac_cv_header_librtmp_rtmp_h=yes \
                           --disable-debug \
                           --enable-optimize \
                           --enable-warnings \
                           --disable-curldebug \
                           --disable-ares \
                           --enable-largefile \
                           --enable-http \
                           --enable-ftp \
                           --enable-file \
                           --disable-ldap \
                           --disable-ldaps \
                           --enable-rtsp \
                           --enable-proxy \
                           --disable-dict \
                           --disable-telnet \
                           --disable-tftp \
                           --disable-pop3 \
                           --disable-imap \
                           --disable-smb \
                           --disable-smtp \
                           --disable-gopher \
                           --disable-manual \
                           --enable-libgcc \
                           --enable-ipv6 \
                           --enable-versioned-symbols \
                           --enable-nonblocking \
                           --enable-threaded-resolver \
                           --enable-verbose \
                           --disable-sspi \
                           --enable-crypto-auth \
                           --enable-cookies \
                           --enable-symbol-hiding \
                           --disable-soname-bump \
                           --with-gnu-ld \
                           --without-krb4 \
                           --without-spnego \
                           --without-gssapi \
                           --with-zlib \
                           --without-egd-socket \
                           --enable-thread \
                           --with-random=/dev/urandom \
                           --without-gnutls \
                           --with-ssl \
                           --without-polarssl \
                           --without-nss \
                           --with-ca-bundle=/run/libreelec/cacert.pem \
                           --without-ca-path \
                           --without-libpsl \
                           --without-libmetalink \
                           --without-libssh2 \
                           --with-librtmp=$SYSROOT_PREFIX/usr \
                           --without-libidn"

pre_configure_target() {
# link against librt because of undefined reference to 'clock_gettime'
  export LIBS="-lrt -lm -lrtmp"
}

post_makeinstall_target() {
  rm -rf $INSTALL/usr/share/zsh
  rm -rf $INSTALL/usr/bin/curl-config

  sed -e "s:\(['= ]\)/usr:\\1$SYSROOT_PREFIX/usr:g" -i $SYSROOT_PREFIX/usr/bin/curl-config
}
