# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aspnet8-runtime"
PKG_VERSION="8.0.7"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="61a21ef486e0075ba2c68aaceee0429d731414611d2291e1c7056cd3e5d955bb"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/421d499f-85cb-43dd-97b2-8ebfd06dda8a/61b03be4662125e4af044c7881e66f0e/aspnetcore-runtime-8.0.7-linux-arm64.tar.gz"
    ;;
  "arm")
    PKG_SHA256="9ad9398327a6cb239e7bda239f29a9db64838676113d5a2e54d9319b443f52e7"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/d37fc703-70c6-46f2-a5a1-b60f45fd71d0/6a74aa0bb89feb7f795df1ea92d030bf/aspnetcore-runtime-8.0.7-linux-arm.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="e55bc969b1cb58f96b927127b5c448a15ea844cfc94387f6e35ab585d94abc93"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/06cbb934-ef54-4627-8848-a24a879f2130/52d4247944cee754ec8f4fd617d502a6/aspnetcore-runtime-8.0.7-linux-x64.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"
