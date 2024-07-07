# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aspnet6-runtime"
PKG_VERSION="6.0.31"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="09da2d3fa2facebffeac4185ae1532f61a93f71dffbdb9e3efb0a27bfcf414ce"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/088b0ba5-2eaa-4815-a5c2-3517b99d059c/f6d18014064903be5fa2f654f51f5ce0/aspnetcore-runtime-6.0.31-linux-arm64.tar.gz"
    ;;
  "arm")
    PKG_SHA256="4c35436362adf4aef7f12af49d892f32dab58135895ebbf43209fb3dc9ddb1d3"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/dbcc9954-e476-416b-9411-f65a6d265e67/5a64d97dbae939763192831e828f58d9/aspnetcore-runtime-6.0.31-linux-arm.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="f03fdd09e114028a3e4751d7504280cbf1264ca72bf69aa87bc8489348b46e64"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/c8c7ccb6-b0f8-4448-a542-ed153838cac3/f104b5cc6c11109c0b48e2bb8f5b6cef/aspnetcore-runtime-6.0.31-linux-x64.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"
