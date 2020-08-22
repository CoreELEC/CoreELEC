# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="emuelec-ports"
PKG_VERSION=""
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPLv3"
PKG_SITE=""
PKG_URL=""
PKG_DEPENDS_TARGET="toolchain commander-genius devilutionX sdlpop VVVVVV opentyrian eduke"
PKG_SECTION="emuelec"
PKG_SHORTDESC="EmuELEC Ports Meta Package"
PKG_TOOLCHAIN="manual"

# Not Enabled yet: bermuda hodesdl hydracastlelabyrinth

#make_target() {
## builder commands go here

#}

makeinstall_target() {
mkdir -p $INSTALL/usr/config/emuelec/ports
python3 $PKG_DIR/ports_builder.py $PKG_DIR/ports.yaml $PKG_DIR/scripts/
cp -r $PKG_DIR/scripts/* $INSTALL/usr/config/emuelec/ports/
}
