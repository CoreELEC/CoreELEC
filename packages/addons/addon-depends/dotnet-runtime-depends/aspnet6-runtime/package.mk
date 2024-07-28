# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aspnet6-runtime"
PKG_VERSION="6.0.32"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="70d7035083bc2b330709eb6208d082a3cfc18839425b31bccff032aadc66c212"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/7b3ead1a-441d-42b9-ac91-1253ed8aee48/044d517eaff9f65e18e3e27f4d825d34/aspnetcore-runtime-6.0.32-linux-arm64.tar.gz"
    ;;
  "arm")
    PKG_SHA256="1500178b218dc218c1465b9b60b248c8780dccb15b62a56641d03c8d816eff16"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/d5106f1a-d140-4c8c-b480-001824b72768/7e9cf426bf45040eadfcc8bb20227b6d/aspnetcore-runtime-6.0.32-linux-arm.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="ee937f7c03f4e908c3dcb0f1c063bd911bc08f7a30d49ea41f084fa403b923f0"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/99f90118-96b4-4d06-97ad-d779715319f6/aecf393f9b9d362b66b93a47d90cfa8d/aspnetcore-runtime-6.0.32-linux-x64.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"
