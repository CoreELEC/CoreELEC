################################################################################
#      This file is part of CoreELEC (http://coreelec.org).
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

PKG_NAME="qt-everywhere"
PKG_VERSION="5.6.3"
PKG_SHA256="2fa0cf2e5e8841b29a4be62062c1a65c4f6f2cf1beaf61a5fd661f520cd776d0"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="http://qt-project.org"
PKG_URL="http://download.qt.io/official_releases/qt/5.6/$PKG_VERSION/single/$PKG_NAME-opensource-src-$PKG_VERSION.tar.xz"
PKG_DEPENDS_TARGET="pcre zlib"
PKG_SOURCE_DIR="$PKG_NAME-opensource-src-$PKG_VERSION"
PKG_LONGDESC="A cross-platform application and UI framework"

PKG_CONFIGURE_OPTS_TARGET="-prefix /usr
                           -sysroot $SYSROOT_PREFIX
                           -hostprefix $TOOLCHAIN
                           -device linux-libreelec-g++
                           -opensource -confirm-license
                           -release
                           -static
                           -make libs
                           -force-pkg-config
                           -no-accessibility
                           -no-sql-sqlite
                           -no-sql-mysql
                           -no-qml-debug
                           -system-zlib
                           -no-mtdev
                           -no-gif
                           -no-libjpeg
                           -no-harfbuzz
                           -no-libproxy
                           -system-pcre
                           -no-glib
                           -silent
                           -no-cups
                           -no-iconv
                           -no-evdev
                           -no-tslib
                           -no-icu
                           -no-strip
                           -no-fontconfig
                           -no-dbus
                           -no-opengl
                           -no-libudev
                           -no-libinput
                           -no-eglfs
                           -skip qt3d
                           -skip qtactiveqt
                           -skip qtandroidextras
                           -skip qtcanvas3d
                           -skip qtconnectivity
                           -skip qtdeclarative
                           -skip qtdoc
                           -skip qtgraphicaleffects
                           -skip qtimageformats
                           -skip qtlocation
                           -skip qtmacextras
                           -skip qtmultimedia
                           -skip qtquickcontrols
                           -skip qtquickcontrols2
                           -skip qtscript
                           -skip qtsensors
                           -skip qtserialbus
                           -skip qtsvg
                           -skip qttranslations
                           -skip qtwayland
                           -skip qtwebchannel
                           -skip qtwebengine
                           -skip qtwebsockets
                           -skip qtwebview
                           -skip qtwinextras
                           -skip qtx11extras
                           -skip qtxmlpatterns"

configure_target() {
  QMAKE_CONF_DIR="qtbase/mkspecs/devices/linux-libreelec-g++"
  QMAKE_CONF="${QMAKE_CONF_DIR}/qmake.conf"
  
  cd ..
  mkdir -p $QMAKE_CONF_DIR
  cat > $QMAKE_CONF <<EOF
    MAKEFILE_GENERATOR       = UNIX
    CONFIG                  += incremental
    QMAKE_INCREMENTAL_STYLE  = sublib
    include(../../common/linux.conf)
    include(../../common/gcc-base-unix.conf)
    include(../../common/g++-unix.conf)
    load(device_config)
    QMAKE_CC                = $CC
    QMAKE_CXX               = $CXX
    QMAKE_LINK              = $CXX
    QMAKE_LINK_SHLIB        = $CXX
    QMAKE_AR                = $AR cqs
    QMAKE_OBJCOPY           = $OBJCOPY
    QMAKE_NM                = $NM -P
    QMAKE_STRIP             = $STRIP
    QMAKE_CFLAGS = $CFLAGS
    QMAKE_CXXFLAGS = $CXXFLAGS
    QMAKE_LFLAGS = $LDFLAGS
    load(qt_config)
EOF
    
  echo '#include "../../linux-g++/qplatformdefs.h"' >> $QMAKE_CONF_DIR/qplatformdefs.h
  
  unset CC CXX LD RANLIB AR AS CPPFLAGS CFLAGS LDFLAGS CXXFLAGS
  ./configure $PKG_CONFIGURE_OPTS_TARGET
}