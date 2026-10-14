# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aspnet10-runtime"
PKG_VERSION="10.0.13"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="d8d4f61d9985f2ce49f2b918b426d1ee1378d64bb00bc0331274b0df2954aad8"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/10.0.13/aspnetcore-runtime-10.0.13-linux-arm64.tar.gz"
    ;;
  "arm")
    PKG_SHA256="6ce84956d176402f2bef232341d623f9e72ce46d04d08acc17ffc34d5a36530e"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/10.0.13/aspnetcore-runtime-10.0.13-linux-arm.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="28ff9afd94108b574d3565627d0e755f9d20327ec588259eec777497252e0ae9"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/10.0.13/aspnetcore-runtime-10.0.13-linux-x64.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"
