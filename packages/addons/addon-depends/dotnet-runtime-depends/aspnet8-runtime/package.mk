# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="aspnet8-runtime"
PKG_VERSION="8.0.8"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."
PKG_TOOLCHAIN="manual"

case "${ARCH}" in
  "aarch64")
    PKG_SHA256="ac79115682ee679756838ee623ca46617322c787826f3638438bc6443fcee345"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/f6fcf2c9-39ad-49c7-80b5-92306309e796/3cac9217f55528cb60c95702ba92d78b/aspnetcore-runtime-8.0.8-linux-arm64.tar.gz"
    ;;
  "arm")
    PKG_SHA256="284c4c9ae3eae7548450ead59e445b3b64c72301ecf393926578231e480dd21e"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/26f16795-9928-4ddd-96f4-666e6e256715/bf797e4f997c965aeb0183b467fcf71a/aspnetcore-runtime-8.0.8-linux-arm.tar.gz"
    ;;
  "x86_64")
    PKG_SHA256="7bee47a53a0a4977e4182e8085355d146be6b2f958aa3f3ae2de0c39439e7348"
    PKG_URL="https://download.visualstudio.microsoft.com/download/pr/648de803-0b0c-46bc-9601-42a94dae0b41/241fd17cee8d473a78675e30681979bb/aspnetcore-runtime-8.0.8-linux-x64.tar.gz"
    ;;
esac
PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"
