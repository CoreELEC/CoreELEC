# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="rkbin"
if [ "$DEVICE" == "OdroidGoAdvance" ] || [ "$DEVICE" == "GameForce" ]; then
PKG_VERSION="5e54e6c0bbc0141985aca17adcebf3692cdc7f78"
PKG_SHA256="39289966b39c7bd5b2e5031d12f071e1aa80b80dc83c74b8658b3c119b6d5fd5"
else
# Version is: Kwiboo/tag:libreelec-4563e24
PKG_VERSION="4563e249a3f47e7fcd47a4c3769b6c05683b6e9d"
PKG_SHA256="0b3479117700bce9afea2110c1f027b626c76d99045802218b35a53606547d60"
fi
PKG_ARCH="arm aarch64"
PKG_LICENSE="nonfree"
PKG_SITE="https://github.com/rockchip-linux/rkbin"
PKG_URL="https://github.com/rockchip-linux/rkbin/archive/${PKG_VERSION}.tar.gz"
PKG_LONGDESC="rkbin: Rockchip Firmware and Tool Binaries"
PKG_TOOLCHAIN="manual"
