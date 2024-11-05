# spdx-license-identifier: gpl-2.0-only
# copyright (c) 2024-present team libreelec (https://libreelec.tv)

PKG_NAME="bindgen-cli"
PKG_VERSION="0.69.5"
PKG_SHA256="30e69cd65a9f5ed08d8cf1b75626c13f39fce9a843cb109fbf778c8dd402439f"
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
