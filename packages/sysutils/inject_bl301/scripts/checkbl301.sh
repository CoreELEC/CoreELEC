#!/bin/sh
#
# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.org)
#
#  Detect BL301 injection
#
#####################################################
#
# Comand Line Arguments
# -v = Show verbose output
#
#####################################################

VERBOSE=0
INSTALLED=0

if [ "$1" = "-v" ]; then
  VERBOSE=1
fi

if [ -e /dev/bootloader ]; then
  dd if=/dev/bootloader bs=1M count=4 status=none | grep -aq COREELEC_BL301_BIN
  if [ ${?} = 0 ]; then
    INSTALLED=1
  fi
fi

if [ "$VERBOSE" = 1 ]; then
  if [ "$INSTALLED" = 1 ]; then
    echo "CoreELEC BL301 Installed"
  else
    echo "CoreELEC BL301 Not found"
  fi
fi
exit $INSTALLED
