# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="dotnet-runtime"
PKG_REV="120"
PKG_ARCH="any"
PKG_LICENSE="MIT"
PKG_SITE="https://dotnet.microsoft.com/"
PKG_DEPENDS_TARGET="toolchain icu aspnet5-runtime aspnet6-runtime dotnet3-runtime dotnet5-runtime"
PKG_SECTION="tools"
PKG_SHORTDESC="ASP.NET Core Runtime"
PKG_LONGDESC="ASP.NET Core Runtimes ($(get_pkg_version dotnet3-runtime)), ($(get_pkg_version dotnet5-runtime)) and ($(get_pkg_version aspnet6-runtime)) enables you to run existing console/web/server applications."
PKG_TOOLCHAIN="manual"

PKG_IS_ADDON="yes"
PKG_ADDON_NAME="ASP.Net Core Runtimes"
PKG_ADDON_PROJECTS="any !RPi1"
PKG_ADDON_TYPE="xbmc.python.script"
PKG_MAINTAINER="Anton Voyl (awiouy)"

addon() {
  mkdir -p ${ADDON_BUILD}/${PKG_ADDON_ID}/bin
    cp -r $(get_build_dir dotnet3-runtime)/* \
          ${ADDON_BUILD}/${PKG_ADDON_ID}/bin
    cp -r $(get_build_dir dotnet5-runtime)/* \
          ${ADDON_BUILD}/${PKG_ADDON_ID}/bin
    cp -r $(get_build_dir aspnet5-runtime)/* \
          ${ADDON_BUILD}/${PKG_ADDON_ID}/bin
    cp -r $(get_build_dir aspnet6-runtime)/* \
          ${ADDON_BUILD}/${PKG_ADDON_ID}/bin

  mkdir -p ${ADDON_BUILD}/${PKG_ADDON_ID}/lib
    cp -L $(get_install_dir icu)/usr/lib/lib*.so.?? \
          ${ADDON_BUILD}/${PKG_ADDON_ID}/lib/
}
