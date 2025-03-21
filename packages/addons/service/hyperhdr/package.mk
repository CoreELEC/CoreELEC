# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="hyperhdr"
PKG_VERSION="0453be928be44e5aafa597db1037cd12822ba61f"
PKG_REV="201"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/awawa-dev/HyperHDR"
PKG_URL="https://github.com/awawa-dev/HyperHDR.git"
GET_HANDLER_SUPPORT="git"
PKG_DEPENDS_TARGET="toolchain qt-everywhere pkg-config libjpeg-turbo alsa-lib"
PKG_TOOLCHAIN="cmake"
PKG_SECTION="service"
PKG_SHORTDESC="HyperHDR: an ambient lighting controller"
PKG_LONGDESC="HyperHDR (v21.0.0.0) is an opensource ambient lighting implementation."

PKG_IS_ADDON="yes"
PKG_ADDON_NAME="HyperHDR"
PKG_ADDON_TYPE="xbmc.service"

# Setting default values
PKG_PLATFORM="-DPLATFORM=linux -DENABLE_AMLOGIC=ON"
PKG_PLATFORM="$PKG_PLATFORM -DENABLE_WS281XPWM=OFF -DENABLE_FRAMEBUFFER=OFF -DENABLE_PIPEWIRE=OFF -DENABLE_X11=OFF -DENABLE_CEC=OFF"

PKG_CMAKE_OPTS_TARGET="-DCMAKE_NO_SYSTEM_FROM_IMPORTED=ON \
                       -DCMAKE_BUILD_TYPE=Release \
                       -DUSE_STATIC_QT_PLUGINS=ON \
                       $PKG_PLATFORM \
                       -Wno-dev"

addon() {
   mkdir -p ${ADDON_BUILD}/${PKG_ADDON_ID}/{bin,lib}  
   cp -r -P -p $(get_install_dir hyperhdr)/usr/share/hyperhdr/bin/* ${ADDON_BUILD}/${PKG_ADDON_ID}/bin
   cp -r -P $(get_install_dir hyperhdr)/usr/share/hyperhdr/lib/* ${ADDON_BUILD}/${PKG_ADDON_ID}/lib
   
   patchelf --add-rpath '$ORIGIN/../lib.private' ${ADDON_BUILD}/${PKG_ADDON_ID}/bin/hyperhdr
   mkdir -p ${ADDON_BUILD}/${PKG_ADDON_ID}/lib.private
   cp -p $(get_install_dir zstd)/usr/lib/libzstd.so.1 ${ADDON_BUILD}/${PKG_ADDON_ID}/lib.private
}

