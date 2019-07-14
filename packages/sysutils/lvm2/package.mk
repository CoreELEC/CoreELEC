PKG_NAME=lvm2
PKG_VERSION=2.03.02

PKG_URL="http://mirrors.kernel.org/sourceware/lvm2/releases/LVM2.$PKG_VERSION.tgz"
PKG_HASH=550ba750239fd75b7e52c9877565cabffef506bbf6d7f6f17b9700dee56c720f

PKG_DEPENDS_TARGET="toolchain systemd readline util-linux"
PKG_SHORTDESC="Logical Volume Manager 2 utilities"


PKG_CONFIGURE_OPTS_TARGET="ac_cv_func_malloc_0_nonnull=yes \
			   ac_cv_func_realloc_0_nonnull=yes \
                           --disable-o_direct"
