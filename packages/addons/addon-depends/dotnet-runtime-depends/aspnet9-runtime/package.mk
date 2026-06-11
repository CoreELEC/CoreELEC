# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aspnet9-runtime"
PKG_VERSION="9.0.17"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="ffe7b74e1c60b06941773d87ea76b168b9b4e6aedbe4f2050b2d01f61666fecf"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/9.0.17/aspnetcore-runtime-9.0.17-linux-arm64.tar.gz"
    ;;
  "arm")
    PKG_SHA256="1aae5695823f1433e46c3a74151c235c48f5f51b1229e85a06786c6bd94ef848"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/9.0.17/aspnetcore-runtime-9.0.17-linux-arm.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="5c4220c50cd413620dc9768e86945d5ee9d65280200f9932c12b7646132e1546"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/9.0.17/aspnetcore-runtime-9.0.17-linux-x64.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"
