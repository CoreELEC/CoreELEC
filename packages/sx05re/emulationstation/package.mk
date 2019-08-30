# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="emulationstation"
PKG_VERSION="fb6a0abbec51229374d94ce27a4c014b498d04d9"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/shantigilbert/EmulationStation"
PKG_URL="$PKG_SITE.git"
PKG_DEPENDS_TARGET="toolchain SDL2-git freetype curl freeimage vlc bash rapidjson ${OPENGLES}"
PKG_SECTION="emuelec"
PKG_NEED_UNPACK="busybox"
PKG_SHORTDESC="Emulationstation emulator frontend with EmuELEC changes"
PKG_BUILD_FLAGS="-gold"
GET_HANDLER_SUPPORT="git"
PKG_GIT_CLONE_BRANCH="EmuELEC"

# themes for Emulationstation
PKG_DEPENDS_TARGET="$PKG_DEPENDS_TARGET es-theme-ComicBook"

pre_patch() { 
if [ -f "$PKG_DIR/patches/emulationstation-999-addon-options.patch" ]; then
mv $PKG_DIR/patches/emulationstation-999-addon-options.patch $PKG_DIR/patches/emulationstation-999-addon-options.patch.addon
fi 

if [[ ${EMUELEC_ADDON} ]]; then
mv $PKG_DIR/patches/emulationstation-999-addon-options.patch.addon $PKG_DIR/patches/emulationstation-999-addon-options.patch
fi
}

post_patch() {
if [ ${EMUELEC_ADDON} = "Yes" ]; then
mv $PKG_DIR/patches/emulationstation-999-addon-options.patch $PKG_DIR/patches/emulationstation-999-addon-options.patch.addon
fi
}

post_makeinstall_target() {

	mkdir -p $INSTALL/usr/config/emulationstation/resources
	cp -rf $PKG_BUILD/resources/* $INSTALL/usr/config/emulationstation/resources/
	ln -sf /storage/.config/emulationstation/resources $INSTALL/usr/bin/resources

	mkdir -p $INSTALL/etc/emulationstation/
	ln -sf /storage/.config/emulationstation/themes $INSTALL/etc/emulationstation/
    
	mkdir -p $INSTALL/usr/config/emulationstation
	cp -rf $PKG_DIR/config/scripts $INSTALL/usr/config/emulationstation
	cp -rf $PKG_DIR/config/*.cfg $INSTALL/usr/config/emulationstation
	cp -rf $PKG_DIR/config/es_systems.cfg.${PROJECT} $INSTALL/usr/config/emulationstation/es_systems.cfg   
	
	chmod +x $INSTALL/usr/config/emulationstation/scripts/*
	chmod +x $INSTALL/usr/config/emulationstation/scripts/configscripts/*
	find $INSTALL/usr/config/emulationstation/scripts/ -type f -exec chmod o+x {} \; 

	if [ ${PROJECT} = "Amlogic-ng" ]; then    
	sed -i "s|-r 32000 -Z|-Z|" $INSTALL/usr/config/emulationstation/scripts/bgm.sh
	sed -i "s|Libretro_mba_mini|Libretro_mba_mini,Libretro_mame2016|" $INSTALL/usr/config/emulationstation/scripts/getcores.sh
	sed -i "s|Libretro_snes9x2005_plus|Libretro_snes9x2005_plus,Libretro_mesen-s|" $INSTALL/usr/config/emulationstation/scripts/getcores.sh
	fi
}

post_install() {  
  enable_service emustation.service
}
