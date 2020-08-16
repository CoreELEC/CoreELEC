PKG_NAME="glesheaders"
PKG_VERSION="d4000def121b818ae0f583d8372d57643f723fdc"
PKG_SHA256="4f2103fc927cc006ee5c9b647e899f50b0dcaeee127fec713387d06a333eb404"
PKG_ARCH="any"
PKG_LICENSE="Proprietary"
PKG_SITE="https://github.com/LibreELEC/libmali"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ARM Mali libraries used in LibreELEC"
PKG_TOOLCHAIN="manual"
PKG_NEED_UNPACK="$(get_pkg_directory emuelec) $(get_build_dir emuelec)"

post_unpack() {
cp -rf $PKG_BUILD/include/* $SYSROOT_PREFIX/usr/include
}
