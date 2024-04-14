# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aspnet6-runtime"
PKG_VERSION="6.0.29"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="11296bb5c22b7b38cfd0cba472fc79d40edc4e778238a1fab466028504cc84e9"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/094fe5d6-0520-4c0a-9edf-b53d269f8b20/8c5e69ed04787815037ae373ffb77466/aspnetcore-runtime-6.0.29-linux-arm64.tar.gz"
    ;;
  "arm")
    PKG_SHA256="f6d6d7418a5bff97c653db1aa8ba4faf83379521e519918f31afcd4c41051833"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/57510d35-63b1-4535-bf83-e10deb8a3b78/b052a4381befd434cbe8da36ab937ff8/aspnetcore-runtime-6.0.29-linux-arm.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="2ed988b68f40582edca7280e07b1e0bc342bfcd25b28016f4ff48f75fd3f775b"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/70ddd1ed-776d-41d2-b192-f02436ef3ca6/337d6dd35177408acb9889289a7743a7/aspnetcore-runtime-6.0.29-linux-x64.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"
