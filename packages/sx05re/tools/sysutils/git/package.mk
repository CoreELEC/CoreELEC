# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="git"
PKG_VERSION="2.21.0"
PKG_SHA256="85eca51c7404da75e353eba587f87fea9481ba41e162206a6f70ad8118147bee"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPLv2"
PKG_SITE="https://git-scm.com/"
PKG_URL="https://mirrors.edge.kernel.org/pub/software/scm/git/git-$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain openssl pcre curl libiconv zlib"
PKG_SECTION="emuelec"
PKG_SHORTDESC="Git is a free and open source distributed version control system designed to handle everything from small to very large projects with speed and efficiency. "
PKG_LONGDESC="Git is a free and open source distributed version control system designed to handle everything from small to very large projects with speed and efficiency. "
PKG_IS_ADDON="no"
PKG_AUTORECONF="no"

PKG_CONFIGURE_OPTS_TARGET="ac_cv_fread_reads_directories=yes \
                           ac_cv_snprintf_returns_bogus=yes"
                           
pre_configure_target() {
 cd ..
 rm -rf .$TARGET_NAME
}

make_target() {
 cd $PKG_BUILD
 make git
}

makeinstall_target() { 
mkdir -p $INSTALL/usr/bin
cp git $INSTALL/usr/bin
}
