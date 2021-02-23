#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2021-present Shanti Gilbert (https://github.com/shantigilbert)

# Source predefined functions and variables
. /etc/profile

DUKEDIR="/storage/.config/eduke32"
DUKECFG="${DUKEDIR}/eduke32.cfg"

mkdir -p "${DUKEDIR}"
if [ ! -f "${DUKECFG}" ]; then
# We only do these changes if the cfg file does not already exists, if it exists we asume the user has created it outside this script
# Only for handheld devices, SBCs already handle this gratefully
               
    if [ "$EE_DEVICE" == "OdroidGoAdvance" ] || [ "$EE_DEVICE" == "GameForce" ]; then
        touch "${DUKECFG}"
    
        case "$(oga_ver)" in
            "OGA" | "OGABE")
                echo "ScreenDisplay = 0" > "${DUKECFG}"
                echo "ScreenHeight = 320" >> "${DUKECFG}"
                echo "ScreenMode = 0" >> "${DUKECFG}"
                echo "ScreenWidth = 480" >> "${DUKECFG}"
                echo "MaxRefreshFreq = 0" >> "${DUKECFG}"
            ;;
            "GF")
                echo "ScreenDisplay = 0" > "${DUKECFG}"
                echo "ScreenHeight = 480" >> "${DUKECFG}"
                echo "ScreenMode = 0" >> "${DUKECFG}"
                echo "ScreenWidth = 640" >> "${DUKECFG}"
                echo "MaxRefreshFreq = 0" >> "${DUKECFG}"
            ;;
            "OGS")
                echo "ScreenDisplay = 0" > "${DUKECFG}"
                echo "ScreenHeight = 480" >> "${DUKECFG}"
                echo "ScreenMode = 0" >> "${DUKECFG}"
                echo "ScreenWidth = 854" >> "${DUKECFG}"
                echo "MaxRefreshFreq = 0" >> "${DUKECFG}"
            ;;
        esac
    fi
fi

cd /storage/roms/ports/eduke
eduke32
