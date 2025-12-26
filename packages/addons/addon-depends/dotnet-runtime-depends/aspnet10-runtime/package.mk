# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aspnet10-runtime"
PKG_VERSION="10.0.12"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="bc4a2513b996a8e1c1eb05a68bb40df93362669e2a64832f9e7ff429adbe6115"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/10.0.12/aspnetcore-runtime-10.0.12-linux-arm64.tar.gz"
    ;;
  "arm")
    PKG_SHA256="c1475990bb10f134031bebc138d44379b141faa4f08c591c7df9162088c46b4b"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/10.0.12/aspnetcore-runtime-10.0.12-linux-arm.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="321d5848dbf1daa0b63743e213f8961c5c701f328cd03fb2b12042618dd565ad"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/10.0.12/aspnetcore-runtime-10.0.12-linux-x64.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"
