# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aspnet8-runtime"
PKG_VERSION="8.0.31"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="e103ff30bb547983034d094410fbf7821ab307ac67633e8327d7e5812e565c96"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/8.0.31/aspnetcore-runtime-8.0.31-linux-arm64.tar.gz"
    ;;
  "arm")
    PKG_SHA256="629a3b1ec6c98685b002833162e57aa3f94bcecc5dfc75ea3f6fa642f83623ad"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/8.0.31/aspnetcore-runtime-8.0.31-linux-arm.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="e69404ee842cdb128558e7975234f48f0abee32cc3adc3b2867d0ba38cf594b6"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/8.0.31/aspnetcore-runtime-8.0.31-linux-x64.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"
