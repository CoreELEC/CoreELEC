# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="fakeroot"
PKG_VERSION="2.1.4"
PKG_SHA256="0822bd5a9f0cf19d2ba0546b88b0432d4d3d9917db62c57b74044ccadba06e49"
PKG_LICENSE="GPL-3.0-or-later"
PKG_SITE="https://tracker.debian.org/pkg/fakeroot"
PKG_URL="http://ftp.debian.org/debian/pool/main/f/fakeroot/${PKG_NAME}_${PKG_VERSION}.orig.tar.xz"
PKG_DEPENDS_HOST="ccache:host libcap:host libtool:host meson:host ninja:host"
PKG_LONGDESC="fakeroot provides a fake root environment by means of LD_PRELOAD and SYSV IPC (or TCP) trickery."

PKG_MESON_OPTS_HOST="-Ddocs=false"
