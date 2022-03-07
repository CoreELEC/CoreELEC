# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present 5schatten (https://github.com/5schatten)

PKG_NAME="SDL2"
PKG_VERSION="2.0.20"
PKG_SHA256="c56aba1d7b5b0e7e999e4a7698c70b63a3394ff9704b5f6e1c57e0c16f04dd06"
PKG_LICENSE="GPL"
PKG_SITE="https://www.libsdl.org/"
PKG_URL="https://www.libsdl.org/release/SDL2-$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain alsa-lib systemd dbus $OPENGLES pulseaudio"
PKG_LONGDESC="Simple DirectMedia Layer is a cross-platform development library designed to provide low level access to audio, keyboard, mouse, joystick, and graphics hardware."
PKG_DEPENDS_HOST="toolchain:host distutilscross:host"

if [ "${DEVICE}" = "Amlogic-ng" ] || [ "${DEVICE}" = "Amlogic" ]; then
  PKG_PATCH_DIRS="Amlogic"
fi

if [ "$DEVICE" == "OdroidGoAdvance" ]; then
  PKG_PATCH_DIRS="OdroidGoAdvance"
fi

if [ "$DEVICE" == "OdroidGoAdvance" ] || [ "$DEVICE" == "GameForce" ]; then
  PKG_DEPENDS_TARGET="$PKG_DEPENDS_TARGET libdrm mali-bifrost librga"
fi

pre_make_host() {
if [ "$DEVICE" == "OdroidGoAdvance" ] || [ "$DEVICE" == "GameForce" ]; then
	sed -i "s| -lrga||g" ${PKG_BUILD}/CMakeLists.txt
fi
}

pre_make_target() {
if [ "$DEVICE" == "OdroidGoAdvance" ] || [ "$DEVICE" == "GameForce" ]; then
# Since we removed "-lrga" from host we need to re-add it for target, hacky way of doing it but for now it works.
if ! `grep -rnw "${PKG_BUILD}/CMakeLists.txt" -e '-lrga'`; then
	sed -i "s|--no-undefined|--no-undefined -lrga|" ${PKG_BUILD}/CMakeLists.txt
fi
fi
}

PKG_CMAKE_OPTS_HOST="-DVIDEO_MALI=OFF -DVIDEO_KMSDRM=OFF"

pre_configure_target(){
  PKG_CMAKE_OPTS_TARGET="-DSDL_STATIC=OFF \
                         -DLIBC=ON \
                         -DGCC_ATOMICS=ON \
                         -DALTIVEC=OFF \
                         -DOSS=OFF \
                         -DALSA=ON \
                         -DALSA_SHARED=ON \
                         -DJACK=OFF \
                         -DJACK_SHARED=OFF \
                         -DESD=OFF \
                         -DESD_SHARED=OFF \
                         -DARTS=OFF \
                         -DARTS_SHARED=OFF \
                         -DNAS=OFF \
                         -DNAS_SHARED=OFF \
                         -DLIBSAMPLERATE=OFF \
                         -DLIBSAMPLERATE_SHARED=OFF \
                         -DSNDIO=OFF \
                         -DDISKAUDIO=OFF \
                         -DDUMMYAUDIO=OFF \
                         -DVIDEO_WAYLAND=OFF \
                         -DVIDEO_WAYLAND_QT_TOUCH=ON \
                         -DWAYLAND_SHARED=OFF \
                         -DVIDEO_MIR=OFF \
                         -DMIR_SHARED=OFF \
                         -DVIDEO_COCOA=OFF \
                         -DVIDEO_DIRECTFB=OFF \
                         -DVIDEO_VIVANTE=OFF \
                         -DDIRECTFB_SHARED=OFF \
                         -DFUSIONSOUND=OFF \
                         -DFUSIONSOUND_SHARED=OFF \
                         -DVIDEO_DUMMY=OFF \
                         -DINPUT_TSLIB=OFF \
                         -DPTHREADS=ON \
                         -DPTHREADS_SEM=ON \
                         -DDIRECTX=OFF \
                         -DSDL_DLOPEN=ON \
                         -DCLOCK_GETTIME=OFF \
                         -DRPATH=OFF \
                         -DRENDER_D3D=OFF \
                         -DVIDEO_X11=OFF \
                         -DVIDEO_OPENGLES=ON \
                         -DVIDEO_VULKAN=OFF \
                         -DPULSEAUDIO=ON"
if [ "$DEVICE" == "OdroidGoAdvance" ] || [ "$DEVICE" == "GameForce" ]; then
PKG_CMAKE_OPTS_TARGET="$PKG_CMAKE_OPTS_TARGET -DVIDEO_KMSDRM=ON"
else
PKG_CMAKE_OPTS_TARGET="$PKG_CMAKE_OPTS_TARGET -DVIDEO_MALI=ON -DVIDEO_KMSDRM=OFF"
fi
}

post_makeinstall_target() {
  sed -e "s:\(['=LI]\)/usr:\\1${SYSROOT_PREFIX}/usr:g" -i $SYSROOT_PREFIX/usr/bin/sdl2-config
  rm -rf $INSTALL/usr/bin
}
