# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aspnet6-runtime"
PKG_VERSION="6.0.25"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="1c652059f51c82847a68f0827a164d3e6bbda54ab9fe951c2ed22213bc4d3535"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/8f085f4e-ce83-494f-add1-7e6d4e04f90e/398b661de84bda4d74b5c04fa709eadb/aspnetcore-runtime-6.0.25-linux-arm64.tar.gz"
    ;;
  "arm")
    PKG_SHA256="10ab3b8b6964cb87b4be458c9a3195dfff01fb20b95ec0bd7c80b43cf4f8087a"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/88cf902b-08e0-4329-b2cf-7d0ab104d97d/287edc7e830d810424d62f6efc5c577a/aspnetcore-runtime-6.0.25-linux-arm.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="b4ef19514e6b45f893d6b0f6f436ec01338fcca197faa25e88f0d3a5a0c6c5d0"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/0cf64d28-dec3-4553-b38d-8f526e6f64b0/0bf8e79d48da8cb4913bc1c969653e9a/aspnetcore-runtime-6.0.25-linux-x64.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"
