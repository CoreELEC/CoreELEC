# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Team CoreELEC (https://coreelec.org)

PKG_NAME="inject_bl301"
PKG_VERSION="f71decac2828af4ce12852f995cc5bab16d10072"
PKG_SHA256="57f9b4c75b5aab03855ff2206169ef4a2d97efef02908aace70a21d419c56893"
PKG_LICENSE="proprietary"
PKG_SITE="https://coreelec.org"
PKG_URL="https://sources.coreelec.org/$PKG_NAME-$PKG_VERSION.tar.xz"
PKG_DEPENDS_TARGET="toolchain bl301"
PKG_LONGDESC="Tool to inject bootloader blob BL301.bin on internal eMMC"
PKG_TOOLCHAIN="manual"

pre_make_target() {
  cp -av ${PKG_DIR}/config/bl301.conf ${PKG_BUILD}/bl301.conf
  for f in $(find $(get_build_dir bl301) -mindepth 1 -name 'coreelec_config.c'); do
    cat ${f} | awk -F'[(),"]' '/.config_id_a\s*=\s*HASH/ {printf("%s %s\n", $2, $3)}' | \
      while read id name; do
        if ! grep -Fwq "${id}" ${PKG_BUILD}/bl301.conf; then
          echo -e '\n['${id}']' >> ${PKG_BUILD}/bl301.conf;
          cat ${f%.*}.h | awk -v id="HASHSTR_${id} " '$0 ~ id {printf("config_id=%s\n", $3)}' >> ${PKG_BUILD}/bl301.conf;
          echo -e "config_name=${name}" >> ${PKG_BUILD}/bl301.conf;
        fi
      done
  done
}

makeinstall_target() {
  mkdir -p ${INSTALL}/usr/sbin
  mkdir -p ${INSTALL}/usr/lib/coreelec
  mkdir -p ${INSTALL}/etc/inject_bl301
    install -m 0755 inject_bl301 ${INSTALL}/usr/sbin/inject_bl301
    install -m 0755 ${PKG_DIR}/scripts/check-bl301.sh ${INSTALL}/usr/lib/coreelec/check-bl301
    install -m 0755 ${PKG_DIR}/scripts/update-bl301.sh ${INSTALL}/usr/lib/coreelec/update-bl301
    install -m 0644 ${PKG_BUILD}/bl301.conf ${INSTALL}/etc/inject_bl301/bl301.conf
}

post_install() {
  enable_service update-bl301.service
}
