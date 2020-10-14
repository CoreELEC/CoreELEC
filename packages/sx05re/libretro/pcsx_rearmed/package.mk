# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020 Trond Haugland (trondah@gmail.com)

PKG_NAME="pcsx_rearmed"
PKG_VERSION="19b9695a71f15ef0bf61c7c3cfd6c98ec5ccb028"
PKG_SHA256="2a5722b82870c8688f5819e7f4db3bedae394c5b79c2165c9d2abc3c9bfff1ab"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/libretro/pcsx_rearmed"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_SHORTDESC="ARM optimized PCSX fork"
PKG_TOOLCHAIN="manual"
PKG_BUILD_FLAGS="+speed -gold"

makeinstall_target() {
  cd ${PKG_BUILD}
  if [ "${ARCH}" != "aarch64" ]; then
    make -f Makefile.libretro GIT_VERSION=${PKG_VERSION} platform=rpi3
  fi

# Thanks to escalade for the multilib solution! https://forum.odroid.com/viewtopic.php?f=193&t=39281

VERSION=${LIBREELEC_VERSION}
INSTALLTO="/usr/lib/libretro/"
PROJECT_ALT=${PROJECT}

if [ "$DEVICE" == "OdroidGoAdvance" ]; then
PROJECT_ALT=${DEVICE}
fi

mkdir -p ${INSTALL}${INSTALLTO}

if [ "${ARCH}" = "aarch64" ]; then
    mkdir -p ${INSTALL}/usr/bin
    mkdir -p ${INSTALL}/usr/config/emuelec/lib32
    LIBS="ld-2.*.so \
		libarmmem-v7l.* \
		librt.so* \
		librt-*.so \
		libass.so* \
		libasound.so* \
		libopenal.so* \
		libpulse.so* \
		libpulseco*.so* \
		libfreetype.so* \
		libpthread*.so* \
		libudev.so* \
		libusb-1.0.so* \
		libSDL2-2.0.so* \
		libavcodec.so* \
		libavformat.so* \
		libavutil.so.56* \
		libswscale.so.5* \
		libswresample.so.3* \
		libstdc++.so.6* \
		libm.so* \
		libm-2.*.so \
		libgcc_s.so* \
		libc.so* \
		libc-*.so \
		ld-linux-armhf.so* \
		libfontconfig.so* \
		libexpat.so* \
		libbz2.so* \
		libz.so* \
		libpulsecommon-12* \
		libdbus-1.so* \
		libdav1d.so* \
		libspeex.so* \
		libssl.so* \
		libcrypt*.so* \
		libsystemd.so* \
		libdl.so.2 \
		libdl-*.so \
		libMali.*.so"
	if [ "$PROJECT" == "Amlogic" ]; then
		LIBS+=" libMali.so"
	fi
	if [ "$DEVICE" == "OdroidGoAdvance" ]; then
		LIBS+=" libdrm.so* \
		librga.so \
		libpng*.so.* \
		librockchip_mpp.so* \
		libxkbcommon.so* \
		libmali.so"
	fi
    for lib in ${LIBS}
    do 
      find $PKG_BUILD/../../build.${DISTRO}-${PROJECT_ALT}.arm-${VERSION}/*/.install_pkg -name ${lib} -exec cp -vP \{} ${INSTALL}/usr/config/emuelec/lib32 \;
    done
    
    if [ "$DEVICE" == "OdroidGoAdvance" ]; then
	ln -sf libmali.so $INSTALL/usr/config/emuelec/lib32/libMali.so
    ln -sf libMali.so $INSTALL/usr/config/emuelec/lib32/libgbm.so
    fi

    ln -sf libMali.so $INSTALL/usr/config/emuelec/lib32/libMali.so.0
    ln -sf libMali.so $INSTALL/usr/config/emuelec/lib32/libEGL.so
    ln -sf libMali.so $INSTALL/usr/config/emuelec/lib32/libEGL.so.1
    ln -sf libMali.so $INSTALL/usr/config/emuelec/lib32/libEGL.so.1.0.0
    ln -sf libMali.so $INSTALL/usr/config/emuelec/lib32/libGLES_CM.so.1
    ln -sf libMali.so $INSTALL/usr/config/emuelec/lib32/libGLESv1_CM.so
    ln -sf libMali.so $INSTALL/usr/config/emuelec/lib32/libGLESv1_CM.so.1
    ln -sf libMali.so $INSTALL/usr/config/emuelec/lib32/libGLESv1_CM.so.1.0.1
    ln -sf libMali.so $INSTALL/usr/config/emuelec/lib32/libGLESv1_CM.so.1.1
    ln -sf libMali.so $INSTALL/usr/config/emuelec/lib32/libGLESv2.so
    ln -sf libMali.so $INSTALL/usr/config/emuelec/lib32/libGLESv2.so.2
    ln -sf libMali.so $INSTALL/usr/config/emuelec/lib32/libGLESv2.so.2.0
    ln -sf libMali.so $INSTALL/usr/config/emuelec/lib32/libGLESv2.so.2.0.0
    ln -sf libMali.so $INSTALL/usr/config/emuelec/lib32/libGLESv3.so
    ln -sf libMali.so $INSTALL/usr/config/emuelec/lib32/libGLESv3.so.3
    ln -sf libMali.so $INSTALL/usr/config/emuelec/lib32/libGLESv3.so.3.0
    ln -sf libMali.so $INSTALL/usr/config/emuelec/lib32/libGLESv3.so.3.0.0
    
    cp -vP $PKG_BUILD/../../build.${DISTRO}-${PROJECT_ALT}.arm-${VERSION}/retroarch-*/.install_pkg/usr/bin/retroarch ${INSTALL}/usr/bin/retroarch32
    patchelf --set-interpreter /emuelec/lib32/ld-linux-armhf.so.3 ${INSTALL}/usr/bin/retroarch32
    cp -vP $PKG_BUILD/../../build.${DISTRO}-${PROJECT_ALT}.arm-${VERSION}/pcsx_rearmed-*/.install_pkg/usr/lib/libretro/pcsx_rearmed_libretro.so ${INSTALL}${INSTALLTO}
    chmod -f +x ${INSTALL}/usr/config/emuelec/lib32/* || :
else
    cp pcsx_rearmed_libretro.so ${INSTALL}${INSTALLTO}
fi
}
