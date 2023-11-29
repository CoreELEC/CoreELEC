# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="mpv-drmprime"
PKG_VERSION="0.37.0"
PKG_SHA256="1d2d4adbaf048a2fa6ee134575032c4b2dad9a7efafd5b3e69b88db935afaddf"
PKG_LICENSE="GPL"
PKG_SITE="https://mpv.io/"
PKG_URL="https://github.com/mpv-player/mpv/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain alsa ffmpeg libass libdrm libplacebo lua52"
PKG_LONGDESC="A media player based on MPlayer and mplayer2. It supports a wide variety of video file formats, audio and video codecs, and subtitle types."
PKG_BUILD_FLAGS="-sysroot"

PKG_MESON_OPTS_TARGET="--prefix=/usr \
                        -Dlibarchive=disabled \
                        -Dlua=enabled \
                        -Djavascript=disabled \
                        -Duchardet=disabled \
                        -Drubberband=disabled \
                        -Dlcms2=disabled \
                        -Dvapoursynth=disabled \
                        -Djack=disabled \
                        -Dwayland=disabled \
                        -Dx11=disabled \
                        -Dvulkan=disabled \
                        -Dcaca=disabled \
                        -Ddrm=enabled \
                        -Dgbm=enabled \
                        -Degl-drm=enabled \
                        -Dmanpage-build=disabled"

if [ "${OPENGLES_SUPPORT}" = "yes" ]; then
  PKG_DEPENDS_TARGET+=" ${OPENGLES}"
fi

if [ "${OPENGL_SUPPORT}" = "yes" ]; then
  PKG_DEPENDS_TARGET+=" ${OPENGL}"
fi

if [ "${VAAPI_SUPPORT}" = "yes" ]; then
  PKG_DEPENDS_TARGET+=" libva"
  PKG_MESON_OPTS_TARGET+=" -Dvaapi=enabled -Dvaapi-drm=enabled"
else
  PKG_MESON_OPTS_TARGET+=" -Dvaapi=disabled"
fi

if [ "${PULSEAUDIO_SUPPORT}" = "yes" ]; then
  PKG_DEPENDS_TARGET+=" pulseaudio"
  PKG_MESON_OPTS_TARGET+=" -Dpulse=enabled"
else
  PKG_MESON_OPTS_TARGET+=" -Dpulse=disabled"
fi

if [ "${KODI_BLURAY_SUPPORT}" = "yes" ]; then
  PKG_DEPENDS_TARGET+=" libbluray"
  PKG_MESON_OPTS_TARGET+=" -Dlibbluray=enabled"
else
  PKG_MESON_OPTS_TARGET+=" -Dlibbluray=disabled"
fi

pre_configure_target() {
  export PKG_CONFIG_PATH="$(get_install_dir libplacebo)/usr/lib/pkgconfig:${PKG_CONFIG_PATH}"
}
