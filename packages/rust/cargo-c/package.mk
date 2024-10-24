# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2023-present Team CoreELEC (https://coreelec.org)

PKG_NAME="cargo-c"
PKG_VERSION="v0.10.5"
PKG_SHA256="3f131a6a647851a617a87daaaf777a9e50817957be0af29806615613e98efc8a"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/lu-zero/cargo-c"
PKG_URL="https://github.com/lu-zero/cargo-c/archive/refs/tags/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="cargo:host"
PKG_LONGDESC="Use Cargo-c to build and install C-compatible libraries"
PKG_TOOLCHAIN="manual"

make_host() {
  cargo build --release --manifest-path ${PKG_BUILD}/Cargo.toml
}

makeinstall_host() {
  cargo install --profile release --path ${PKG_BUILD}
}
