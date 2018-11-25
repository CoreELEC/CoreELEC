# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="tvheadend42"
PKG_VERSION="db177a6047769caff02fbf65de7eef00a930a027"
PKG_SHA256="2d301e54e45dc054181b837a80382d9dec9a659b996aa7411b31d16063046022"
PKG_VERSION_NUMBER="4.2.6-62"
PKG_REV="116"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="http://www.tvheadend.org"
PKG_URL="https://github.com/tvheadend/tvheadend/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain avahi comskip curl dvb-apps ffmpegx libdvbcsa libhdhomerun \
                    libiconv openssl pngquant:host Python2:host tvh-dtv-scan-tables"
PKG_SECTION="service"
PKG_SHORTDESC="Tvheadend: a TV streaming server for Linux"
PKG_LONGDESC="Tvheadend ($PKG_VERSION_NUMBER): is a TV streaming server for Linux supporting DVB-S/S2, DVB-C, DVB-T/T2, IPTV, SAT>IP, ATSC and ISDB-T"

PKG_IS_ADDON="yes"
PKG_ADDON_NAME="Tvheadend Server 4.2"
PKG_ADDON_TYPE="xbmc.service"

# basic transcoding options
PKG_TVH_TRANSCODING="\
  --disable-ffmpeg_static \
  --disable-libfdkaac_static \
  --disable-libopus_static \
  --disable-libtheora \
  --disable-libtheora_static \
  --disable-libvorbis_static \
  --disable-libvpx_static \
  --disable-libx264_static \
  --disable-libx265_static \
  --enable-libav \
  --enable-libfdkaac \
  --enable-libopus \
  --enable-libvorbis \
  --enable-libvpx \
  --enable-libx264 \
  --enable-libx265"

# specific transcoding options
if [[ "$TARGET_ARCH" != "x86_64" ]]; then
  PKG_TVH_TRANSCODING="$PKG_TVH_TRANSCODING \
    --disable-libvpx \
    --disable-libx265"
fi

PKG_CONFIGURE_OPTS_TARGET="--prefix=/usr \
                           --arch=$TARGET_ARCH \
                           --cpu=$TARGET_CPU \
                           --cc=$CC \
                           $PKG_TVH_TRANSCODING \
                           --enable-avahi \
                           --enable-bundle \
                           --disable-dbus_1 \
                           --enable-dvbcsa \
                           --enable-dvben50221 \
                           --disable-dvbscan \
                           --enable-hdhomerun_client \
                           --disable-hdhomerun_static \
                           --enable-epoll \
                           --enable-inotify \
                           --enable-pngquant \
                           --disable-libmfx_static \
                           --disable-nvenc \
                           --disable-uriparser \
                           --enable-tvhcsa \
                           --enable-trace \
                           --nowerror \
                           --disable-bintray_cache \
                           --python=$TOOLCHAIN/bin/python"

post_unpack() {
  sed -e 's/VER="0.0.0~unknown"/VER="'$PKG_VERSION_NUMBER' ~ CoreELEC Tvh-addon v'$ADDON_VERSION'.'$PKG_REV'"/g' -i $PKG_BUILD/support/version
  sed -e 's|'/usr/bin/pngquant'|'$TOOLCHAIN/bin/pngquant'|g' -i $PKG_BUILD/support/mkbundle
}

pre_configure_target() {
# fails to build in subdirs
  cd $PKG_BUILD
  rm -rf .$TARGET_NAME

# pass ffmpegx to build
  PKG_CONFIG_PATH="$(get_build_dir ffmpegx)/.INSTALL_PKG/usr/local/lib/pkgconfig"
  CFLAGS="$CFLAGS -I$(get_build_dir ffmpegx)/.INSTALL_PKG/usr/local/include"
  LDFLAGS="$LDFLAGS -L$(get_build_dir ffmpegx)/.INSTALL_PKG/usr/local/lib"

# pass libhdhomerun to build
  CFLAGS="$CFLAGS -I$(get_build_dir libhdhomerun)"

  export CROSS_COMPILE="$TARGET_PREFIX"
  export CFLAGS="$CFLAGS -I$SYSROOT_PREFIX/usr/include/iconv -L$SYSROOT_PREFIX/usr/lib/iconv"
}

post_make_target() {
  $CC -O -fbuiltin -fomit-frame-pointer -fPIC -shared -o capmt_ca.so src/extra/capmt_ca.c -ldl
}

makeinstall_target() {
  :
}

addon() {
  mkdir -p $ADDON_BUILD/$PKG_ADDON_ID/bin

  cp $PKG_DIR/addon.xml $ADDON_BUILD/$PKG_ADDON_ID

  # set only version (revision will be added by buildsystem)
  sed -e "s|@ADDON_VERSION@|$ADDON_VERSION|g" \
      -i $ADDON_BUILD/$PKG_ADDON_ID/addon.xml

  cp -P $PKG_BUILD/build.linux/tvheadend $ADDON_BUILD/$PKG_ADDON_ID/bin
  cp -P $PKG_BUILD/capmt_ca.so $ADDON_BUILD/$PKG_ADDON_ID/bin
  cp -P $(get_build_dir comskip)/.install_pkg/usr/bin/comskip $ADDON_BUILD/$PKG_ADDON_ID/bin

  #dvb-scan files
  mkdir -p $ADDON_BUILD/$PKG_ADDON_ID/dvb-scan
  cp -r $(get_build_dir tvh-dtv-scan-tables)/atsc \
        $(get_build_dir tvh-dtv-scan-tables)/dvb-* \
        $(get_build_dir tvh-dtv-scan-tables)/isdb-t \
        $ADDON_BUILD/$PKG_ADDON_ID/dvb-scan
}
