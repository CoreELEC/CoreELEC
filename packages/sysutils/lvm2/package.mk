################################################################################
#      This file is part of CoreELEC - https://coreelec.org
#      Copyright (C) 2018-present CoreELEC (team (at) coreelec.org)
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

PKG_NAME="lvm2"
PKG_VERSION="2_02_177"
PKG_SHA256="b0ab445db367cb689b01cccc2cde720f9493804efd62bf438c09de34cd5dd04f"
PKG_ARCH="any"
PKG_LICENSE="MIT"
PKG_SITE="https://www.sourceware.org/lvm2"
PKG_URL="https://github.com/lvmteam/lvm2/archive/v$PKG_VERSION.tar.gz"
PKG_SOURCE_DIR="$PKG_NAME-$PKG_VERSION"
PKG_DEPENDS_TARGET="toolchain libaio"
PKG_DEPENDS_INIT="toolchain libaio lvm2"
PKG_SECTION="system"
PKG_LONGDESC="lvm2 - userspace toolset that provide logical volume management facilities on linux"

LVM2_CONFIG_DEFAULT="ac_cv_func_malloc_0_nonnull=yes \
                     ac_cv_func_realloc_0_nonnull=yes \
                     --disable-selinux"

PKG_CONFIGURE_OPTS_INIT="$LVM2_CONFIG_DEFAULT \
                         --enable-static_link"

PKG_CONFIGURE_OPTS_TARGET="$LVM2_CONFIG_DEFAULT"

make_init() {
  make LIBS="-lpthread -lm"
}

makeinstall_init() {
  mkdir -p $INSTALL/usr/sbin
    cp -PR $PKG_BUILD/.$TARGET_NAME-init/tools/lvm.static $INSTALL/usr/sbin/lvm

  mkdir -p $INSTALL/usr/lib
    cp -PR $PKG_BUILD/.$TARGET_NAME-init/libdm/libdevmapper.so* $INSTALL/usr/lib

  mkdir -p $INSTALL/etc/lvm
    cp -PR $PKG_DIR/config/lvm.conf $INSTALL/etc/lvm/lvm.conf
}

post_makeinstall_target() {
  rm -rf $INSTALL/etc/lvm
    ln -sf /storage/.config/lvm $INSTALL/etc/lvm

  mkdir -p $INSTALL/usr/config/lvm
    cp -PR $PKG_DIR/config/lvm.conf $INSTALL/usr/config/lvm
}
