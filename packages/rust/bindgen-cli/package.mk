# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2024-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="bindgen-cli"
PKG_VERSION="0.73.2"
PKG_SHA256="5fc3277dd334e4bbbd7bdb8cad5d4fb87a667b2e95a2bae350a8d5424c280629"
PKG_LICENSE="BSD-3-Clause"
PKG_SITE="https://rust-lang.github.io/rust-bindgen/"
PKG_URL="https://github.com/rust-lang/rust-bindgen/archive/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="cargo:host"
PKG_LONGDESC="Automatically generates Rust FFI bindings to C and some C++ libraries"
PKG_TOOLCHAIN="manual"

configure_host() {
  cd ${PKG_BUILD}
}

make_host() {
  cd ${PKG_BUILD}

  cargo build -v --target ${RUST_HOST} --release
}

makeinstall_host() {
  mkdir -p ${TOOLCHAIN}/bin
    cp -a ${PKG_BUILD}/.${RUST_HOST}/target/${RUST_HOST}/release/bindgen ${TOOLCHAIN}/bin/
}
