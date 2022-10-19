# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2022-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="augustus"
PKG_VERSION="8c0adb21847155e6b5eb42717b87069e918fae65"
PKG_SHA256="88981b3e8da2b7a3f5e4775c27344d1d2104aa18fbc8b404a4289e44919f1eb9"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPLv3"
PKG_SITE="https://github.com/Keriew/augustus"
PKG_URL="$PKG_SITE/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain SDL2"
PKG_LONGDESC="An open source re-implementation of Caesar III"
PKG_TOOLCHAIN="cmake-make"

pre_configure_target() {
# Just setting the version
sed -i "s|unknown development version|Version: ${PKG_VERSION:0:7} - ${DISTRO}|g" ${PKG_BUILD}/CMakeLists.txt
}
