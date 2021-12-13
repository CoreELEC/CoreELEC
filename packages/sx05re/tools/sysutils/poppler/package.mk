# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="poppler"
PKG_VERSION="3b82fb789116c805b0a69f6bf3a72466a1e87308"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/freedesktop/poppler"
PKG_URL="$PKG_SITE.git"
PKG_DEPENDS_TARGET="toolchain zlib libpng libjpeg-turbo boost"
PKG_LONGDESC="The poppler pdf rendering library "
PKG_TOOLCHAIN="cmake"


pre_configure_target() { 
PKG_CMAKE_OPTS_TARGET="-DENABLE_LIBOPENJPEG=none \
                       -DENABLE_GLIB=ON \
                       -DENABLE_QT5=off \
                       -DENABLE_CPP=off"
                       
# Disable "gobject-introspection"
sed -i "s|set(HAVE_INTROSPECTION \${INTROSPECTION_FOUND})|set(HAVE_INTROSPECTION "NO")|g" ${PKG_BUILD}/CMakeLists.txt
}

post_makeinstall_target() {
    mkdir -p $INSTALL/usr/bin/batocera
    ln -sf /usr/bin/pdftoppm $INSTALL/usr/bin/batocera/pdftoppm
    ln -sf /usr/bin/pdfinfo $INSTALL/usr/bin/batocera/pdfinfo
}
