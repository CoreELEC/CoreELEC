# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aspnet9-runtime"
PKG_VERSION="9.0.16"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="134335c35cecf0e67ba9989b1c0ddb8970e64b1b6053a57a3a47a1912214bf87"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/9.0.16/aspnetcore-runtime-9.0.16-linux-arm64.tar.gz"
    ;;
  "arm")
    PKG_SHA256="9ef5ca65ce5075331789c215de281783e84677fa658587027485c236d1d65a75"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/9.0.16/aspnetcore-runtime-9.0.16-linux-arm.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="153087f9397cb3df5da69e25b0720135ced9013215df505f38b4940b2dca4c58"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/9.0.16/aspnetcore-runtime-9.0.16-linux-x64.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"
