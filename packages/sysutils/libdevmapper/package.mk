PKG_NAME="libdevmapper"
PKG_VERSION="2.03.10"
PKG_LICENSE="GPLv2 LGPL2.1"
PKG_SITE="https://sourceware.org/lvm2"
PKG_URL="http://mirrors.kernel.org/sourceware/lvm2/releases/LVM2.$PKG_VERSION.tgz"
PKG_SHA256="5ad1645a480440892e35f31616682acba0dc204ed049635d2df3e5a5929d0ed0"
PKG_LONGDESC="libdevmapper library from lvm2 package"
PKG_DEPENDS_TARGET="toolchain systemd readline util-linux"

PKG_CONFIGURE_OPTS_TARGET="ac_cv_func_malloc_0_nonnull=yes \
                           ac_cv_func_realloc_0_nonnull=yes \
                           --disable-o_direct \
                           --disable-nsl \
                           --disable-selinux \
                           --disable-symvers"

post_makeinstall_target() {
  rm -rf $INSTALL/etc
  rm -rf $INSTALL/usr/bin
}
