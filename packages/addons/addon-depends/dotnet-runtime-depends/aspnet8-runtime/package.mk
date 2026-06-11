# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aspnet8-runtime"
PKG_VERSION="8.0.28"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="96bb848ecb24090b659711966386735c3f4210eef91dd1bb1e0408232bc834ba"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/8.0.28/aspnetcore-runtime-8.0.28-linux-arm64.tar.gz"
    ;;
  "arm")
    PKG_SHA256="197c9bb5965cd6ca4a222a8fded1299dff74fc11e21f4e1bb4eaa1f47f92ec1d"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/8.0.28/aspnetcore-runtime-8.0.28-linux-arm.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="b40cb21cd2f5a6120ca4734f02c42d52024f588792b7817a1b1e4fcd053ba433"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/8.0.28/aspnetcore-runtime-8.0.28-linux-x64.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"
