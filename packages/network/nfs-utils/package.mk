# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)
# Copyright (C) 2024-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="nfs-utils"
PKG_VERSION="2.6.4"
PKG_SHA256="01b3b0fb9c7d0bbabf5114c736542030748c788ec2fd9734744201e9b0a1119d"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="http://www.linux-nfs.org/"
PKG_URL="https://www.kernel.org/pub/linux/utils/nfs-utils/${PKG_VERSION}/nfs-utils-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_HOST="toolchain"
PKG_DEPENDS_TARGET="toolchain keyutils libevent libtirpc rpcbind sqlite util-linux nfs-utils:host systemd rpcsvc-proto libdevmapper"
PKG_LONGDESC="The NFS Utilities package contains the userspace server and client tools necessary to use the kernel NFS abilities."

post_unpack() {
  # we use own proc-fs-nfsd.mount file to also load nfsd module
  cp $PKG_DIR/system.d/* $PKG_BUILD/systemd

  # move path /var/lib/nfs -> /run/nfs
  #   nfsdcld[3268]: cld_inotify_setup: inotify_add_watch failed: No such file or directory
  find $PKG_BUILD -type f -exec sed -i \
    -e 's|/var/lib/nfs|/run/nfs|g' \
    -e 's|var-lib-nfs|run-nfs|g' {} \;

  mv $PKG_BUILD/systemd/var-lib-nfs-rpc_pipefs.mount \
     $PKG_BUILD/systemd/run-nfs-rpc_pipefs.mount
  mv $PKG_BUILD/systemd/var-lib-nfs-rpc_pipefs.mount.in \
     $PKG_BUILD/systemd/run-nfs-rpc_pipefs.mount.in
}

pre_configure_host() {
  cd $PKG_BUILD
  rm -rf .$HOST_NAME

  PKG_CONFIGURE_OPTS_HOST=" \
    --with-statedir=/run/nfs \
    --with-rpcgen=internal \
    libsqlite3_cv_is_recent=unknown \
    ac_cv_header_rpc_rpc_h=yes \
    ac_cv_header_event2_event_h=yes \
    ac_cv_lib_event_core_event_base_dispatch=yes \
    --disable-nfsdcld \
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
