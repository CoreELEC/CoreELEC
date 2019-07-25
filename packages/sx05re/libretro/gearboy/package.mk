PKG_NAME="gearboy"
PKG_VERSION="8fb5bfee85db584b0aa440ab2cfd90855190afa9"
PKG_SHA256="c7900081c7fc6ccd1a8c84d7813714607d054d66ce553cddd9e536e39fd843ae"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/drhelius/Gearboy"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_PRIORITY="optional"
PKG_SECTION="libretro"
PKG_SHORTDESC="Game Boy / Gameboy Color emulator for iOS, Mac, Raspberry Pi, Windows and Linux"
PKG_LONGDESC="Game Boy / Gameboy Color emulator for iOS, Mac, Raspberry Pi, Windows and Linux"

PKG_IS_ADDON="no"
PKG_TOOLCHAIN="make"
PKG_AUTORECONF="no"

make_target() {
  make -C platforms/libretro/
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/lib/libretro
  cp platforms/libretro/gearboy_libretro.so $INSTALL/usr/lib/libretro/
}
