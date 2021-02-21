#!/usr/bin/env bash

# This file is part of The RetroPie Project
#
# The RetroPie Project is the legal property of its developers, whose names are
# too numerous to list here. Please refer to the COPYRIGHT.md file distributed with this source.
#
# See the LICENSE.md file at the top-level directory of this distribution and
# at https://raw.githubusercontent.com/RetroPie/RetroPie-Setup/master/LICENSE.md
#

rp_module_id="reicast"
rp_module_desc="Dreamcast emulator Reicast"
rp_module_help="ROM Extensions: .cdi .gdi\n\nCopy your Dreamcast roms to $romdir/dreamcast\n\nCopy the required BIOS files dc_boot.bin and dc_flash.bin to $biosdir/dc"
rp_module_licence="GPL2 https://raw.githubusercontent.com/reicast/reicast-emulator/master/LICENSE"
rp_module_section="opt"

function input_reicast() {
    local temp_file="$(mktemp)"
    cd "/usr/bin"
    ./reicast-joyconfig.py -f "$temp_file" >/dev/tty
    iniConfig " = " "" "$temp_file"
    iniGet "mapping_name"
    local mapping_file="/storage/.config/reicast/mappings/controller_${ini_value// /}.cfg"
    mv "$temp_file" "$mapping_file"
    chown $user:$user "$mapping_file"
}

function gui_reicast() {
    while true; do
        local options=(
            1 "Configure input devices for Reicast"
        )
        local cmd=(dialog --ascii-lines --backtitle "$__backtitle" --menu "Choose an option" 22 76 16)
        local choice=$("${cmd[@]}" "${options[@]}" 2>&1 >/dev/tty)
        [[ -z "$choice" ]] && break
        case "$choice" in
            1)
                clear
                input_reicast
                ;;
        esac
    done
}
