# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aspnet9-runtime"
PKG_VERSION="9.0.18"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="3fdd35388dbf31721f1bf453a3e40c2814f5ad3a3a2493ba8919c85e63e90dae"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/9.0.18/aspnetcore-runtime-9.0.18-linux-arm64.tar.gz"
    ;;
  "arm")
    PKG_SHA256="f514aa65328bbbaab0b5e6a415336cf8eb0435f05aa9e6a8f9a7afd11b868f69"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/9.0.18/aspnetcore-runtime-9.0.18-linux-arm.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="5bff8639acecf30672c08ec3a0c8aeb583f5b55115be4e1c8181f199b8cf36e6"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/9.0.18/aspnetcore-runtime-9.0.18-linux-x64.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"
