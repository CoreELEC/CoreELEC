# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2009-2016 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2016-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="alsa-utils"
PKG_VERSION="1.2.16"
PKG_SHA256="092399d5e8749a1d5e188e393157521cec4b75693b60ebb79bbce728cff2232c"
PKG_LICENSE="GPL-2.0-or-later"
PKG_SITE="https://www.alsa-project.org/"
PKG_URL="https://www.alsa-project.org/files/pub/utils/alsa-utils-${PKG_VERSION}.tar.bz2"
PKG_DEPENDS_TARGET="toolchain alsa-lib ncurses systemd"
PKG_LONGDESC="This package includes the utilities for ALSA, like alsamixer, aplay, arecord, alsactl, iecset and speaker-test."
PKG_TOOLCHAIN="autotools"

PKG_CONFIGURE_OPTS_TARGET="--disable-alsaconf \
                           --disable-alsaloop \
                           --enable-alsatest \
                           --disable-bat \
                           --disable-dependency-tracking \
                           --disable-nls \
                           --disable-rst2man \
                           --disable-xmlto"

post_configure_target() {
  libtool_remove_rpath libtool
}

post_makeinstall_target() {
  rm -rf ${INSTALL}/lib ${INSTALL}/var
  rm -rf ${INSTALL}/usr/share/alsa/speaker-test
  rm -rf ${INSTALL}/usr/share/sounds
  rm -rf ${INSTALL}/usr/lib/systemd/system

  # remove default udev rule to restore mixer configs, we install our own.
  # so we avoid resetting our soundconfig
  rm -rf ${INSTALL}/usr/lib/udev/rules.d/90-alsa-restore.rules

  mkdir -p ${INSTALL}/.noinstall
  for i in aconnect amidi aplaymidi arecordmidi aseqdump aseqnet iecset; do
    mv ${INSTALL}/usr/bin/${i} ${INSTALL}/.noinstall
  done

  # temp fix until multimedia-tools addon is bumped
  cp ${INSTALL}/usr/bin/alsamixer ${INSTALL}/.noinstall

  mkdir -p ${INSTALL}/usr/lib/udev
    cp ${PKG_DIR}/scripts/soundconfig ${INSTALL}/usr/lib/udev
}
