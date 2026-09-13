# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aspnet9-runtime"
PKG_VERSION="9.0.20"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="948f49d0f0ac3d67638f24a0c6d6ed1008eced27bfaf055b17a8dd5569f5b0d7"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/9.0.20/aspnetcore-runtime-9.0.20-linux-arm64.tar.gz"
    ;;
  "arm")
    PKG_SHA256="68ba01b0d41c065440bdc9a9f4793aefc459ab1022bf91d5af5cba9386241f8a"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/9.0.20/aspnetcore-runtime-9.0.20-linux-arm.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="2e7a60cb847b57fda9c4027fdf68afbc98dc318a8965661b06dd67a2e6a69c9d"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/9.0.20/aspnetcore-runtime-9.0.20-linux-x64.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"
