# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="vlc"
PKG_VERSION="3.0.8"
PKG_SHA256="e0149ef4a20a19b9ecd87309c2d27787ee3f47dfd47c6639644bc1f6fd95bdf6"
PKG_REV="20190822"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="http://www.videolan.org"
PKG_URL="https://download.videolan.org/pub/videolan/$PKG_NAME/$PKG_VERSION/$PKG_NAME-$PKG_VERSION.tar.xz"
PKG_DEPENDS_TARGET="toolchain libdvbpsi gnutls ffmpeg libmpeg2 zlib flac libvorbis libxml2 pulseaudio mpg123-compat"
PKG_SHORTDESC="VideoLAN multimedia player and streamer"
PKG_LONGDESC="VLC is the VideoLAN project's media player. It plays MPEG, MPEG2, MPEG4, DivX, MOV, WMV, QuickTime, mp3, Ogg/Vorbis files, DVDs, VCDs, and multimedia streams from various network sources."
PKG_AUTORECONF="yes"
PKG_TOOLCHAIN="configure"

PKG_CONFIGURE_OPTS_TARGET="--enable-silent-rules \
            --disable-dependency-tracking \
            --without-contrib \
            --disable-nls \
            --disable-rpath \
            --disable-dbus \
            --disable-gprof \
            --disable-cprof \
            --disable-debug \
            --enable-run-as-root \
            --disable-coverage \
            --enable-sout \
            --disable-lua \
            --enable-httpd \
            --enable-vlm \
            --disable-growl \
            --disable-notify \
            --disable-taglib \
            --disable-live555 \
            --disable-dc1394 \
            --disable-dvdread \
            --disable-dvdnav \
            --disable-opencv \
            --disable-decklink \
            --disable-sftp \
            --enable-v4l2 \
            --disable-gnomevfs \
            --disable-vcdx \
            --disable-vcd \
            --disable-libcddb \
            --disable-dvbpsi \
            --disable-screen \
            --disable-ogg \
            --enable-mux_ogg \
            --disable-shout\
            --disable-mkv \
            --disable-mod \
            --enable-mpc \
            --disable-gme \
            --disable-wma-fixed \
            --disable-shine \
            --disable-omxil \
            --disable-mad \
            --disable-merge-ffmpeg \
            --enable-avcodec \
            --enable-avformat \
            --enable-swscale \
            --enable-postproc \
            --disable-faad \
            --disable-flac \
            --enable-aa \
            --disable-twolame \
            --disable-quicktime \
            --disable-realrtsp \
            --disable-libtar \
            --disable-a52 \
            --disable-dca \
            --enable-libmpeg2 \
            --disable-vorbis \
            --disable-tremor \
            --disable-speex \
            --disable-theora \
            --disable-schroedinger \
            --disable-png \
            --disable-x264 \
            --disable-fluidsynth \
            --disable-zvbi \
            --disable-telx \
            --disable-libass \
            --disable-kate \
            --disable-tiger \
            --disable-libva \
            --disable-vdpau \
            --without-x \
            --disable-xcb \
            --disable-xvideo \
            --disable-sdl \
            --disable-sdl-image \
            --disable-freetype \
            --disable-fribidi \
            --disable-fontconfig \
            --enable-libxml2 \
            --disable-svg \
            --disable-directx \
            --disable-directfb \
            --disable-caca \
            --disable-oss \
            --enable-pulse \
            --enable-alsa \
            --disable-jack \
            --disable-upnp \
            --disable-skins2 \
            --disable-kai \
            --disable-macosx \
            --disable-macosx-dialog-provider \
            --disable-macosx-eyetv \
            --disable-macosx-vlc-app \
            --disable-macosx-qtkit \
            --disable-macosx-quartztext \
            --disable-ncurses \
            --disable-goom \
            --disable-projectm \
            --disable-atmo \
            --disable-bonjour \
            --enable-udev \
            --disable-mtp \
            --disable-lirc \
            --disable-libgcrypt \
            --disable-update-check \
            --disable-kva \
            --disable-bluray \
            --disable-samplerate \
            --disable-sid \
            --disable-crystalhd \
            --disable-dxva2 \
            --disable-dav1d \
            --enable-vlc \
            --disable-qt \
            --enable-neon"

pre_configure_target() {
  export LDFLAGS="$LDFLAGS -lresolv -fopenmp"
}

post_makeinstall_target() {
  rm -fr $INSTALL/usr/share/applications
  rm -fr $INSTALL/usr/share/icons
  rm -fr $INSTALL/usr/share/kde4
  rm -f $INSTALL/usr/bin/rvlc
  rm -f $INSTALL/usr/bin/vlc-wrapper

  mkdir -p $INSTALL/usr/config
    mv -f $INSTALL/usr/lib/vlc $INSTALL/usr/config
    ln -sf /storage/.config/vlc $INSTALL/usr/lib/vlc
}
