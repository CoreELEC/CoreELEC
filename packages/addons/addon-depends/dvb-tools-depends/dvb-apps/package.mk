# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2012 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="dvb-apps"
PKG_VERSION="9f848ee0b1529ad5d33b62d1035bfdaf607ccbd8"
PKG_SHA256="5ed8693a7d469e47f01923d2a42720ff4b61de1760eb3dee1a49d0c9f8c62d93"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://www.linuxtv.org/wiki/index.php/LinuxTV_dvb-apps"
PKG_URL="https://github.com/tbsdtv/dvb-apps/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Applications for initial setup, testing and operation of an DVB device supporting the DVB-S, DVB-C, DVB-T, and ATSC."
PKG_BUILD_FLAGS="-sysroot"

PKG_MAKE_OPTS_TARGET="enable_shared=no"
PKG_MAKEINSTALL_OPTS_TARGET="enable_shared=no"

pre_make_target() {
  export PERL_USE_UNSAFE_INC=1
}
