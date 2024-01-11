# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="vdr-plugin-xmltv2vdr"
PKG_VERSION="089976d4950c2641714be599766f0534825165ca"
PKG_SHA256="6e108a833dbd2ec3b1d812bc4e29e2ebfa1014bf21157a3168b3af759af7b4d5"
PKG_LICENSE="GPL"
PKG_SITE="http://projects.vdr-developer.org/projects/plg-xmltv2vdr"
PKG_URL="https://github.com/vdr-projects/vdr-plugin-xmltv2vdr/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain vdr sqlite openssl curl libzip libxml2 libxslt enca"
PKG_NEED_UNPACK="$(get_pkg_directory vdr)"
PKG_LONGDESC="xmltv2vdr imports data in xmltv format"
PKG_TOOLCHAIN="manual"
PKG_BUILD_FLAGS="+pic"

pre_configure_target() {
  export CXXFLAGS="${CXXFLAGS} -Wno-narrowing"
  export LIBS="-L${SYSROOT_PREFIX}/usr/lib/iconv -lssl -lcrypto -lbz2"
}

make_target() {
  VDR_DIR=$(get_build_dir vdr)
  make VDRDIR=${VDR_DIR} \
    LIBDIR="." \
    LOCALEDIR="./locale" \
    install
}

post_make_target() {
  cd dist/epgdata2xmltv
  make -j1
  cd -
  ${STRIP} dist/epgdata2xmltv/epgdata2xmltv
}
