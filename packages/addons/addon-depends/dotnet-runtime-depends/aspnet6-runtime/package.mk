# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aspnet6-runtime"
PKG_VERSION="6.0.27"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="8acc813922d3bd6d3cdc10448a62ea6af5dba18ac3137d47dde453d324f53766"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/6be3e44e-1306-422b-845c-9313589bbeb0/d76f133799f6b2c8e3ea7dc9d92b7a03/aspnetcore-runtime-6.0.27-linux-arm64.tar.gz"
    ;;
  "arm")
    PKG_SHA256="14f9db2f396b041c27655dbd8435159a0f570112bafe70abfc6275d99800d32c"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/d339df74-9573-4ca1-9835-61a829e3fcf4/6937d0f4650f3622dbcdbe8a1717f212/aspnetcore-runtime-6.0.27-linux-arm.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="18e11c64d046295a4f5b4164b4142f676b2989977a0ed32c9481783476a63b28"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/d3e6b8a2-f7de-441e-a3af-c18b7584034b/9f15be4d095b7bbb751222b4d68a17e3/aspnetcore-runtime-6.0.27-linux-x64.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"
