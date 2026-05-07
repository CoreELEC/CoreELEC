# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="flatpak"
PKG_VERSION="1.16.6"
PKG_REV="0"
PKG_ARCH="aarch64 x86_64"
PKG_SHA256="cff1fd58c5f5163107d634c050bbed3ce7264af9b611540de6b87f760479eb69"
PKG_LICENSE="LGPL-2.1"
PKG_SITE="https://flatpak.org/"
PKG_URL="https://github.com/flatpak/flatpak/archive/${PKG_VERSION}.tar.gz"
PKG_DEPENDS_TARGET="toolchain appstream bubblewrap fuse3 gdk-pixbuf gpgme json-glib libarchive ostree pyparsing:host systemd xdg-dbus-proxy zstd"
PKG_DEPENDS_CONFIG="appstream libassuan gpgme ostree shared-mime-info xz"
PKG_SECTION="tools"
PKG_SHORTDESC="Linux application sandboxing and distribution framework"
PKG_LONGDESC="Linux application sandboxing and distribution framework"
PKG_BUILD_FLAGS="-sysroot"

PKG_IS_ADDON="yes"
PKG_ADDON_NAME="Flatpak"
PKG_ADDON_PROJECTS="any !Generic-legacy"
PKG_ADDON_VERSION="${ADDON_VERSION}.${PKG_REV}"
PKG_ADDON_REQUIRES="tools.gnupg:0.0.0 tools.externalhelper:0.0.0"

PKG_MESON_OPTS_TARGET="-Ddconf=disabled \
                       -Ddocbook_docs=disabled \
                       -Dgir=disabled \
                       -Dgtkdoc=disabled \
                       -Dlibzstd=enabled \
                       -Dmalcontent=disabled \
                       -Dman=disabled \
                       -Drun_media_dir=/media \
                       -Dseccomp=disabled \
                       -Dselinux_module=disabled \
                       -Dsystem_bubblewrap=/storage/.kodi/addons/tools.flatpak/bin/bwrap \
                       -Dsystem_dbus_proxy=/storage/.kodi/addons/tools.flatpak/bin/xdg-dbus-proxy \
                       -Dsystem_bubblewrap=/storage/.kodi/addons/tools.flatpak/bin/bwrap \
                       -Dsystem_fusermount=/storage/.kodi/addons/tools.flatpak/bin/fusermount3 \
                       -Dsystem_install_dir=/storage/flatpak \
                       -Dsystem_helper=disabled \
                       -Dsystemd=enabled \
                       -Dtests=false \
                       -Dxauth=disabled \
                       -Dwayland_security_context=disabled"

pre_configure_target() {
  export TARGET_CFLAGS+=" -Wno-discarded-qualifiers \
                          -I$(get_install_dir gpgme)/usr/include"
  export TARGET_LDFLAGS+=" -L$(get_install_dir gpgme)/usr/lib -lcrypto -lbz2 -lz -llz4 -llzo2"
}

addon() {
  mkdir -p ${ADDON_BUILD}/${PKG_ADDON_ID}/bin
    cp ${PKG_INSTALL}/usr/bin/flatpak ${ADDON_BUILD}/${PKG_ADDON_ID}/bin
    cp ${PKG_INSTALL}/usr/lib/{flatpak-portal,flatpak-session-helper} ${ADDON_BUILD}/${PKG_ADDON_ID}/bin
    cp $(get_install_dir bubblewrap)/usr/bin/bwrap ${ADDON_BUILD}/${PKG_ADDON_ID}/bin
    cp $(get_install_dir fuse3)/usr/bin/fusermount3 ${ADDON_BUILD}/${PKG_ADDON_ID}/bin
    cp $(get_install_dir xdg-dbus-proxy)/usr/bin/xdg-dbus-proxy ${ADDON_BUILD}/${PKG_ADDON_ID}/bin

    for f in bwrap flatpak flatpak-portal flatpak-session-helper fusermount3 xdg-dbus-proxy; do
      patchelf --add-rpath '${ORIGIN}/../lib.private' ${ADDON_BUILD}/${PKG_ADDON_ID}/bin/$f
    done

  mkdir -p ${ADDON_BUILD}/${PKG_ADDON_ID}/lib.private
    cp -L $(get_install_dir appstream)/usr/lib/libappstream.so.5 ${ADDON_BUILD}/${PKG_ADDON_ID}/lib.private
    cp -L $(get_install_dir gpgme)/usr/lib/libgpgme.so.45 ${ADDON_BUILD}/${PKG_ADDON_ID}/lib.private
    cp -L $(get_install_dir json-glib)/usr/lib/libjson-glib-1.0.so.0 ${ADDON_BUILD}/${PKG_ADDON_ID}/lib.private
    cp -L $(get_install_dir ostree)/usr/lib/libostree-1.so.1 ${ADDON_BUILD}/${PKG_ADDON_ID}/lib.private

  mkdir -p ${ADDON_BUILD}/${PKG_ADDON_ID}/services
    for s in portal session-helper; do
      sed -e "s|/usr/lib/|/storage/.kodi/addons/${PKG_ADDON_ID}/bin/|" \
        ${PKG_INSTALL}/usr/lib/systemd/user/flatpak-$s.service \
        > ${ADDON_BUILD}/${PKG_ADDON_ID}/services/flatpak-$s.service
    done

    for s in Flatpak portal.Flatpak; do
      sed -e "s|/usr/lib/|/storage/.kodi/addons/${PKG_ADDON_ID}/bin/|" \
        ${PKG_INSTALL}/usr/share/dbus-1/services/org.freedesktop.$s.service \
        > ${ADDON_BUILD}/${PKG_ADDON_ID}/services/org.freedesktop.$s.service
     done
}
