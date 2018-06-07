################################################################################
#      This file is part of OpenELEC - http://www.openelec.tv
#      Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
#
#  OpenELEC is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 2 of the License, or
#  (at your option) any later version.
#
#  OpenELEC is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with OpenELEC.  If not, see <http://www.gnu.org/licenses/>.
################################################################################

PKG_NAME="openssh"
PKG_VERSION="7.5p1"
PKG_SHA256="9846e3c5fab9f0547400b4d2c017992f914222b3fd1f8eee6c7dc6bc5e59f9f0"
PKG_ARCH="any"
PKG_LICENSE="OSS"
PKG_SITE="http://www.openssh.com/"
PKG_URL="http://ftp.openbsd.org/pub/OpenBSD/OpenSSH/portable/$PKG_NAME-$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain zlib openssl"
PKG_SECTION="network"
PKG_SHORTDESC="openssh: An open re-implementation of the SSH package"
PKG_LONGDESC="This is a Linux port of OpenBSD's excellent OpenSSH. OpenSSH is based on the last free version of Tatu Ylonen's SSH with all patent-encumbered algorithms removed, all known security bugs fixed, new features reintroduced, and many other clean-ups. SSH (Secure Shell) is a program to log into another computer over a network, to execute commands in a remote machine, and to move files from one machine to another. It provides strong authentication and secure communications over insecure channels. It is intended as a replacement for rlogin, rsh, rcp, and rdist."
PKG_TOOLCHAIN="autotools"
PKG_BUILD_FLAGS="+lto"

PKG_CONFIGURE_OPTS_TARGET="--sysconfdir=/etc/ssh \
                           --libexecdir=/usr/lib/openssh \
                           --disable-strip \
                           --disable-lastlog \
                           --with-sandbox=no \
                           --disable-utmp \
                           --disable-utmpx \
                           --disable-wtmp \
                           --disable-wtmpx \
                           --without-rpath \
                           --with-ssl-engine \
                           --with-privsep-user=nobody \
                           --disable-pututline \
                           --disable-pututxline \
                           --disable-etc-default-login \
                           --with-keydir=/storage/.cache/ssh \
                           --without-pam"

pre_configure_target() {
  export LD="$CC"
  export LDFLAGS="$TARGET_CFLAGS $TARGET_LDFLAGS"
}

post_makeinstall_target() {
  rm -rf $INSTALL/usr/lib/openssh/ssh-keysign
  rm -rf $INSTALL/usr/lib/openssh/ssh-pkcs11-helper
  if [ ! $SFTP_SERVER = "yes" ]; then
    rm -rf $INSTALL/usr/lib/openssh/sftp-server
  fi
  rm -rf $INSTALL/usr/bin/ssh-add
  rm -rf $INSTALL/usr/bin/ssh-agent
  rm -rf $INSTALL/usr/bin/ssh-keyscan

  sed -e "s|^#PermitRootLogin.*|PermitRootLogin yes|g" \
      -e "s|^#StrictModes.*|StrictModes no|g" \
      -i $INSTALL/etc/ssh/sshd_config

  echo "PubkeyAcceptedKeyTypes +ssh-dss" >> $INSTALL/etc/ssh/sshd_config

  debug_strip $INSTALL/usr
}

post_install() {
  enable_service sshd.service
}
