# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="mali-bifrost"
PKG_VERSION="f226e982386287a4df669e2832d9ddd613d4153b"
PKG_SHA256="cec41b7383b64fbb9fee891c9bcfaa979cb8d8b943d131a471c7fac7e16b393e"
PKG_ARCH="arm aarch64"
PKG_LICENSE="nonfree"
PKG_SITE="https://github.com/rockchip-linux/libmali"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain libdrm"
PKG_LONGDESC="The Mali GPU library used in Rockchip Platform for Odroidgo Advance"

pre_configure_target() {

# Testing new version, Vulkan in the name means nothing, Vulkan is not working.
BLOB_PKG="rk3326_r13p0_gbm_with_vulkan_and_cl.zip"
BLOB_SUM="ef1a18fabf270d0a6029917d6b0e6237d328613c2f8be4d420ea23e022288dd9"

if [ ! -e "${SOURCES}/${PKG_NAME}/${BLOB_PKG}" ]
  then
    curl -Lo "${SOURCES}/${PKG_NAME}/${BLOB_PKG}" "https://dn.odroid.com/RK3326/ODROID-GO-Advance/${BLOB_PKG}"
  fi
  DLD_SUM=$(sha256sum "${SOURCES}/${PKG_NAME}/${BLOB_PKG}" | awk '{printf $1}')
  if [ ! "${DLD_SUM}" == "${BLOB_SUM}" ]
  then
    echo "Blob package mismatch, exiting."
    exit 1
  fi

  unzip -o "${SOURCES}/${PKG_NAME}/${BLOB_PKG}" -d ${PKG_BUILD}
}


post_makeinstall_target() {
	# remove all the extra blobs, we only need one
	rm -rf $INSTALL/usr
	
if [ "$ARCH" == "arm" ]; then
	BLOB="libmali.so_rk3326_gbm_arm32_r13p0_with_vulkan_and_cl"
	else
	BLOB="libmali.so_rk3326_gbm_arm64_r13p0_with_vulkan_and_cl"
fi
	
	mkdir -p $INSTALL/usr/lib/
	cp $PKG_BUILD/$BLOB $INSTALL/usr/lib/libmali.so

	ln -sf /usr/lib/libmali.so $INSTALL/usr/lib/libEGL.so
	ln -sf /usr/lib/libmali.so $INSTALL/usr/lib/libEGL.so.1
	ln -sf /usr/lib/libmali.so $INSTALL/usr/lib/libgbm.so
	ln -sf /usr/lib/libmali.so $INSTALL/usr/lib/libGLESv2.so
	ln -sf /usr/lib/libmali.so $INSTALL/usr/lib/libGLESv2.so.2
	ln -sf /usr/lib/libmali.so $INSTALL/usr/lib/libGLESv3.so
	ln -sf /usr/lib/libmali.so $INSTALL/usr/lib/libGLESv3.so.3
	ln -sf /usr/lib/libmali.so $INSTALL/usr/lib/libGLESv1_CM.so.1
	ln -sf /usr/lib/libmali.so $INSTALL/usr/lib/libGLES_CM.so.1

	cp -pr $PKG_BUILD/include $SYSROOT_PREFIX/usr
	cp $PKG_BUILD/include/gbm.h $SYSROOT_PREFIX/usr/include/gbm.h
	
	mkdir -p $SYSROOT_PREFIX/usr/lib
	
	cp $PKG_BUILD/$BLOB  $SYSROOT_PREFIX/usr/lib/libmali.so

	ln -sf $SYSROOT_PREFIX/usr/lib/libmali.so $SYSROOT_PREFIX/usr/lib/libEGL.so
	ln -sf $SYSROOT_PREFIX/usr/lib/libmali.so $SYSROOT_PREFIX/usr/lib/libEGL.so.1
	ln -sf $SYSROOT_PREFIX/usr/lib/libmali.so $SYSROOT_PREFIX/usr/lib/libgbm.so
	ln -sf $SYSROOT_PREFIX/usr/lib/libmali.so $SYSROOT_PREFIX/usr/lib/libGLESv2.so
	ln -sf $SYSROOT_PREFIX/usr/lib/libmali.so $SYSROOT_PREFIX/usr/lib/libGLESv2.so.2
	ln -sf $SYSROOT_PREFIX/usr/lib/libmali.so $SYSROOT_PREFIX/usr/lib/libGLESv3.so
	ln -sf $SYSROOT_PREFIX/usr/lib/libmali.so $SYSROOT_PREFIX/usr/lib/libGLESv3.so.3
	ln -sf $SYSROOT_PREFIX/usr/lib/libmali.so $SYSROOT_PREFIX/usr/lib/libGLESv1_CM.so.1
	ln -sf $SYSROOT_PREFIX/usr/lib/libmali.so $SYSROOT_PREFIX/usr/lib/libGLES_CM.so.1
}
