# SPDX-License-Identifier: GPL-3.0
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="moonlight-embedded"
PKG_VERSION="97598d9354edf6186ee1bfc9e8d18a6f90984208"
PKG_SHA256="014c0986258e2eba648be51d55310e7dd3801ce9498395cf671148097993240b"
PKG_ARCH="any"
PKG_LICENSE="GPLv3"
PKG_SITE="https://github.com/TheChoconut/moonlight-embedded"
PKG_URL="https://github.com/TheChoconut/moonlight-embedded/archive/${PKG_VERSION}.tar.gz"
PKG_MAINTAINER="TheChoconut"
PKG_DEPENDS_TARGET="toolchain curl pulseaudio systemd alsa-lib moonlight-common-c libevdev sdlgamecontrollerdb enet opus libamcodec"
PKG_SECTION=""
PKG_SHORTDESC="Open source NVIDIA GameStream Linux client"
PKG_LONGDESC="Moonlight Embedded is an open source implementation of NVIDIA's GameStream, as used by the NVIDIA Shield, but built for Linux."
PKG_CMAKE_OPTS_TARGET="-DDISABLE_SOFTWARE=ON -DDISABLE_CEC=ON"

pre_build_target() {
  cp -a $(get_build_dir moonlight-common-c)/* ${PKG_BUILD}/third_party/moonlight-common-c
  cp -a $(get_build_dir sdlgamecontrollerdb)/* ${PKG_BUILD}/third_party/SDL_GameControllerDB
  cp -a $(get_build_dir enet)/* ${PKG_BUILD}/third_party/moonlight-common-c/enet
}
