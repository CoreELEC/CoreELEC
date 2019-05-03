# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)

PKG_NAME="fakeroot"
PKG_VERSION="1.23"
PKG_SHA256="009cd6696a931562cf1c212bb57ca441a4a2d45cd32c3190a35c7ae98506f4f6"
PKG_ARCH="any"
PKG_LICENSE="GPL3"
PKG_SITE="http://fakeroot.alioth.debian.org/"
PKG_URL="http://ftp.debian.org/debian/pool/main/f/fakeroot/${PKG_NAME}_${PKG_VERSION}.orig.tar.xz"
PKG_DEPENDS_HOST="ccache:host libcap:host"
PKG_SECTION="toolchain/devel"
PKG_SHORTDESC="fakeroot: provides a fake root environment by means of LD_PRELOAD and SYSV IPC (or TCP) trickery."
PKG_LONGDESC="fakeroot provides a fake root environment by means of LD_PRELOAD and SYSV IPC (or TCP) trickery."

PKG_CONFIGURE_OPTS_HOST="--with-gnu-ld"
