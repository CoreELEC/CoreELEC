# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aspnet9-runtime"
PKG_VERSION="9.0.14"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="2639b53f3e3e31e0d657d80bcb8bbe2a61daa089bd21445cf51b1f894f62be42"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/9.0.14/aspnetcore-runtime-9.0.14-linux-arm64.tar.gz"
    ;;
  "arm")
    PKG_SHA256="150eeed5fbfed139ff3e18bdfc8f8fd200c4ebb25455a1c376b5e15659ee387f"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/9.0.14/aspnetcore-runtime-9.0.14-linux-arm.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="81a10f628c901ba7675b1efaddb9ab36e86d0ca8ac32c1f4003482c5178976ec"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/9.0.14/aspnetcore-runtime-9.0.14-linux-x64.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"
