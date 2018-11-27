PKG_NAME="acelist"
PKG_VERSION="latest"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_DEPENDS_TARGET="toolchain"
PKG_SECTION="third/tools"
PKG_SHORTDESC="NoxBit: Create playlist from pomoyka.win"
PKG_LONGDESC="NoxBit: Create playlist from pomoyka.win"
PKG_TOOLCHAIN="manual"

make_target() {
  : # nothing to make here
}

makeinstall_target() {
  mkdir -p $INSTALL/usr/bin
    cp $PKG_DIR/scripts/* $INSTALL/usr/bin
  mkdir -p $INSTALL/usr/config/acelist
#    cp noxbit.cfg $INSTALL/usr/config/acelist
    cp $PKG_DIR/config/* $INSTALL/usr/config/acelist
#  mkdir -p $INSTALL/usr/config/noxbit/bin
#    cp STM-* $INSTALL/usr/config/noxbit/bin
#  ln -sf /storage/.config/noxbit/bin/STM-Agent $INSTALL/usr/bin/STM-Agent
#  ln -sf /storage/.config/noxbit/bin/STM-Downloader $INSTALL/usr/bin/STM-Downloader
#  ln -sf /storage/.config/noxbit/bin/STM-Hypervisor $INSTALL/usr/bin/STM-Hypervisor
}

post_install() {
    enable_service acelist.timer
}
