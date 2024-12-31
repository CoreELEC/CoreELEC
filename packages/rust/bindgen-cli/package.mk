# spdx-license-identifier: gpl-2.0-only
# copyright (c) 2024-present team libreelec (https://libreelec.tv)

PKG_NAME="bindgen-cli"
PKG_VERSION="0.71.1"
PKG_SHA256="620d80c32b6aaf42d12d85de86fc56950c86b2a13a5b943c10c29d30c4f3efb0"
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
