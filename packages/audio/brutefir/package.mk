PKG_NAME="brutefir"
PKG_VERSION="1.0o"
PKG_SHA256="caae4a933b53b55b29d6cb7e2803e20819f31def6d0e4e12f9a48351e6dbbe9f"
PKG_LICENSE="GPL"
PKG_SITE="https://torger.se/anders/brutefir.html"
PKG_URL="https://torger.se/anders/files/brutefir-$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain alsa-lib fftw3 fftw3f"
PKG_LONGDESC="BruteFIR is a software convolution engine, a program for applying long FIR filters to multi-channel digital audio."

post_makeinstall_target() {
  mkdir -p $INSTALL/usr/config
  cp -PR $PKG_DIR/config/* $INSTALL/usr/config
}
