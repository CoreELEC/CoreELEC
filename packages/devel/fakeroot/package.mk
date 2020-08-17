# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)

# fakeroot-1.20.2 depends on libcap:host, which depends on attr:host
# there are reported buildproblems with attr:host, which should be replicated
# use fakeroot-1.18.4 instead until attr:host builds

PKG_NAME="fakeroot"
PKG_VERSION="1.24"
PKG_SHA256="2e045b3160370b8ab4d44d1f8d267e5d1d555f1bb522d650e7167b09477266ed"
PKG_LICENSE="GPL3"
PKG_SITE="http://fakeroot.alioth.debian.org/"
PKG_URL="http://ftp.debian.org/debian/pool/main/f/fakeroot/${PKG_NAME}_${PKG_VERSION}.orig.tar.gz"
PKG_DEPENDS_HOST="ccache:host libcap:host"
PKG_LONGDESC="fakeroot provides a fake root environment by means of LD_PRELOAD and SYSV IPC (or TCP) trickery."

PKG_CONFIGURE_OPTS_HOST="--with-gnu-ld --with-ipc=tcp"
