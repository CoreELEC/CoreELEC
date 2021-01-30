# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

PKG_NAME="youtube-dl"
PKG_VERSION="2021.01.24.1"
PKG_SHA256="cb9f350dde08685c8f841818b6fe54271361f590570554313a897810f0240a8a"
PKG_LICENSE="The Unlicense"
PKG_SITE="https://github.com/ytdl-org/youtube-dl"
PKG_URL="${PKG_SITE}/releases/download/${PKG_VERSION}/youtube-dl"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="Command-line program to download videos from YouTube.com and other video sites"
PKG_TOOLCHAIN="manual"

unpack() {
:
}

makeinstall_target() {
mkdir -p ${INSTALL}/usr/bin
cp -rf ${SOURCES}/${PKG_NAME}/${PKG_NAME}-${PKG_VERSION}.youtube-dl ${INSTALL}/usr/bin/youtube-dl
chmod +x ${INSTALL}/usr/bin/youtube-dl
}
