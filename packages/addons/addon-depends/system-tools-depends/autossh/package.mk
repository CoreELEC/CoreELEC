################################################################################
#      This file is part of LibreELEC - https://libreelec.tv
#      Copyright (C) 2016-present Team LibreELEC
#
#  LibreELEC is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 2 of the License, or
#  (at your option) any later version.
#
#  LibreELEC is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with LibreELEC.  If not, see <http://www.gnu.org/licenses/>.
################################################################################

PKG_NAME="autossh"
PKG_VERSION="1.4f"
PKG_SHA256="0172e5e1bea40c642e0ef025334be3aadd4ff3b4d62c0b177ed88a8384e2f8f2"
PKG_ARCH="any"
PKG_LICENSE="GPL"
PKG_SITE="http://www.harding.motd.ca/"
PKG_URL="http://www.harding.motd.ca/autossh/autossh-${PKG_VERSION}.tgz"
PKG_DEPENDS_TARGET="toolchain"
PKG_SECTION="tools"
PKG_LONGDESC="Automatically restart SSH sessions and tunnels"
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_TARGET="ac_cv_func_malloc_0_nonnull=yes \
                           ac_cv_func_realloc_0_nonnull=yes"

pre_configure_target() {
  export CFLAGS="$CFLAGS -I."
}

makeinstall_target() {
  :
}
