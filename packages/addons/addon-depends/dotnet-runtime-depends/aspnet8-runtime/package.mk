# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aspnet8-runtime"
PKG_VERSION="8.0.2"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="107074b613c48ffcd311670bc34df93f63fce33938ac35770c3db1969da770ef"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/bdfd0216-539e-4dfd-81ea-1b7a77dda929/59a62884bdb8684ef0e4f434eaea0ca3/aspnetcore-runtime-8.0.2-linux-arm64.tar.gz"
    ;;
  "arm")
    PKG_SHA256="940a9e0f89330924a5aed8dedaadb6220f4504f42b5c676e1f6a77060595bb49"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/272dbea2-057e-4032-9857-7e00b476ceec/3c472df94b1c3f5e0d009cbccc9256a6/aspnetcore-runtime-8.0.2-linux-arm.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="9748a8c44b20219b7529b8dde8c17aa07dfadb0206609ea6ce7329269379a121"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/d6d79cc3-df2f-4680-96ff-a7198f461139/df025000eaf5beb85d9137274a8c53ea/aspnetcore-runtime-8.0.2-linux-x64.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"
