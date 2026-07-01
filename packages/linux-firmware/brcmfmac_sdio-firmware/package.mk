# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

PKG_NAME="brcmfmac_sdio-firmware"
PKG_VERSION="5987820e4ff88a5626536f66257165fe3a781b73"
PKG_SHA256="764603490351a87fee8cf57ec6c880bb803d9cccd65d4f97e4aed300acfd84df"
PKG_LICENSE="LicenseRef-firmware-Broadcom"
PKG_SITE="https://github.com/LibreELEC/brcmfmac_sdio-firmware"
PKG_URL="https://github.com/LibreELEC/brcmfmac_sdio-firmware/archive/${PKG_VERSION}.tar.gz"
PKG_LONGDESC="Broadcom SDIO firmware used with LibreELEC"
PKG_TOOLCHAIN="manual"

post_makeinstall_target() {
  FW_TARGET_DIR=${INSTALL}/$(get_full_firmware_dir)

  if find_file_path firmwares/${PKG_NAME}.dat; then
    FW_LISTS="${FOUND_PATH}"
  else
    FW_LISTS="${PKG_DIR}/firmwares/any.dat ${PKG_DIR}/firmwares/${TARGET_ARCH}.dat"
  fi

  for fwlist in ${FW_LISTS}; do
    [ -f ${fwlist} ] || continue
    while read -r fwline; do
      [ -z "${fwline}" ] && continue
      [[ ${fwline} =~ ^#.* ]] && continue
      [[ ${fwline} =~ ^[[:space:]] ]] && continue

      for fwfile in $(cd ${PKG_BUILD} && eval "find ${fwline}"); do
        [ -d ${PKG_BUILD}/${fwfile} ] && continue
        if [ -f ${PKG_BUILD}/${fwfile} ]; then
          mkdir -p $(dirname ${FW_TARGET_DIR}/brcm/${fwfile})
            cp -Lv ${PKG_BUILD}/${fwfile} ${FW_TARGET_DIR}/brcm/${fwfile}
        else
          echo "ERROR: Firmware file ${fwfile} does not exist - aborting"
          exit 1
        fi
      done
    done <${fwlist}
  done

  mkdir -p ${INSTALL}/usr/bin
    cp ${PKG_DIR}/scripts/brcmfmac-firmware-setup ${INSTALL}/usr/bin
}

post_install() {
  enable_service brcmfmac-firmware.service
}
