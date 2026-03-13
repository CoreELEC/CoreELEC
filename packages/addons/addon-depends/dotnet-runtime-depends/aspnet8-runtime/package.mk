# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aspnet8-runtime"
PKG_VERSION="8.0.25"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="b63ccc5717cde0c8cf83b7d4e9a4c6d5ddb09c24d5e80158084547fc99d1a001"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/8.0.25/aspnetcore-runtime-8.0.25-linux-arm64.tar.gz"
    ;;
  "arm")
    PKG_SHA256="d89e096d6533999ce1a9c8f9ce26b0250b08699a7a7fdabb85aef99ea55de6fe"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/8.0.25/aspnetcore-runtime-8.0.25-linux-arm.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="deb256183aef75800e222c31fe91ff71d4d4c54598c0c2ad922b227ce6dbf816"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/8.0.25/aspnetcore-runtime-8.0.25-linux-x64.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"
