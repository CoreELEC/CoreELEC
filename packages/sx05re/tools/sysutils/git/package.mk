# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="git"
PKG_VERSION="2.34.1"
PKG_SHA256="fc4eb5ecb9299db91cdd156c06cdeb41833f53adc5631ddf8c0cb13eaa2911c1"
PKG_REV="1"
PKG_ARCH="any"
PKG_LICENSE="GPLv2"
PKG_SITE="https://git-scm.com/"
PKG_URL="https://mirrors.edge.kernel.org/pub/software/scm/git/git-$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain openssl pcre curl libiconv zlib"
PKG_SECTION="emuelec"
PKG_SHORTDESC="Git is a free and open source distributed version control system designed to handle everything from small to very large projects with speed and efficiency. "
PKG_LONGDESC="Git is a free and open source distributed version control system designed to handle everything from small to very large projects with speed and efficiency. "


PKG_CONFIGURE_OPTS_TARGET="ac_cv_fread_reads_directories=yes \
                           ac_cv_snprintf_returns_bogus=yes \
                           ac_cv_iconv_omits_bom=yes"
                           
pre_configure_target() {
 cd ..
 rm -rf .$TARGET_NAME
}
