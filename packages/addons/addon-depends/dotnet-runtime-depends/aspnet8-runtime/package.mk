# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aspnet8-runtime"
PKG_VERSION="8.0.32"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="c53ba238b23b47eec5e1f02980b2d0d128cdddee9cb0f8834c9f9171df1f8710"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/8.0.32/aspnetcore-runtime-8.0.32-linux-arm64.tar.gz"
    ;;
  "arm")
    PKG_SHA256="367c35c63c1ffe0263462ec20871d075ddcf40c26c28c83c0a4e20f77be631dc"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/8.0.32/aspnetcore-runtime-8.0.32-linux-arm.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="bde0bb08f1f10d6dd60d0c3d8a220e33d7ce6d1c74c7098cf7f6a4e1a1ef5137"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/8.0.32/aspnetcore-runtime-8.0.32-linux-x64.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"
