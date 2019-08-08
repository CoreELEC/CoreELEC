# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present 5schatten (https://github.com/5schatten)

PKG_NAME="SDL_GameControllerDB"
PKG_VERSION="ef8542c9f354e0f65cb861b67f0aa81366060874"
PKG_SHA256="0e00c189d46a96682065cb6e506b97f5c2c89ff54244f069b7a98155a49ffeae"
PKG_LICENSE="OSS"
PKG_SITE="https://github.com/gabomdq/SDL_GameControllerDB"
PKG_URL="https://github.com/gabomdq/SDL_GameControllerDB/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET=""
PKG_LONGDESC="A community sourced database of game controller mappings to be used with SDL2 Game Controller functionality"
PKG_TOOLCHAIN="manual"

makeinstall_target() {
  mkdir -p $INSTALL/usr/config/SDL-GameControllerDB
  cp $PKG_BUILD/gamecontrollerdb.txt $INSTALL/usr/config/SDL-GameControllerDB
}
