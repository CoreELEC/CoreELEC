# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aspnet6-runtime"
PKG_VERSION="6.0.33"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="4019316b34bbdf5756abda4037f87a4328f26abfb6c0c4fd79cf160c35ad337b"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/0c5a5f3a-881e-4ceb-a334-c5e3b210eef8/9834ffebacea659cd14d272fb01f81c4/aspnetcore-runtime-6.0.33-linux-arm64.tar.gz"
    ;;
  "arm")
    PKG_SHA256="f19cf33ad2c53f6285130809f976255c8f45d043e52c4d6a8759363ef4a47cfa"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/4ac9397f-3f4a-4cd0-aba2-35e7f1b47396/9823f50c32028899f430bc3ae87251b1/aspnetcore-runtime-6.0.33-linux-arm.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="4fb761ed8d344405a690b628de883223594e0f19794aa226fb21bd6ddd0c0d0b"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/91f66f75-bd3e-48f1-acb9-99c0da753f96/42c47999ee4c4d108774536afe5da160/aspnetcore-runtime-6.0.33-linux-x64.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"
