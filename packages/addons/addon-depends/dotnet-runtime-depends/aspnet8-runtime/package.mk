# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aspnet8-runtime"
PKG_VERSION="8.0.26"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="5fe42fef4dd28548e7fb38a3db6554c5e1f303dd5b0cf559e78f0300b8bcf96b"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/8.0.26/aspnetcore-runtime-8.0.26-linux-arm64.tar.gz"
    ;;
  "arm")
    PKG_SHA256="5272e62ed8c87b6e93358fa826cc9da9b1e490c6d8a99d837240a48207fa3408"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/8.0.26/aspnetcore-runtime-8.0.26-linux-arm.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="6ca320e839f1b8a788dd93492f6040bb3c3a230ff335bf3a36ee27f2fbd449e5"
    PKG_URL="https://builds.dotnet.microsoft.com/dotnet/aspnetcore/Runtime/8.0.26/aspnetcore-runtime-8.0.26-linux-x64.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"
