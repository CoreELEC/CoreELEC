################################################################################
#      This file is part of CoreELEC - http://coreelec.org
#      Copyright (C) 2018-present Team CoreELEC
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

PKG_NAME="fd628-aml"
PKG_VERSION="c002fb2"
PKG_SHA256="920af14d95d011dbae86025651d12614bf6a4761850fd3311f6facbde8592f95"
PKG_ARCH="arm aarch64"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/arthur-liberman/linux_fd628"
PKG_URL="https://github.com/arthur-liberman/linux_fd628/archive/$PKG_VERSION.tar.gz"
PKG_SOURCE_DIR="linux_fd628-$PKG_VERSION*"
PKG_DEPENDS_TARGET="toolchain linux"
PKG_NEED_UNPACK="$LINUX_DEPENDS"
PKG_SECTION="driver"
PKG_SHORTDESC="fd628-aml: Driver for Amlogic FD628 display"
PKG_LONGDESC="fd628-aml: Driver for Amlogic FD628 display"

PKG_TOOLCHAIN="manual"

pre_make_target() {
  unset LDFLAGS
}

make_target() {
  make -C "$(kernel_path)" M="$PKG_BUILD/driver"

  make FD628Service
}

makeinstall_target() {
  mkdir -p $INSTALL/$(get_full_module_dir)/$PKG_NAME
    find $PKG_BUILD/ -name \*.ko -not -path '*/\.*' -exec cp {} $INSTALL/$(get_full_module_dir)/$PKG_NAME \;

  mkdir -p $INSTALL/usr/sbin
    cp -P FD628Service $INSTALL/usr/sbin
}

post_install() {
  enable_service fd628.service
}
