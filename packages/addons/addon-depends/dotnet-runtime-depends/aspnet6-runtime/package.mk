# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aspnet6-runtime"
PKG_VERSION="6.0.26"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="bb599cf64b0dc4b4cefbf8aec7a37801d6f89f030ac5ed98eeaf8a11f1e595b3"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/cfc40e77-a6de-481f-812d-6867289e2d8b/eeedeebccc412fd01110d8b59050754d/aspnetcore-runtime-6.0.26-linux-arm64.tar.gz"
    ;;
  "arm")
    PKG_SHA256="3fedc16eb901bc42c354eaa62f082ed600c357327b3b65c3b080e6f6473a17dc"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/c1d42ac0-cd0c-4188-b260-1667a7443534/f0d1a0b4b88432f1c8d31b467d8548f0/aspnetcore-runtime-6.0.26-linux-arm.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="8860633eaf2d24fb5d62913b05a97880f6ca2e9ed4f1e4112e52debe06c994cf"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/b63daa46-51f4-480e-ad03-ef2c5a6a2885/ae059763456991305109bf98b3a67640/aspnetcore-runtime-6.0.26-linux-x64.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"
