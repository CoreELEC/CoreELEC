# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aspnet6-runtime"
PKG_VERSION="6.0.22"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="19895e7c6655b18841d46124a9ef162a7f64769812e08f39cb1d343f9c1f91cd"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/000ddf12-2c8b-4d97-9b3d-f76c8fef461e/c2dfb5a82b7952cb272c0f5dbeb7fcb1/aspnetcore-runtime-6.0.22-linux-arm64.tar.gz"
    ;;
  "arm")
    PKG_SHA256="72c54614cea6b9fced48d1cd85b6da298599d55f01591075a4cf6632b35f21b9"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/994282df-ceee-45e9-890a-cd979a7ae186/f54f388f61b7a2a57b39d166f9936966/aspnetcore-runtime-6.0.22-linux-arm.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="5b595dc770de578c0dff3943aae155a19e73854d49e26ecf2379d867db003f33"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/a936856b-96f0-4525-8f74-b96b792c3664/2da9be398c92985d3f95c3336361d1ba/aspnetcore-runtime-6.0.22-linux-x64.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"
