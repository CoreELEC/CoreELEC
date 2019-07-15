# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present 5schatten (https://github.com/5schatten)

PKG_NAME="GLideN64"
PKG_VERSION="abf7a7ae63c23cbcfef105ca4cfa42f5904aafb2"
PKG_SHA256=""
PKG_LICENSE="GPLv2"
PKG_SITE="https://github.com/gonetz/GLideN64"
PKG_URL="https://github.com/gonetz/GLideN64/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain linux glibc freetype:host zlib bzip2 libpng16 ${OPENGLES}"
PKG_LONGDESC="A new generation, open-source graphics plugin for N64 emulators."
PKG_TOOLCHAIN="cmake"
 
pre_configure_target() {
  PKG_CMAKE_SCRIPT="$PKG_BUILD/src/CMakeLists.txt"
  PKG_CMAKE_OPTS_TARGET="-DEGL=On -DMUPENPLUSAPI=On -DVEC4_OPT=On -DNEON_OPT=On -DCRC_ARMV8=On -DODROID=On"
  
  # Fix revision header
  PKG_REV_H=$PKG_BUILD/src/Revision.h
  echo "#define PLUGIN_REVISION" ""\"${PKG_VERSION:0:10}""\"     > ${PKG_REV_H}
  echo "#define PLUGIN_REVISION_W" "L"\"${PKG_VERSION:0:10}""\" >> ${PKG_REV_H}

    #rm -rf $PKG_BUILD/src/GLideNHQ/inc
    sed -i -e "s|/opt/vc/|$SYSROOT_PREFIX/|g" $PKG_CMAKE_SCRIPT
     }

makeinstall_target() {
 :
}
