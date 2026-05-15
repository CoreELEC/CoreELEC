# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aspnet8-runtime"
PKG_VERSION="8.0.27"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="bbae812d442b525e3bfe2535fee8484f039ca064a4399adffa4f7529d920c492"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/8.0.27/aspnetcore-runtime-8.0.27-linux-arm64.tar.gz"
    ;;
  "arm")
    PKG_SHA256="46f0410557760037b01c0f189e8965b5061e9c33e7385404e4139a5aae93f27a"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/8.0.27/aspnetcore-runtime-8.0.27-linux-arm.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="101ba21953842d2b8668a43d5d49bc27be44747764921f374bc0176284bd6f09"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/8.0.27/aspnetcore-runtime-8.0.27-linux-x64.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"
