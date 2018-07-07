################################################################################
#      This file is part of CoreELEC - https://coreelec.org
#      Copyright (C) 2018-present CoreELEC (team (at) coreelec.org)
#      Copyright (C) 2018 Arthur Liberman (arthur_liberman (at) hotmail.com)
#
#  CoreELEC is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 2 of the License, or
#  (at your option) any later version.
#
#  CoreELEC is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with CoreELEC.  If not, see <http://www.gnu.org/licenses/>.
################################################################################

PKG_NAME="openvfd"
PKG_VERSION="2168470"
PKG_SHA256="d5e1d4fd4795f6036e86850827b9bc024ba9767d7ef3f14fa5e90f9fd0bfe0f2"
PKG_ARCH="arm aarch64"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/arthur-liberman/linux_openvfd"
PKG_URL="https://github.com/arthur-liberman/linux_openvfd/archive/$PKG_VERSION.tar.gz"
PKG_SOURCE_DIR="linux_openvfd-$PKG_VERSION*"
PKG_DEPENDS_TARGET="toolchain linux"
PKG_NEED_UNPACK="$LINUX_DEPENDS"
PKG_SECTION="driver"
PKG_SHORTDESC="openvfd: Driver for VFD displays"
PKG_LONGDESC="openvfd: Driver for VFD displays"

PKG_TOOLCHAIN="manual"

pre_make_target() {
  unset LDFLAGS
}

make_target() {
  kernel_make -C "$(kernel_path)" M="$PKG_BUILD/driver"

  make OpenVFDService
}

makeinstall_target() {
  mkdir -p $INSTALL/$(get_full_module_dir)/$PKG_NAME
    find $PKG_BUILD/ -name \*.ko -not -path '*/\.*' -exec cp {} $INSTALL/$(get_full_module_dir)/$PKG_NAME \;

  mkdir -p $INSTALL/usr/sbin
    cp -P OpenVFDService $INSTALL/usr/sbin
}

post_install() {
  enable_service openvfd.service
}
