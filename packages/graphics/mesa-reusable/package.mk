# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2025-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="mesa-reusable"
PKG_LICENSE="MIT"
PKG_SITE="http://www.mesa3d.org/"
PKG_DEPENDS_HOST="toolchain:host"
PKG_LONGDESC="Mesa is a 3-D graphics library with an API."
PKG_TOOLCHAIN="manual"

# mesa owns the naming and decides whether to depend on this package, and must
# never source it in turn
PKG_VERSION="$(get_pkg_variable mesa PKG_REUSABLE_VERSION)"
PKG_SOURCE_NAME="$(get_pkg_variable mesa PKG_REUSABLE_SOURCE_NAME)"

# only probe when asked to, and leave PKG_URL unset when there is nothing to get
# so that scripts/get does not retry a missing archive
if [ "${USE_REUSABLE}" = "yes" -o "${USE_REUSABLE}" = "preferred" ]; then
  PKG_URL="$(get_pkg_variable mesa PKG_REUSABLE_URL)"
  PKG_SHA256="$(get_reusable_sha256 ${PKG_NAME} ${PKG_SOURCE_NAME} ${PKG_URL})"
  if [ -z "${PKG_SHA256}" ]; then
    PKG_URL=""
  fi
fi

# neither the version nor the archive comes from this directory, so rebuild
# when mesa is bumped or a different archive is chosen
PKG_STAMP="${PKG_VERSION} ${PKG_SHA256}"

unpack() {
  [ -f "${SOURCES}/${PKG_NAME}/${PKG_SOURCE_NAME}" ] ||
    die "${PKG_SOURCE_NAME} is not available, set USE_REUSABLE=preferred or no"

  mkdir -p ${TOOLCHAIN}/bin
  tar -xf ${SOURCES}/${PKG_NAME}/${PKG_SOURCE_NAME} -C ${TOOLCHAIN}/bin
}
