# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aspnet9-runtime"
PKG_VERSION="9.0.19"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="6d09f9462268e110bbaf4577eec61fa3df2b449c5d6f2d3b4eec7d669367a483"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/9.0.19/aspnetcore-runtime-9.0.19-linux-arm64.tar.gz"
    ;;
  "arm")
    PKG_SHA256="3cf459678cf922a1fa018b9f5145f1451ff78dcc376c462dbdc76dbcdfc90825"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/9.0.19/aspnetcore-runtime-9.0.19-linux-arm.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="d949d01244ffd3bc0ab23602a4c186d5e961cae75961f0ed0e25b9e57e164b4f"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/9.0.19/aspnetcore-runtime-9.0.19-linux-x64.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"
