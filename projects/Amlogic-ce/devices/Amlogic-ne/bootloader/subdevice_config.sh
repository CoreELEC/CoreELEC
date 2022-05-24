#!/bin/sh
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2022-present Team CoreELEC (https://coreelec.org)

unset DEVICE_CFGLOAD
unset DEVICE_BOOT_LOGO
unset DEVICE_TIMEOUT_LOGO
unset DEVICE_DTB
unset DEVICE_UBOOT
unset DEVICE_CHAIN_UBOOT
unset DEVICE_BOOT_INI
unset DEVICE_BOOT_SCR
unset DEVICE_UBOOT_BIN
unset DEVICE_CHAIN_UBOOT_BIN

case ${1} in
  Generic)
    DEVICE_CFGLOAD="${1}_cfgload"
  ;;
esac
