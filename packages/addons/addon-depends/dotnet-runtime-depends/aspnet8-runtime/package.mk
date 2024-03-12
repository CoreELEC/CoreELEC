# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aspnet8-runtime"
PKG_VERSION="8.0.4"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="e3b666aa7836a83b71575df0680d0e57bf46b356668521db174e5686fec28cf0"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/80ec12e5-b26f-466c-a20c-f96772ea709d/606e7203912400b44cb35d6fcecf60bf/aspnetcore-runtime-8.0.4-linux-arm64.tar.gz"
    ;;
  "arm")
    PKG_SHA256="f0d65259cb6bdf3324fe0ef2a1b72418ed3db67dbe404debd44bffb352d02875"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/f12a9449-04ba-454c-bc35-4cdb426accf6/2729a371b61d59794845eb309a46fba2/aspnetcore-runtime-8.0.4-linux-arm.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="ec817fa8f7a1c0f9c627425b1eda6ffe7a49bd180cb15d9c59ace2a051dd83e4"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/0b0bc7f4-c6e5-4cec-a7ed-45c2fac0da4b/ae2090564274152b5a4be9f1e66c5d30/aspnetcore-runtime-8.0.4-linux-x64.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"
