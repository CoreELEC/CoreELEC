# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aspnet8-runtime"
PKG_VERSION="8.0.29"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="840f8e365c62acce12b219aa0c011bcba2b6599586b77b3acc03c0a0cdb22698"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/8.0.29/aspnetcore-runtime-8.0.29-linux-arm64.tar.gz"
    ;;
  "arm")
    PKG_SHA256="4ada2a0bf6a339d1e97425b8984e9ffff799bcbcf063b73962841c5b766e5a5e"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/8.0.29/aspnetcore-runtime-8.0.29-linux-arm.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="fcb2e17c57cade69e0350e40dc5ec63f02143ef686b3d72bfb02f3146f13a6b5"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/8.0.29/aspnetcore-runtime-8.0.29-linux-x64.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"
