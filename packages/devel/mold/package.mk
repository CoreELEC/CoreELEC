# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="mold"
PKG_VERSION="3.0.0"
PKG_SHA256="1dee837e227b0c3f2661def602ef8ddc0b889ae5b1d28c1ddec5f29e08e5ceb5"
PKG_LICENSE="MIT"
PKG_SITE="https://github.com/rui314/mold"
PKG_URL="https://github.com/rui314/mold/archive/refs/tags/v${PKG_VERSION}.tar.gz"
PKG_DEPENDS_HOST="ccache:host mimalloc:host"
PKG_LONGDESC="mold is a faster drop-in replacement for existing Unix linkers"
PKG_TOOLCHAIN="manual"

# gcc:host depends on mold:host when MOLD_SUPPORT=yes, and rust:host, cargo:host
# and cargo-reusable:host depend on the toolchain. So only unpack a prebuilt
# rust: the cargo-reusable archive when rust uses it, else the rust snapshots.
if [ "$(get_pkg_variable rust PKG_REUSABLE)" = "yes" ]; then
  # the reusable cargo links against libssl
  PKG_DEPENDS_HOST+=" openssl:host"
  PKG_DEPENDS_UNPACK="cargo-reusable"
else
  PKG_DEPENDS_UNPACK="rustc-snapshot rust-std-snapshot cargo-snapshot"
fi

pre_configure_host() {
  if listcontains "${PKG_DEPENDS_UNPACK}" "cargo-reusable"; then
    PKG_RUST_DIR="$(get_build_dir cargo-reusable)"
  else
    PKG_RUST_DIR="${PKG_BUILD}/rust-snapshot"
    "$(get_build_dir rustc-snapshot)/install.sh" --prefix="${PKG_RUST_DIR}" --disable-ldconfig
    "$(get_build_dir rust-std-snapshot)/install.sh" --prefix="${PKG_RUST_DIR}" --disable-ldconfig
    "$(get_build_dir cargo-snapshot)/install.sh" --prefix="${PKG_RUST_DIR}" --disable-ldconfig
  fi
}

configure_host() {
  cd ${PKG_BUILD}
}

make_host() {
  cd ${PKG_BUILD}

  # rust:host wipes its build dir (and the shared cargo_home) when it unpacks
  export CARGO_HOME="${PKG_BUILD}/cargo_home"
  export RUSTC="${PKG_RUST_DIR}/bin/rustc"
  local rust_host_env="${RUST_HOST//-/_}"
  export "CARGO_TARGET_${rust_host_env^^}_LINKER=${CC}"

  # build the bundled zlib and zstd statically
  export LIBZ_SYS_STATIC="1"

  # only link for the target arch, mold defaults to all of them
  local mold_feature
  case "${TARGET_ARCH}" in
    aarch64) mold_feature="arm64" ;;
    arm) mold_feature="arm32" ;;
    *) mold_feature="${TARGET_ARCH}" ;;
  esac

  # link our mimalloc into mold ahead of libc so it provides malloc. Local libs
  # come before std, which holds the malloc references, so --as-needed would
  # drop it. The -as-needed modifier is unstable, allow it for mold only.
  export RUSTC_BOOTSTRAP="mold"

  ${PKG_RUST_DIR}/bin/cargo rustc -v --release --locked \
                                  --package mold-cli \
                                  --bin mold \
                                  --no-default-features \
                                  --features ${mold_feature} \
                                  -- \
                                  -Z unstable-options \
                                  -L native=${TOOLCHAIN}/lib \
                                  -l dylib:-as-needed=mimalloc \
                                  -C link-arg=-Wl,-rpath,${TOOLCHAIN}/lib
}

makeinstall_host() {
  mkdir -p ${TOOLCHAIN}/${TARGET_NAME}/bin
    cp -a ${PKG_BUILD}/.${RUST_HOST}/target/release/mold ${TOOLCHAIN}/${TARGET_NAME}/bin/
    ln -sf mold ${TOOLCHAIN}/${TARGET_NAME}/bin/ld.mold

  # mold -run finds the wrapper relative to the binary in ../lib/mold
  mkdir -p ${TOOLCHAIN}/${TARGET_NAME}/lib/mold
    cp -a ${PKG_BUILD}/.${RUST_HOST}/target/release/mold-wrapper.so ${TOOLCHAIN}/${TARGET_NAME}/lib/mold/
}

post_makeinstall_host() {
  ln -sf ${TOOLCHAIN}/${TARGET_NAME}/bin/mold ${TARGET_PREFIX}ld.mold
}
