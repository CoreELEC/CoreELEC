# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="nfs-utils"
PKG_VERSION="2.4.3"
PKG_SHA256="a74cb744da77771dcfaad735d711a85c31246e5056be2926e069a8a5b4668d63"
PKG_LICENSE="GPL-2.0+"
PKG_SITE="http://nfs.sourceforge.net/"
PKG_URL="https://iweb.dl.sourceforge.net/project/nfs/nfs-utils/$PKG_VERSION/$PKG_NAME-$PKG_VERSION.tar.bz2"
PKG_DEPENDS_HOST="toolchain"
PKG_DEPENDS_TARGET="toolchain nfs-utils:host systemd sqlite libtirpc rpcsvc-proto libevent libdevmapper"
PKG_LONGDESC="The NFS Utilities package contains the userspace server and client tools necessary to use the kernel NFS abilities."

post_unpack() {
  # we use own proc-fs-nfsd.mount file to also load nfsd module
  cp $PKG_DIR/system.d/* $PKG_BUILD/systemd
}

pre_configure_host() {
  cd $PKG_BUILD
  rm -rf .$HOST_NAME

  PKG_CONFIGURE_OPTS_HOST=" \
    --with-statedir=/run/nfs \
    --with-rpcgen=internal \
    --disable-nfsv4 \
    --disable-nfsv41 \
    --disable-gss \
    --disable-uuid \
    --disable-ipv6 \
    --disable-caps \
    --disable-tirpc \
    --without-systemd \
    --without-tcp-wrappers"
}

pre_configure_target() {
  cd $PKG_BUILD
  rm -rf .$TARGET_NAME

  PKG_CONFIGURE_OPTS_TARGET=" \
    --with-systemd=/usr/lib/systemd/system \
    --with-nfsconfig=/storage/.config/nfs.conf \
    --with-statduser=$(whoami) \
    --with-statedir=/run/nfs \
    --with-rpcgen=$PKG_BUILD/tools/rpcgen/rpcgen \
    --enable-nfsv4 \
    --enable-nfsv41 \
    --enable-tirpc \
    --enable-uuid \
    --disable-gss \
    --disable-ipv6 \
    --without-tcp-wrappers"

  # use different paths /etc -> /storage/.config
  # /etc/exports
  CFLAGS+=" -D_PATH_EXPORTS=\\\"/storage/.config/exports\\\""
  # /etc/exports.d
  CFLAGS+=" -D_PATH_EXPORTS_D=\\\"/storage/.config/exports.d\\\""
  # /etc/idmapd.conf
  CFLAGS+=" -D_PATH_IDMAPDCONF=\\\"/storage/.config/idmapd.conf\\\""
  # we don't have nobody user and group
  CFLAGS+=" -DNFS4NOBODY_USER=\\\"root\\\""
  CFLAGS+=" -DNFS4NOBODY_GROUP=\\\"root\\\""
}

make_host() {
  make rpcgen -C tools/rpcgen
}

makeinstall_host() {
  : #
}

post_makeinstall_target() {
  mkdir -p $INSTALL/usr/config

  cp nfs.conf $INSTALL/usr/config
  cp support/nfsidmap/idmapd.conf $INSTALL/usr/config
  cp $PKG_DIR/config/* $INSTALL/usr/config

  # we use tmpfs for it
  rm -fr "$INSTALL/run"

  # we have symbolic link to /usr/sbin
  mkdir -p $INSTALL/usr/sbin
  chmod 755 $INSTALL/sbin/*
  mv $INSTALL/sbin/* $INSTALL/usr/sbin
  rmdir $INSTALL/sbin
}

post_install() {
  enable_service nfs-server.service
}
