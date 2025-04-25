# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="gst-plugins-base"
PKG_VERSION="1.26.1"
PKG_SHA256="659553636f84dcf388cad5cf6530e02b0b2d3dc450e76199287ba9db6a6c5226"
PKG_LICENSE="GPL-2.1-or-later"
PKG_SITE="https://gstreamer.freedesktop.org/modules/gst-plugins-base.html"
PKG_URL="https://gstreamer.freedesktop.org/src/gst-plugins-base/${PKG_NAME}-${PKG_VERSION}.tar.xz"
PKG_DEPENDS_TARGET="toolchain gstreamer"
PKG_LONGDESC="Base GStreamer plugins and helper libraries"
PKG_BUILD_FLAGS="-gold"

pre_configure_target() {
  PKG_MESON_OPTS_TARGET="-Dgl=disabled \
                         -Dadder=disabled \
                         -Dapp=disabled \
                         -Daudioconvert=disabled \
                         -Daudiomixer=disabled \
                         -Daudiorate=disabled \
                         -Daudioresample=disabled \
                         -Daudiotestsrc=disabled \
                         -Dcompositor=disabled \
                         -Dencoding=disabled \
                         -Dgio=disabled \
                         -Dgio-typefinder=disabled \
                         -Doverlaycomposition=disabled \
                         -Dpbtypes=disabled \
                         -Dplayback=disabled \
                         -Drawparse=enabled \
                         -Dsubparse=enabled \
                         -Dtcp=disabled \
                         -Dtypefind=disabled \
                         -Dvideoconvertscale=disabled \
                         -Dvideorate=disabled \
                         -Dvideotestsrc=disabled \
                         -Dvolume=disabled \
                         -Dalsa=disabled \
                         -Dcdparanoia=disabled \
                         -Dlibvisual=disabled \
                         -Dogg=disabled \
                         -Dopus=disabled \
                         -Dpango=disabled \
                         -Dtheora=disabled \
                         -Dtremor=disabled \
                         -Dvorbis=disabled \
                         -Dx11=disabled \
                         -Dxshm=disabled \
                         -Dxi=disabled \
                         -Dxvideo=disabled \
                         -Dexamples=disabled \
                         -Dtests=disabled \
                         -Dtools=disabled \
                         -Dintrospection=disabled \
                         -Dnls=disabled \
                         -Dorc=disabled \
                         -Dglib_debug=disabled \
                         -Dglib_assert=false \
                         -Dglib_checks=false \
                         -Dpackage-name=gst-plugins-base \
                         -Dpackage-origin=LibreELEC.tv \
                         -Ddoc=disabled"
}

post_makeinstall_target() {
  # clean up
  safe_remove ${INSTALL}
}
