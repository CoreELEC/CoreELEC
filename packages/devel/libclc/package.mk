# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2024-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="libclc"
PKG_VERSION="$(get_pkg_version llvm)"
PKG_LICENSE="NCSA OR MIT"
PKG_URL=""
PKG_DEPENDS_HOST="toolchain:host llvm:host"
PKG_DEPENDS_TARGET="toolchain llvm:host"
PKG_LONGDESC="Low-Level Virtual Machine (LLVM) is a compiler infrastructure."
PKG_DEPENDS_UNPACK+=" llvm"
PKG_PATCH_DIRS+=" $(get_pkg_directory llvm)/patches"
PKG_TOOLCHAIN="cmake"

unpack() {
  mkdir -p ${PKG_BUILD}
  tar --strip-components=1 -xf ${SOURCES}/llvm/llvm-${PKG_VERSION}.tar.xz -C ${PKG_BUILD}
}

pre_configure() {
  PKG_CMAKE_SCRIPT="${PKG_BUILD}/libclc/CMakeLists.txt"
  # libclc builds the one target named by LLVM_DEFAULT_TARGET_TRIPLE, and its
  # CLC language falls back to CMAKE_C_COMPILER unless clang is named
  LIBCLC_CMAKE_OPTS="-DLLVM_DEFAULT_TARGET_TRIPLE=spirv64-unknown-unknown \
                     -DCMAKE_CLC_COMPILER=${TOOLCHAIN}/bin/clang"
}

pre_configure_host() {
  mkdir -p "${PKG_BUILD}/.${HOST_NAME}"
  cd ${PKG_BUILD}/.${HOST_NAME}
  PKG_CMAKE_OPTS_HOST="${LIBCLC_CMAKE_OPTS}"
}

pre_configure_target() {
  mkdir -p "${PKG_BUILD}/.${TARGET_NAME}"
  cd ${PKG_BUILD}/.${TARGET_NAME}
  # cross-compile: use HOST LLVM so cmake finds clang, opt, llvm-spirv, not sysroot LLVM whose tools were stripped
  PKG_CMAKE_OPTS_TARGET="${LIBCLC_CMAKE_OPTS} \
                         -DLLVM_DIR=${TOOLCHAIN}/lib/cmake/llvm"
}
