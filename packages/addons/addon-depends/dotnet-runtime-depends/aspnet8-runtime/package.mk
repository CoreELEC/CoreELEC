# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aspnet8-runtime"
PKG_VERSION="8.0.6"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="2d03221eb786ad84b865aa5eca84ad79bb2e8cde57e96cac9e0c4edaaec14c0f"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/ccdcbb70-a5e9-4753-b6e3-4461ce56a69d/240803fc1ffba38ab3603778c03e9b87/aspnetcore-runtime-8.0.6-linux-arm64.tar.gz"
    ;;
  "arm")
    PKG_SHA256="b621a6cee65890a123bbf6282fc23190a8abe12af76279a1aa799b0e82f814bf"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/c27a9707-8627-43d3-837e-fa144bab2984/40f243e656752b87ff033e568d49b510/aspnetcore-runtime-8.0.6-linux-arm.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="d4f8ce14e12aba9ca970c15c1ce09486bffc930fe05d4520678763562ca6b48b"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/ce31d92b-b514-4f9c-843b-29c466871369/b332eba5641cbc6eed1e3a98480972d2/aspnetcore-runtime-8.0.6-linux-x64.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"
