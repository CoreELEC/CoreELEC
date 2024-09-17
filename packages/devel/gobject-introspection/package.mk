# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)
# Copyright (C) 2020-present Team CoreELEC (https://coreelec.org)

PKG_NAME="gobject-introspection"
PKG_VERSION="1.82.0"
PKG_SHA256="82c2372520d9ec58f0361efddb2ef9ce4026618b7389e99ba73f8a4e683ed361"
PKG_ARCH="any"
PKG_LICENSE="LGPL"
PKG_SITE="http://www.gtk.org/"
PKG_URL="https://github.com/GNOME/gobject-introspection/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain libffi glib-initial Python3 qemu:host gobject-introspection:host"
PKG_DEPENDS_HOST="libffi:host glib:host"
PKG_SECTION="devel"
PKG_LONGDESC="Middleware layer between GObject-using C libraries and language bindings."
PKG_TOOLCHAIN="meson"

post_unpack() {
  # disable tests
  sed -e "s|subdir('tests')|# subdir('tests')|" \
      -i ${PKG_BUILD}/meson.build
}

pre_configure_host() {
  PKG_MESON_OPTS_HOST="-Ddoctool=disabled"

  # prevent g-ir-scanner from writing cache data to $HOME
  export GI_SCANNER_DISABLE_CACHE=1
}

post_makeinstall_host() {
  # for gi this variables must be defined
  # for target and not for host
  sed -e "s|'CC'|'TARGET_CC'|g" \
      -e "s|'CXX'|'TARGET_CXX'|g" \
      -e "s|'AR'|'TARGET_AR'|g" \
      -e "s|'CFLAGS'|'TARGET_CFLAGS'|g" \
      -e "s|'LDFLAGS'|'TARGET_LDFLAGS'|g" \
      -e "s|ldshared.startswith(cc)|True|g" \
      -e "s|ldshared\[len(cc):\]|''|g" \
      -i ${TOOLCHAIN}/lib/gobject-introspection/giscanner/ccompiler.py
}

pre_configure_target() {
  QEMU_BINARY="${TOOLCHAIN}/bin/qemu-${TARGET_ARCH}"
  PKG_CONFIG_PATH="${SYSROOT_PREFIX}/usr/lib/pkgconfig"

  PKG_MESON_OPTS_TARGET=" \
    -Ddoctool=disabled \
    -Dpython=${TOOLCHAIN}/bin/${PKG_PYTHON_VERSION} \
    -Dgi_cross_use_prebuilt_gi=true \
    -Dgi_cross_binary_wrapper=${TOOLCHAIN}/bin/g-ir-scanner-qemuwrapper \
    -Dgi_cross_ldd_wrapper=${TOOLCHAIN}/bin/g-ir-scanner-lddwrapper \
    -Dbuild_introspection_data=true"

  # qemu wrapper that will be given to gi-scanner so that
  # it can run target helper binaries through that
  cat > ${TOOLCHAIN}/bin/g-ir-scanner-qemuwrapper << EOF
#!/bin/sh
  # prevent g-ir-scanner from writing cache data to $HOME
  export GI_SCANNER_DISABLE_CACHE=1

  ${QEMU_BINARY} \
    -E LD_LIBRARY_PATH="${SYSROOT_PREFIX}/usr/lib" \
    -L ${SYSROOT_PREFIX}/usr \
    "\$@"
EOF

  # wrapper to use instead of ldd, which does not work
  # when a binary is built for a different architecture
  cat > ${TOOLCHAIN}/bin/g-ir-scanner-lddwrapper << EOF
#!/bin/sh
   ${TARGET_PREFIX}objdump -p "\$@"
EOF

  # wrapper for g-ir-scanner itself, which will be used when building introspection files
  # it calls the native version of the scanner, and tells it to use a qemu
  # wrapper for running transient target binaries produced by the scanner
  cat > ${TOOLCHAIN}/bin/g-ir-scanner-wrapper << EOF
#!/bin/sh
  # prevent g-ir-scanner from writing cache data to $HOME
  export GI_SCANNER_DISABLE_CACHE=1

  ${TOOLCHAIN}/bin/g-ir-scanner \
    --use-binary-wrapper=${TOOLCHAIN}/bin/g-ir-scanner-qemuwrapper \
    --use-ldd-wrapper=${TOOLCHAIN}/bin/g-ir-scanner-lddwrapper \
    "\$@"
EOF

  # wrapper for g-ir-compiler, which runs the target version of it through qemu
  # g-ir-compiler writes out the raw content of a C struct
  # to disk and therefore is arch dependent
  cat > ${TOOLCHAIN}/bin/g-ir-compiler-wrapper << EOF
#!/bin/sh
  ${TOOLCHAIN}/bin/g-ir-scanner-qemuwrapper \
    ${TOOLCHAIN}/${TARGET_NAME}/sysroot/usr/bin/g-ir-compiler \
    "\$@"
EOF

  chmod +x ${TOOLCHAIN}/bin/g-ir-*wrapper
}

post_makeinstall_target() {
  rm -rf ${INSTALL}/usr/bin
  rm -rf ${INSTALL}/usr/lib/gobject-introspection
  rm -rf ${INSTALL}/usr/share

  # tweak the binary names so that it picks up our
  # wrappers which do the cross-compile with qemu
  sed -e "s|prefix=.*|prefix=${TOOLCHAIN}|" \
      -e "s|g_ir_scanner=.*|g_ir_scanner=\${bindir}/g-ir-scanner-wrapper|" \
      -e "s|g_ir_compiler=.*|g_ir_compiler=\${bindir}/g-ir-compiler-wrapper|" \
      -i ${SYSROOT_PREFIX}/usr/lib/pkgconfig/gobject-introspection-1.0.pc
}
