PKG_NAME=cryptsetup
PKG_VERSION=2.1.0
PKG_LICENSE=GPL
PKG_URL=https://www.kernel.org/pub/linux/utils/cryptsetup/v2.1/$PKG_NAME-$PKG_VERSION.tar.xz
PKG_LONGDESC="cryptsetup utility for managing LUKS containers"
PKG_DEPENDS_HOST="toolchain ccache:host"
PKG_DEPENDS_TARGET="toolchain popt lvm2 util-linux json-c openssl"

PKG_CONFIGURE_OPTS_TARGET="
        --disable-cryptsetup-reencrypt \
	--disable-integritysetup \
	--disable-selinux \
	--disable-rpath \
	--disable-veritysetup \
	--disable-udev"

PKG_CONFIGURE_OPTS_HOST="$PKG_CONFIGURE_OPTS_TARGET"

makeinstall_target() {
  mkdir -p $INSTALL/bin
    cp ../.$TARGET_NAME/.libs/cryptsetup $INSTALL/bin

  mkdir -p $INSTALL/lib
    cp ../.$TARGET_NAME/.libs/libcryptsetup.so.12.4.0 $INSTALL/lib
    ln -sf $(basename $INSTALL/lib/libcryptsetup.so.12.4.0) $INSTALL/lib/libcryptsetup.so.12
    ln -sf $(basename $INSTALL/lib/libcryptsetup.so.12.4.0) $INSTALL/lib/libcryptsetup.so
}
