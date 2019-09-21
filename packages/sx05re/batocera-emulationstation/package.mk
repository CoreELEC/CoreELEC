# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="batocera-emulationstation"
PKG_VERSION="d46db3922f0abe36601718c3df47436ba8856640"
PKG_GIT_CLONE_BRANCH="EmuELEC"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/shantigilbert/batocera-emulationstation"
PKG_URL="$PKG_SITE.git"
PKG_DEPENDS_TARGET="toolchain SDL2-git freetype curl freeimage vlc bash rapidjson ${OPENGLES} SDL2_mixer boost_locale fping pyyaml"
PKG_SECTION="emuelec"
PKG_NEED_UNPACK="busybox"
PKG_SHORTDESC="Emulationstation emulator frontend"
PKG_BUILD_FLAGS="-gold"
GET_HANDLER_SUPPORT="git"

if [ ${PROJECT} = "Amlogic-ng" ]; then
  PKG_PATCH_DIRS="${PROJECT}"
fi

# themes for Emulationstation
PKG_DEPENDS_TARGET="$PKG_DEPENDS_TARGET es-theme-ComicBook"


makeinstall_target() {
	mkdir -p $INSTALL/usr/share/locale
	cp -rf $PKG_BUILD/locale/lang/* $INSTALL/usr/share/locale
	
	mkdir -p $INSTALL/usr/config/emulationstation/resources
    cp -rf $PKG_BUILD/resources/* $INSTALL/usr/config/emulationstation/resources/
    
	mkdir -p $INSTALL/usr/lib/python2.7
	cp -rf $PKG_DIR/bluez/* $INSTALL/usr/lib/python2.7
	
	mkdir -p $INSTALL/usr/config/emuelec/
	cp -rf $PKG_DIR/emuelec/* $INSTALL/usr/config/emuelec
	chmod +x $INSTALL/usr/config/emuelec/scripts/batocera/*
	
	mkdir -p $INSTALL/usr/lib/python2.7/site-packages/
	ln -sf /storage/.config/emuelec/lib/python2.7/site-packages/configgen $INSTALL/usr/lib/python2.7/site-packages/configgen
        
    mkdir -p $INSTALL/usr/bin
    ln -sf /storage/.config/emulationstation/resources $INSTALL/usr/bin/resources
    cp -rf $PKG_BUILD/emulationstation $INSTALL/usr/bin

	mkdir -p $INSTALL/etc/emulationstation/
	ln -sf /storage/.config/emulationstation/themes $INSTALL/etc/emulationstation/
   
	mkdir -p $INSTALL/usr/config/emulationstation
	cp -rf $PKG_DIR/config/* $INSTALL/usr/config/emulationstation
	chmod +x $INSTALL/usr/config/emulationstation/scripts/*
	chmod +x $INSTALL/usr/config/emulationstation/scripts/configscripts/*
	find $INSTALL/usr/config/emulationstation/scripts/ -type f -exec chmod o+x {} \; 
	
	if [ ${PROJECT} = "Amlogic-ng" ]; then    
	sed -i "s|-r 32000 -Z|-Z|" $INSTALL/usr/config/emulationstation/scripts/bgm.sh
	fi
}

post_install() {  
  enable_service emustation.service
}
