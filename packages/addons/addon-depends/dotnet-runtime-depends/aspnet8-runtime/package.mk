# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aspnet8-runtime"
PKG_VERSION="8.0.30"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="827785571f027dd8bbc64603b93826a7274941fb24180ad6af6e5760cf3f67dc"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/8.0.30/aspnetcore-runtime-8.0.30-linux-arm64.tar.gz"
    ;;
  "arm")
    PKG_SHA256="f1270b01b719b52e2274862ab1c7945886e14b7750eadb332cd27798636dc9ea"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/8.0.30/aspnetcore-runtime-8.0.30-linux-arm.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="3591ce9a61e635cee8c93630a5e0f338398cb19c5bd618a2f38f8f00e4a3c45e"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/8.0.30/aspnetcore-runtime-8.0.30-linux-x64.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"
