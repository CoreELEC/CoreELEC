#!/bin/bash

#IFS=';'

# Welcome
 dialog --backtitle "The Bezel Project" --title "The Bezel Project - Bezel Pack Utility" \
    --yesno "\nThe Bezel Project Bezel Utility menu.\n\nThis utility will provide a downloader for Retroarach system bezel packs to be used for various systems within RetroPie.\n\nThese bezel packs will only work if the ROMs you are using are named according to the No-Intro naming convention used by EmuMovies/HyperSpin.\n\nThis utility provides a download for a bezel pack for a system and includes a PNG bezel file for every ROM for that system.  The download will also include the necessary configuration files needed for Retroarch to show them.  The script will also update the required retroarch.cfg files for the emulators located in the /opt/retropie/configs directory.  These changes are necessary to show the PNG bezels with an opacity of 1.\n\nPeriodically, new bezel packs are completed and you will need to run the script updater to download the newest version to see these additional packs.\n\n**NOTE**\nThe MAME bezel back is inclusive for any roms located in the arcade/fba/mame-libretro rom folders.\n\n\nDo you want to proceed?" \
    28 110 2>&1 > /dev/tty \
    || exit


function main_menu() {
    local choice

    while true; do
        choice=$(dialog --backtitle "$BACKTITLE" --title " MAIN MENU " \
            --ok-label OK --cancel-label Exit \
            --menu "What action would you like to perform?" 25 75 20 \
            1 "Download system bezel pack (will automatcally enable bezels)" \
            2 "Enable system bezel pack" \
            3 "Disable system bezel pack" \
            4 "Information:  Retroarch cores setup for bezels per system" \
            5 "Uninstall the bezel project completely" \
            2>&1 > /dev/tty)

        case "$choice" in
            1) download_bezel  ;;
            2) enable_bezel  ;;
            3) disable_bezel  ;;
            4) retroarch_bezelinfo  ;;
            5) removebezelproject  ;;
            *)  break ;;
        esac
    done
}

#########################################################
# Functions for download and enable/disable bezel packs #
#########################################################

function install_bezel_pack() {
    local theme="$1"
    local repo="$2"
    if [[ -z "$repo" ]]; then
        repo="default"
    fi
    if [[ -z "$theme" ]]; then
        theme="default"
        repo="default"
    fi
    atheme=`echo ${theme} | sed 's/.*/\L&/'`

    if [[ "${atheme}" == "mame" ]];then
      mv "/opt/retropie/configs/all/retroarch/config/disable_FB Alpha" "/opt/retropie/configs/all/retroarch/config/FB Alpha" 2> /dev/null
      mv "/opt/retropie/configs/all/retroarch/config/disable_MAME 2003" "/opt/retropie/configs/all/retroarch/config/MAME 2003" 2> /dev/null
      mv "/opt/retropie/configs/all/retroarch/config/disable_MAME 2003 (0.78)" "/opt/retropie/configs/all/retroarch/config/MAME 2003 (0.78)" 2> /dev/null
      mv "/opt/retropie/configs/all/retroarch/config/disable_MAME 2010" "/opt/retropie/configs/all/retroarch/config/MAME 2010" 2> /dev/null
    fi

    git clone "https://github.com/$repo/bezelproject-$theme.git" "/tmp/${theme}"
    cp -r "/tmp/${theme}/retroarch/" /opt/retropie/configs/all/
    sudo rm -rf "/tmp/${theme}"

    if [[ "${atheme}" == "mame" ]];then
      show_bezel "arcade"
      show_bezel "fba"
      show_bezel "mame-libretro"
    else
      show_bezel "${atheme}"
    fi
}

function uninstall_bezel_pack() {
    local theme="$1"
    if [[ -d "/opt/retropie/configs/all/retroarch/overlay/GameBezels/$theme" ]]; then
        rm -rf "/opt/retropie/configs/all/retroarch/overlay/GameBezels/$theme"
    fi
    if [[ "${theme}" == "MAME" ]]; then
      if [[ -d "/opt/retropie/configs/all/retroarch/overlay/ArcadeBezels" ]]; then
        rm -rf "/opt/retropie/configs/all/retroarch/overlay/ArcadeBezels"
      fi
    fi
}

function removebezelproject() {
hide_bezel vectrex
hide_bezel supergrafx
hide_bezel sega32x
hide_bezel sg-1000
hide_bezel arcade
hide_bezel fba
hide_bezel mame-libretro
hide_bezel nes
hide_bezel mastersystem
hide_bezel atari5200
hide_bezel atari7800
hide_bezel snes
hide_bezel megadrive
hide_bezel segacd
hide_bezel psx
hide_bezel tg16
hide_bezel tg-cd
hide_bezel atari2600
hide_bezel coleco
hide_bezel n64

rm -rf /opt/retropie/configs/all/retroarch/overlay/GameBezels
rm -rf /opt/retropie/configs/all/retroarch/overlay/ArcadeBezels
rm /home/pi/RetroPie/retropiemenu/bezelproject.sh

}

function download_bezel() {
    local themes=(
        'thebezelproject MAME'
        'thebezelproject Atari2600'
        'thebezelproject Atari5200'
        'thebezelproject Atari7800'
        'thebezelproject GCEVectrex'
        'thebezelproject MasterSystem'
        'thebezelproject MegaDrive'
        'thebezelproject N64'
        'thebezelproject NES'
        'thebezelproject Sega32X'
        'thebezelproject SegaCD'
        'thebezelproject SG-1000'
        'thebezelproject SNES'
        'thebezelproject SuperGrafx'
        'thebezelproject PSX'
        'thebezelproject TG16'
        'thebezelproject TG-CD'
        'thebezelproject ColecoVision'
    )
    while true; do
        local theme
        local installed_bezelpacks=()
        local repo
        local options=()
        local status=()
        local default

        options+=(U "Update install script - script will exit when updated")

        local i=1
        for theme in "${themes[@]}"; do
            theme=($theme)
            repo="${theme[0]}"
            theme="${theme[1]}"
            if [[ $theme == "MegaDrive" ]]; then
              theme="Megadrive"
            fi
            if [[ -d "/opt/retropie/configs/all/retroarch/overlay/GameBezels/$theme" ]]; then
                status+=("i")
                options+=("$i" "Update or Uninstall $theme (installed)")
                installed_bezelpacks+=("$theme $repo")
            else
                status+=("n")
                options+=("$i" "Install $theme (not installed)")
            fi
            ((i++))
        done
        local cmd=(dialog --default-item "$default" --backtitle "$__backtitle" --menu "The Bezel Project -  Bezel Pack Downloader - Choose an option" 22 76 16)
        local choice=$("${cmd[@]}" "${options[@]}" 2>&1 >/dev/tty)
        default="$choice"
        [[ -z "$choice" ]] && break
        case "$choice" in
            U)  #update install script to get new theme listings
                if [[ -d "/home/pigaming" ]]; then
                    cd "/home/pigaming/RetroPie/retropiemenu"
                else
                    cd "/home/pi/RetroPie/retropiemenu" 
                fi
                mv "bezelproject.sh" "bezelproject.sh.bkp" 
                wget "https://raw.githubusercontent.com/thebezelproject/BezelProject/master/bezelproject.sh" 
                chmod 777 "bezelproject.sh" 
                exit
                ;;
            *)  #install or update themes
                theme=(${themes[choice-1]})
                repo="${theme[0]}"
                theme="${theme[1]}"
#                if [[ "${status[choice]}" == "i" ]]; then
                if [[ -d "/opt/retropie/configs/all/retroarch/overlay/GameBezels/$theme" ]]; then
                    options=(1 "Update $theme" 2 "Uninstall $theme")
                    cmd=(dialog --backtitle "$__backtitle" --menu "Choose an option for the bezel pack" 12 40 06)
                    local choice=$("${cmd[@]}" "${options[@]}" 2>&1 >/dev/tty)
                    case "$choice" in
                        1)
                            install_bezel_pack "$theme" "$repo"
                            ;;
                        2)
                            uninstall_bezel_pack "$theme"
                            ;;
                    esac
                else
                    install_bezel_pack "$theme" "$repo"
                fi
                ;;
        esac
    done
}


function disable_bezel() {

clear
    while true; do
        choice=$(dialog --backtitle "$BACKTITLE" --title " MAIN MENU " \
            --ok-label OK --cancel-label Exit \
            --menu "Which system would you like to disable bezels for?" 25 75 20 \
            1 "GCEVectrex" \
            2 "SuperGrafx" \
            3 "Sega32X" \
            4 "SG-1000" \
            5 "Arcade" \
            6 "Final Burn Alpha" \
            7 "MAME Libretro" \
            8 "NES" \
            9 "MasterSystem" \
            10 "Atari 5200" \
            11 "Atari 7800" \
            12 "SNES" \
            13 "MegaDrive" \
            14 "SegaCD" \
            15 "PSX" \
            16 "TG16" \
            17 "TG-CD" \
            18 "Atari 2600" \
            19 "ColecoVision" \
            20 "Nintendo 64" \
            2>&1 > /dev/tty)

        case "$choice" in
            1) hide_bezel vectrex ;;
            2) hide_bezel supergrafx ;;
            3) hide_bezel sega32x ;;
            4) hide_bezel sg-1000 ;;
            5) hide_bezel arcade ;;
            6) hide_bezel fba ;;
            7) hide_bezel mame-libretro ;;
            8) hide_bezel nes ;;
            9) hide_bezel mastersystem ;;
            10) hide_bezel atari5200 ;;
            11) hide_bezel atari7800 ;;
            12) hide_bezel snes ;;
            13) hide_bezel megadrive ;;
            14) hide_bezel segacd ;;
            15) hide_bezel psx ;;
            16) hide_bezel tg16 ;;
            17) hide_bezel tg-cd ;;
            18) hide_bezel atari2600 ;;
            19) hide_bezel coleco ;;
            20) hide_bezel n64 ;;
            *)  break ;;
        esac
    done

}

function enable_bezel() {

clear
    while true; do
        choice=$(dialog --backtitle "$BACKTITLE" --title " MAIN MENU " \
            --ok-label OK --cancel-label Exit \
            --menu "Which system would you like to enable bezels for?" 25 75 20 \
            1 "GCEVectrex" \
            2 "SuperGrafx" \
            3 "Sega32X" \
            4 "SG-1000" \
            5 "Arcade" \
            6 "Final Burn Alpha" \
            7 "MAME Libretro" \
            8 "NES" \
            9 "MasterSystem" \
            10 "Atari 5200" \
            11 "Atari 7800" \
            12 "SNES" \
            13 "MegaDrive" \
            14 "SegaCD" \
            15 "PSX" \
            16 "TG16" \
            17 "TG-CD" \
            18 "Atari 2600" \
            19 "ColecoVision" \
            20 "Nintendo 64" \
            2>&1 > /dev/tty)

        case "$choice" in
            1) show_bezel gcevectrex ;;
            2) show_bezel supergrafx ;;
            3) show_bezel sega32x ;;
            4) show_bezel sg-1000 ;;
            5) show_bezel arcade ;;
            6) show_bezel fba ;;
            7) show_bezel mame-libretro ;;
            8) show_bezel nes ;;
            9) show_bezel mastersystem ;;
            10) show_bezel atari5200 ;;
            11) show_bezel atari7800 ;;
            12) show_bezel snes ;;
            13) show_bezel megadrive ;;
            14) show_bezel segacd ;;
            15) show_bezel psx ;;
            16) show_bezel tg16 ;;
            17) show_bezel tg-cd ;;
            18) show_bezel atari2600 ;;
            19) show_bezel coleco ;;
            20) show_bezel n64 ;;
            *)  break ;;
        esac
    done

}

function hide_bezel() {
dialog --infobox "...processing..." 3 20 ; sleep 2
emulator=$1
file="/opt/retropie/configs/${emulator}/retroarch.cfg"

case ${emulator} in
arcade)
  cp /opt/retropie/configs/${emulator}/retroarch.cfg /opt/retropie/configs/${emulator}/retroarch.cfg.bkp
  cat /opt/retropie/configs/${emulator}/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
  cp /tmp/retroarch.cfg /opt/retropie/configs/${emulator}/retroarch.cfg
  mv "/opt/retropie/configs/all/retroarch/config/FB Alpha" "/opt/retropie/configs/all/retroarch/config/disable_FB Alpha"
  mv "/opt/retropie/configs/all/retroarch/config/MAME 2003" "/opt/retropie/configs/all/retroarch/config/disable_MAME 2003"
  mv "/opt/retropie/configs/all/retroarch/config/MAME 2003 (0.78)" "/opt/retropie/configs/all/retroarch/config/disable_MAME 2003 (0.78)"
  mv "/opt/retropie/configs/all/retroarch/config/MAME 2010" "/opt/retropie/configs/all/retroarch/config/disable_MAME 2010"
  ;;
fba)
  cp /opt/retropie/configs/${emulator}/retroarch.cfg /opt/retropie/configs/${emulator}/retroarch.cfg.bkp
  cat /opt/retropie/configs/${emulator}/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
  cp /tmp/retroarch.cfg /opt/retropie/configs/${emulator}/retroarch.cfg
  mv "/opt/retropie/configs/all/retroarch/config/FB Alpha" "/opt/retropie/configs/all/retroarch/config/disable_FB Alpha"
  ;;
mame-libretro)
  cp /opt/retropie/configs/${emulator}/retroarch.cfg /opt/retropie/configs/${emulator}/retroarch.cfg.bkp
  cat /opt/retropie/configs/${emulator}/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
  cp /tmp/retroarch.cfg /opt/retropie/configs/${emulator}/retroarch.cfg
  mv "/opt/retropie/configs/all/retroarch/config/MAME 2003" "/opt/retropie/configs/all/retroarch/config/disable_MAME 2003"
  mv "/opt/retropie/configs/all/retroarch/config/MAME 2003 (0.78)" "/opt/retropie/configs/all/retroarch/config/disable_MAME 2003 (0.78)"
  mv "/opt/retropie/configs/all/retroarch/config/MAME 2010" "/opt/retropie/configs/all/retroarch/config/disable_MAME 2010"
  ;;
*)
  cp /opt/retropie/configs/${emulator}/retroarch.cfg /opt/retropie/configs/${emulator}/retroarch.cfg.bkp
  cat /opt/retropie/configs/${emulator}/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
  cp /tmp/retroarch.cfg /opt/retropie/configs/${emulator}/retroarch.cfg
  ;;
esac

}

function show_bezel() {
dialog --infobox "...processing..." 3 20 ; sleep 2
emulator=$1
file="/opt/retropie/configs/${emulator}/retroarch.cfg"

case ${emulator} in
arcade)
  ifexist=`cat /opt/retropie/configs/arcade/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/arcade/retroarch.cfg /opt/retropie/configs/arcade/retroarch.cfg.bkp
    cat /opt/retropie/configs/arcade/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/arcade/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/MAME-Horizontal.cfg"' /opt/retropie/configs/arcade/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/arcade/retroarch.cfg
    mv "/opt/retropie/configs/all/retroarch/config/disable_FB Alpha" "/opt/retropie/configs/all/retroarch/config/FB Alpha"
    mv "/opt/retropie/configs/all/retroarch/config/disable_MAME 2003" "/opt/retropie/configs/all/retroarch/config/MAME 2003"
    mv "/opt/retropie/configs/all/retroarch/config/disable_MAME 2003 (0.78)" "/opt/retropie/configs/all/retroarch/config/MAME 2003 (0.78)"
    mv "/opt/retropie/configs/all/retroarch/config/disable_MAME 2010" "/opt/retropie/configs/all/retroarch/config/MAME 2010"
  else
    cp /opt/retropie/configs/arcade/retroarch.cfg /opt/retropie/configs/arcade/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/MAME-Horizontal.cfg"' /opt/retropie/configs/arcade/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/arcade/retroarch.cfg
    mv "/opt/retropie/configs/all/retroarch/config/disable_FB Alpha" "/opt/retropie/configs/all/retroarch/config/FB Alpha"
    mv "/opt/retropie/configs/all/retroarch/config/disable_MAME 2003" "/opt/retropie/configs/all/retroarch/config/MAME 2003"
    mv "/opt/retropie/configs/all/retroarch/config/disable_MAME 2003 (0.78)" "/opt/retropie/configs/all/retroarch/config/MAME 2003 (0.78)"
    mv "/opt/retropie/configs/all/retroarch/config/disable_MAME 2010" "/opt/retropie/configs/all/retroarch/config/MAME 2010"
  fi
  ;;
fba)
  ifexist=`cat /opt/retropie/configs/fba/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/fba/retroarch.cfg /opt/retropie/configs/fba/retroarch.cfg.bkp
    cat /opt/retropie/configs/fba/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/fba/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/MAME-Horizontal.cfg"' /opt/retropie/configs/fba/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/fba/retroarch.cfg
    mv "/opt/retropie/configs/all/retroarch/config/disable_FB Alpha" "/opt/retropie/configs/all/retroarch/config/FB Alpha"
  else
    cp /opt/retropie/configs/fba/retroarch.cfg /opt/retropie/configs/fba/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/MAME-Horizontal.cfg"' /opt/retropie/configs/fba/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/fba/retroarch.cfg
    mv "/opt/retropie/configs/all/retroarch/config/disable_FB Alpha" "/opt/retropie/configs/all/retroarch/config/FB Alpha"
  fi
  ;;
mame-libretro)
  ifexist=`cat /opt/retropie/configs/mame-libretro/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/mame-libretro/retroarch.cfg /opt/retropie/configs/mame-libretro/retroarch.cfg.bkp
    cat /opt/retropie/configs/mame-libretro/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/mame-libretro/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/MAME-Horizontal.cfg"' /opt/retropie/configs/mame-libretro/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/mame-libretro/retroarch.cfg
    mv "/opt/retropie/configs/all/retroarch/config/disable_MAME 2003" "/opt/retropie/configs/all/retroarch/config/MAME 2003"
    mv "/opt/retropie/configs/all/retroarch/config/disable_MAME 2003 (0.78)" "/opt/retropie/configs/all/retroarch/config/MAME 2003 (0.78)"
    mv "/opt/retropie/configs/all/retroarch/config/disable_MAME 2010" "/opt/retropie/configs/all/retroarch/config/MAME 2010"
  else
    cp /opt/retropie/configs/mame-libretro/retroarch.cfg /opt/retropie/configs/mame-libretro/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/MAME-Horizontal.cfg"' /opt/retropie/configs/mame-libretro/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/mame-libretro/retroarch.cfg
    mv "/opt/retropie/configs/all/retroarch/config/disable_MAME 2003" "/opt/retropie/configs/all/retroarch/config/MAME 2003"
    mv "/opt/retropie/configs/all/retroarch/config/disable_MAME 2003 (0.78)" "/opt/retropie/configs/all/retroarch/config/MAME 2003 (0.78)"
    mv "/opt/retropie/configs/all/retroarch/config/disable_MAME 2010" "/opt/retropie/configs/all/retroarch/config/MAME 2010"
  fi
  ;;
atari2600)
  ifexist=`cat /opt/retropie/configs/atari2600/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/atari2600/retroarch.cfg /opt/retropie/configs/atari2600/retroarch.cfg.bkp
    cat /opt/retropie/configs/atari2600/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/atari2600/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Atari-2600.cfg"' /opt/retropie/configs/atari2600/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/atari2600/retroarch.cfg
  else
    cp /opt/retropie/configs/atari2600/retroarch.cfg /opt/retropie/configs/atari2600/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Atari-2600.cfg"' /opt/retropie/configs/atari2600/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/atari2600/retroarch.cfg
  fi
  ;;
atari5200)
  ifexist=`cat /opt/retropie/configs/atari5200/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/atari5200/retroarch.cfg /opt/retropie/configs/atari5200/retroarch.cfg.bkp
    cat /opt/retropie/configs/atari5200/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/atari5200/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Atari-5200.cfg"' /opt/retropie/configs/atari5200/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/atari5200/retroarch.cfg
  else
    cp /opt/retropie/configs/atari5200/retroarch.cfg /opt/retropie/configs/atari5200/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Atari-5200.cfg"' /opt/retropie/configs/atari5200/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/atari5200/retroarch.cfg
  fi
  ;;
atari7800)
  ifexist=`cat /opt/retropie/configs/atari7800/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/atari7800/retroarch.cfg /opt/retropie/configs/atari7800/retroarch.cfg.bkp
    cat /opt/retropie/configs/atari7800/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/atari7800/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Atari-7800.cfg"' /opt/retropie/configs/atari7800/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/atari7800/retroarch.cfg
  else
    cp /opt/retropie/configs/atari7800/retroarch.cfg /opt/retropie/configs/atari7800/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Atari-7800.cfg"' /opt/retropie/configs/atari7800/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/atari7800/retroarch.cfg
  fi
  ;;
coleco)
  ifexist=`cat /opt/retropie/configs/coleco/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/coleco/retroarch.cfg /opt/retropie/configs/coleco/retroarch.cfg.bkp
    cat /opt/retropie/configs/coleco/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/coleco/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Colecovision.cfg"' /opt/retropie/configs/coleco/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/coleco/retroarch.cfg
  else
    cp /opt/retropie/configs/coleco/retroarch.cfg /opt/retropie/configs/coleco/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Colecovision.cfg"' /opt/retropie/configs/coleco/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/coleco/retroarch.cfg
  fi
  ;;
famicom)
  ifexist=`cat /opt/retropie/configs/famicom/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/famicom/retroarch.cfg /opt/retropie/configs/famicom/retroarch.cfg.bkp
    cat /opt/retropie/configs/famicom/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/famicom/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Famicom.cfg"' /opt/retropie/configs/famicom/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/famicom/retroarch.cfg
  else
    cp /opt/retropie/configs/famicom/retroarch.cfg /opt/retropie/configs/famicom/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Famicom.cfg"' /opt/retropie/configs/famicom/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/famicom/retroarch.cfg
  fi
  ;;
fds)
  ifexist=`cat /opt/retropie/configs/fds/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/fds/retroarch.cfg /opt/retropie/configs/fds/retroarch.cfg.bkp
    cat /opt/retropie/configs/fds/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/fds/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Famicom-Disk-System.cfg"' /opt/retropie/configs/fds/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/fds/retroarch.cfg
  else
    cp /opt/retropie/configs/fds/retroarch.cfg /opt/retropie/configs/fds/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Famicom-Disk-System.cfg"' /opt/retropie/configs/fds/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/fds/retroarch.cfg
  fi
  ;;
mastersystem)
  ifexist=`cat /opt/retropie/configs/mastersystem/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/mastersystem/retroarch.cfg /opt/retropie/configs/mastersystem/retroarch.cfg.bkp
    cat /opt/retropie/configs/mastersystem/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/mastersystem/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sega-Master-System.cfg"' /opt/retropie/configs/mastersystem/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/mastersystem/retroarch.cfg
  else
    cp /opt/retropie/configs/mastersystem/retroarch.cfg /opt/retropie/configs/mastersystem/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sega-Master-System.cfg"' /opt/retropie/configs/mastersystem/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/mastersystem/retroarch.cfg
  fi
  ;;
megadrive)
  ifexist=`cat /opt/retropie/configs/megadrive/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/megadrive/retroarch.cfg /opt/retropie/configs/megadrive/retroarch.cfg.bkp
    cat /opt/retropie/configs/megadrive/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/megadrive/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sega-Mega-Drive.cfg"' /opt/retropie/configs/megadrive/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/megadrive/retroarch.cfg
  else
    cp /opt/retropie/configs/megadrive/retroarch.cfg /opt/retropie/configs/megadrive/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sega-Mega-Drive.cfg"' /opt/retropie/configs/megadrive/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/megadrive/retroarch.cfg
  fi
  ;;
megadrive-japan)
  ifexist=`cat /opt/retropie/configs/megadrive-japan/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/megadrive-japan/retroarch.cfg /opt/retropie/configs/megadrive-japan/retroarch.cfg.bkp
    cat /opt/retropie/configs/megadrive-japan/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/megadrive-japan/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sega-Mega-Drive-Japan.cfg"' /opt/retropie/configs/megadrive-japan/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/megadrive-japan/retroarch.cfg
  else
    cp /opt/retropie/configs/megadrive-japan/retroarch.cfg /opt/retropie/configs/megadrive-japan/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sega-Mega-Drive-Japan.cfg"' /opt/retropie/configs/megadrive-japan/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/megadrive-japan/retroarch.cfg
  fi
  ;;
n64)
  ifexist=`cat /opt/retropie/configs/n64/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/n6n64/retroarch.cfg /opt/retropie/configs/n64/retroarch.cfg.bkp
    cat /opt/retropie/configs/n6/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/n64/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-64.cfg"' /opt/retropie/configs/n64/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/n64/retroarch.cfg
  else
    cp /opt/retropie/configs/n64/retroarch.cfg /opt/retropie/configs/n64/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-64.cfg"' /opt/retropie/configs/n64/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/n64/retroarch.cfg
  fi
  ;;
neogeo)
  ifexist=`cat /opt/retropie/configs/neogeo/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/neogeo/retroarch.cfg /opt/retropie/configs/neogeo/retroarch.cfg.bkp
    cat /opt/retropie/configs/neogeo/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/neogeo/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/MAME-Horizontal.cfg"' /opt/retropie/configs/neogeo/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/neogeo/retroarch.cfg
  else
    cp /opt/retropie/configs/neogeo/retroarch.cfg /opt/retropie/configs/neogeo/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/MAME-Horizontal.cfg"' /opt/retropie/configs/neogeo/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/neogeo/retroarch.cfg
  fi
  ;;
nes)
  ifexist=`cat /opt/retropie/configs/nes/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/nes/retroarch.cfg /opt/retropie/configs/nes/retroarch.cfg.bkp
    cat /opt/retropie/configs/nes/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport |grep -v force_aspect > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/nes/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Entertainment-System.cfg"' /opt/retropie/configs/nes/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/nes/retroarch.cfg
    sed -i '4i aspect_ratio_index = "16"' /opt/retropie/configs/nes/retroarch.cfg
    sed -i '5i video_force_aspect = "true"' /opt/retropie/configs/nes/retroarch.cfg
    sed -i '6i video_aspect_ratio = "-1.000000"' /opt/retropie/configs/nes/retroarch.cfg
  else
    cp /opt/retropie/configs/nes/retroarch.cfg /opt/retropie/configs/nes/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Entertainment-System.cfg"' /opt/retropie/configs/nes/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/nes/retroarch.cfg
    sed -i '4i aspect_ratio_index = "16"' /opt/retropie/configs/nes/retroarch.cfg
    sed -i '5i video_force_aspect = "true"' /opt/retropie/configs/nes/retroarch.cfg
    sed -i '6i video_aspect_ratio = "-1.000000"' /opt/retropie/configs/nes/retroarch.cfg
  fi
  ;;
pce-cd)
  ifexist=`cat /opt/retropie/configs/pce-cd/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/pce-cd/retroarch.cfg /opt/retropie/configs/pce-cd/retroarch.cfg.bkp
    cat /opt/retropie/configs/pce-cd/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/pce-cd/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/NEC-PC-Engine-CD.cfg"' /opt/retropie/configs/pce-cd/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/pce-cd/retroarch.cfg
  else
    cp /opt/retropie/configs/pce-cd/retroarch.cfg /opt/retropie/configs/pce-cd/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/NEC-PC-Engine-CD.cfg"' /opt/retropie/configs/pce-cd/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/pce-cd/retroarch.cfg
  fi
  ;;
pcengine)
  ifexist=`cat /opt/retropie/configs/pcengine/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/pcengine/retroarch.cfg /opt/retropie/configs/pcengine/retroarch.cfg.bkp
    cat /opt/retropie/configs/pcengine/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/pcengine/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/NEC-PC-Engine.cfg"' /opt/retropie/configs/pcengine/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/pcengine/retroarch.cfg
  else
    cp /opt/retropie/configs/pcengine/retroarch.cfg /opt/retropie/configs/pcengine/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/NEC-PC-Engine.cfg"' /opt/retropie/configs/pcengine/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/pcengine/retroarch.cfg
  fi
  ;;
psx)
  ifexist=`cat /opt/retropie/configs/psx/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/psx/retroarch.cfg /opt/retropie/configs/psx/retroarch.cfg.bkp
    cat /opt/retropie/configs/psx/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/psx/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sony-PlayStation.cfg"' /opt/retropie/configs/psx/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/psx/retroarch.cfg
  else
    cp /opt/retropie/configs/psx/retroarch.cfg /opt/retropie/configs/psx/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sony-PlayStation.cfg"' /opt/retropie/configs/psx/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/psx/retroarch.cfg
  fi
  ;;
sega32x)
  ifexist=`cat /opt/retropie/configs/sega32x/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/sega32x/retroarch.cfg /opt/retropie/configs/sega32x/retroarch.cfg.bkp
    cat /opt/retropie/configs/sega32x/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/sega32x/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sega-32X.cfg"' /opt/retropie/configs/sega32x/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/sega32x/retroarch.cfg
  else
    cp /opt/retropie/configs/sega32x/retroarch.cfg /opt/retropie/configs/sega32x/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sega-32X.cfg"' /opt/retropie/configs/sega32x/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/sega32x/retroarch.cfg
  fi
  ;;
segacd)
  ifexist=`cat /opt/retropie/configs/segacd/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/segacd/retroarch.cfg /opt/retropie/configs/segacd/retroarch.cfg.bkp
    cat /opt/retropie/configs/segacd/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/segacd/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sega-CD.cfg"' /opt/retropie/configs/segacd/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/segacd/retroarch.cfg
  else
    cp /opt/retropie/configs/segacd/retroarch.cfg /opt/retropie/configs/segacd/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sega-CD.cfg"' /opt/retropie/configs/segacd/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/segacd/retroarch.cfg
  fi
  ;;
sfc)
  ifexist=`cat /opt/retropie/configs/sfc/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/sfc/retroarch.cfg /opt/retropie/configs/sfc/retroarch.cfg.bkp
    cat /opt/retropie/configs/sfc/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/sfc/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Super-Famicom.cfg"' /opt/retropie/configs/sfc/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/sfc/retroarch.cfg
  else
    cp /opt/retropie/configs/sfc/retroarch.cfg /opt/retropie/configs/sfc/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Super-Famicom.cfg"' /opt/retropie/configs/sfc/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/sfc/retroarch.cfg
  fi
  ;;
sg-1000)
  ifexist=`cat /opt/retropie/configs/sg-1000/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/sg-1000/retroarch.cfg /opt/retropie/configs/sg-1000/retroarch.cfg.bkp
    cat /opt/retropie/configs/sg-1000/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/sg-1000/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sega-SG-1000.cfg"' /opt/retropie/configs/sg-1000/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/sg-1000/retroarch.cfg
  else
    cp /opt/retropie/configs/sg-1000/retroarch.cfg /opt/retropie/configs/sg-1000/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sega-SG-1000.cfg"' /opt/retropie/configs/sg-1000/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/sg-1000/retroarch.cfg
  fi
  ;;
snes)
  ifexist=`cat /opt/retropie/configs/snes/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/snes/retroarch.cfg /opt/retropie/configs/snes/retroarch.cfg.bkp
    cat /opt/retropie/configs/snes/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/snes/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Super-Nintendo-Entertainment-System.cfg"' /opt/retropie/configs/snes/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/snes/retroarch.cfg
  else
    cp /opt/retropie/configs/snes/retroarch.cfg /opt/retropie/configs/snes/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Super-Nintendo-Entertainment-System.cfg"' /opt/retropie/configs/snes/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/snes/retroarch.cfg
  fi
  ;;
supergrafx)
  ifexist=`cat /opt/retropie/configs/supergrafx/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/supergrafx/retroarch.cfg /opt/retropie/configs/supergrafx/retroarch.cfg.bkp
    cat /opt/retropie/configs/supergrafx/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/supergrafx/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/NEC-SuperGrafx.cfg"' /opt/retropie/configs/supergrafx/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/supergrafx/retroarch.cfg
  else
    cp /opt/retropie/configs/supergrafx/retroarch.cfg /opt/retropie/configs/supergrafx/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/NEC-SuperGrafx.cfg"' /opt/retropie/configs/supergrafx/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/supergrafx/retroarch.cfg
  fi
  ;;
tg16)
  ifexist=`cat /opt/retropie/configs/tg16/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/tg16/retroarch.cfg /opt/retropie/configs/tg16/retroarch.cfg.bkp
    cat /opt/retropie/configs/tg16/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/tg16/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/NEC-TurboGrafx-16.cfg"' /opt/retropie/configs/tg16/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/tg16/retroarch.cfg
  else
    cp /opt/retropie/configs/tg16/retroarch.cfg /opt/retropie/configs/tg16/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/NEC-TurboGrafx-16.cfg"' /opt/retropie/configs/tg16/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/tg16/retroarch.cfg
  fi
  ;;
tg-cd)
  ifexist=`cat /opt/retropie/configs/tg-cd/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/tg-cd/retroarch.cfg /opt/retropie/configs/tg-cd/retroarch.cfg.bkp
    cat /opt/retropie/configs/tg-cd/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/tg-cd/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/NEC-TurboGrafx-CD.cfg"' /opt/retropie/configs/tg-cd/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/tg-cd/retroarch.cfg
  else
    cp /opt/retropie/configs/tg-cd/retroarch.cfg /opt/retropie/configs/tg-cd/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/NEC-TurboGrafx-CD.cfg"' /opt/retropie/configs/tg-cd/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/tg-cd/retroarch.cfg
  fi
  ;;
gcevectrex)
  ifexist=`cat /opt/retropie/configs/vectrex/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/vectrex/retroarch.cfg /opt/retropie/configs/vectrex/retroarch.cfg.bkp
    cat /opt/retropie/configs/vectrex/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/vectrex/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/GCE-Vectrex.cfg"' /opt/retropie/configs/vectrex/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/vectrex/retroarch.cfg
  else
    cp /opt/retropie/configs/vectrex/retroarch.cfg /opt/retropie/configs/vectrex/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/GCE-Vectrex.cfg"' /opt/retropie/configs/vectrex/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/vectrex/retroarch.cfg
  fi
  ;;
atarilynx_1080)
  ifexist=`cat /opt/retropie/configs/atarilynx/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/atarilynx/retroarch.cfg /opt/retropie/configs/atarilynx/retroarch.cfg.bkp
    cat /opt/retropie/configs/atarilynx/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Atari-Lynx-Horizontal.cfg"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '5i custom_viewport_width = "1010"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '6i custom_viewport_height = "640"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '7i custom_viewport_x = "455"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '8i custom_viewport_y = "225"' /opt/retropie/configs/atarilynx/retroarch.cfg
  else
    cp /opt/retropie/configs/atarilynx/retroarch.cfg /opt/retropie/configs/atarilynx/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Atari-Lynx-Horizontal.cfg"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '5i custom_viewport_width = "1010"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '6i custom_viewport_height = "640"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '7i custom_viewport_x = "455"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '8i custom_viewport_y = "225"' /opt/retropie/configs/atarilynx/retroarch.cfg
  fi  
  ;;  
atarilynx_720)
  ifexist=`cat /opt/retropie/configs/atarilynx/retroarch.cfg |grep "input_overlay" |wc -l` 
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/atarilynx/retroarch.cfg /opt/retropie/configs/atarilynx/retroarch.cfg.bkp
    cat /opt/retropie/configs/atarilynx/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Atari-Lynx-Horizontal.cfg"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '5i custom_viewport_width = "670"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '6i custom_viewport_height = "425"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '7i custom_viewport_x = "305"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '8i custom_viewport_y = "150"' /opt/retropie/configs/atarilynx/retroarch.cfg
  else
    cp /opt/retropie/configs/atarilynx/retroarch.cfg /opt/retropie/configs/atarilynx/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Atari-Lynx-Horizontal.cfg"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '5i custom_viewport_width = "670"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '6i custom_viewport_height = "425"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '7i custom_viewport_x = "305"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '8i custom_viewport_y = "150"' /opt/retropie/configs/atarilynx/retroarch.cfg
  fi  
  ;;
atarilynx_other)
  ifexist=`cat /opt/retropie/configs/atarilynx/retroarch.cfg |grep "input_overlay" |wc -l` 
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/atarilynx/retroarch.cfg /opt/retropie/configs/atarilynx/retroarch.cfg.bkp
    cat /opt/retropie/configs/atarilynx/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Atari-Lynx-Horizontal.cfg"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '5i custom_viewport_width = "715"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '6i custom_viewport_height = "460"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '7i custom_viewport_x = "325"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '8i custom_viewport_y = "160"' /opt/retropie/configs/atarilynx/retroarch.cfg
  else
    cp /opt/retropie/configs/atarilynx/retroarch.cfg /opt/retropie/configs/atarilynx/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Atari-Lynx-Horizontal.cfg"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '5i custom_viewport_width = "715"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '6i custom_viewport_height = "460"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '7i custom_viewport_x = "325"' /opt/retropie/configs/atarilynx/retroarch.cfg
    sed -i '8i custom_viewport_y = "160"' /opt/retropie/configs/atarilynx/retroarch.cfg
  fi
  ;;
gamegear_1080)
  ifexist=`cat /opt/retropie/configs/gamegear/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/gamegear/retroarch.cfg /opt/retropie/configs/gamegear/retroarch.cfg.bkp
    cat /opt/retropie/configs/gamegear/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sega-Game-Gear.cfg"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '5i custom_viewport_width = "1160"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '6i custom_viewport_height = "850"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '7i custom_viewport_x = "380"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '8i custom_viewport_y = "120"' /opt/retropie/configs/gamegear/retroarch.cfg
  else
    cp /opt/retropie/configs/gamegear/retroarch.cfg /opt/retropie/configs/gamegear/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sega-Game-Gear.cfg"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '5i custom_viewport_width = "1160"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '6i custom_viewport_height = "850"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '7i custom_viewport_x = "380"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '8i custom_viewport_y = "120"' /opt/retropie/configs/gamegear/retroarch.cfg
  fi  
  ;;  
gamegear_720)
  ifexist=`cat /opt/retropie/configs/gamegear/retroarch.cfg |grep "input_overlay" |wc -l` 
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/gamegear/retroarch.cfg /opt/retropie/configs/gamegear/retroarch.cfg.bkp
    cat /opt/retropie/configs/gamegear/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sega-Game-Gear.cfg"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '5i custom_viewport_width = "780"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '6i custom_viewport_height = "580"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '7i custom_viewport_x = "245"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '8i custom_viewport_y = "70"' /opt/retropie/configs/gamegear/retroarch.cfg
  else
    cp /opt/retropie/configs/gamegear/retroarch.cfg /opt/retropie/configs/gamegear/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sega-Game-Gear.cfg"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '5i custom_viewport_width = "780"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '6i custom_viewport_height = "580"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '7i custom_viewport_x = "245"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '8i custom_viewport_y = "70"' /opt/retropie/configs/gamegear/retroarch.cfg
  fi  
  ;;
gamegear_other)
  ifexist=`cat /opt/retropie/configs/gamegear/retroarch.cfg |grep "input_overlay" |wc -l` 
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/gamegear/retroarch.cfg /opt/retropie/configs/gamegear/retroarch.cfg.bkp
    cat /opt/retropie/configs/gamegear/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sega-Game-Gear.cfg"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '5i custom_viewport_width = "835"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '6i custom_viewport_height = "625"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '7i custom_viewport_x = "270"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '8i custom_viewport_y = "75"' /opt/retropie/configs/gamegear/retroarch.cfg
  else
    cp /opt/retropie/configs/gamegear/retroarch.cfg /opt/retropie/configs/gamegear/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sega-Game-Gear.cfg"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '5i custom_viewport_width = "835"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '6i custom_viewport_height = "625"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '7i custom_viewport_x = "270"' /opt/retropie/configs/gamegear/retroarch.cfg
    sed -i '8i custom_viewport_y = "75"' /opt/retropie/configs/gamegear/retroarch.cfg
  fi
  ;;
gb_1080)
  ifexist=`cat /opt/retropie/configs/gb/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/gb/retroarch.cfg /opt/retropie/configs/gb/retroarch.cfg.bkp
    cat /opt/retropie/configs/gb/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/gb/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Game-Boy.cfg"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '5i custom_viewport_width = "625"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '6i custom_viewport_height = "565"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '7i custom_viewport_x = "645"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '8i custom_viewport_y = "235"' /opt/retropie/configs/gb/retroarch.cfg
  else
    cp /opt/retropie/configs/gb/retroarch.cfg /opt/retropie/configs/gb/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Game-Boy.cfg"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '5i custom_viewport_width = "625"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '6i custom_viewport_height = "565"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '7i custom_viewport_x = "645"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '8i custom_viewport_y = "235"' /opt/retropie/configs/gb/retroarch.cfg
  fi  
  ;;  
gb_720)
  ifexist=`cat /opt/retropie/configs/gb/retroarch.cfg |grep "input_overlay" |wc -l` 
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/gb/retroarch.cfg /opt/retropie/configs/gb/retroarch.cfg.bkp
    cat /opt/retropie/configs/gb/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/gb/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Game-Boy.cfg"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '5i custom_viewport_width = "429"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '6i custom_viewport_height = "380"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '7i custom_viewport_x = "420"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '8i custom_viewport_y = "155"' /opt/retropie/configs/gb/retroarch.cfg
  else
    cp /opt/retropie/configs/gb/retroarch.cfg /opt/retropie/configs/gb/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Game-Boy.cfg"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '5i custom_viewport_width = "429"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '6i custom_viewport_height = "380"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '7i custom_viewport_x = "420"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '8i custom_viewport_y = "155"' /opt/retropie/configs/gb/retroarch.cfg
  fi  
  ;;
gb_other)
  ifexist=`cat /opt/retropie/configs/gb/retroarch.cfg |grep "input_overlay" |wc -l` 
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/gb/retroarch.cfg /opt/retropie/configs/gb/retroarch.cfg.bkp
    cat /opt/retropie/configs/gb/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/gb/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Game-Boy.cfg"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '5i custom_viewport_width = "455"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '6i custom_viewport_height = "415"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '7i custom_viewport_x = "455"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '8i custom_viewport_y = "162"' /opt/retropie/configs/gb/retroarch.cfg
  else
    cp /opt/retropie/configs/gb/retroarch.cfg /opt/retropie/configs/gb/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Game-Boy.cfg"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '5i custom_viewport_width = "455"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '6i custom_viewport_height = "415"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '7i custom_viewport_x = "455"' /opt/retropie/configs/gb/retroarch.cfg
    sed -i '8i custom_viewport_y = "162"' /opt/retropie/configs/gb/retroarch.cfg
  fi
  ;;
gba_1080)
  ifexist=`cat /opt/retropie/configs/gba/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/gba/retroarch.cfg /opt/retropie/configs/gba/retroarch.cfg.bkp
    cat /opt/retropie/configs/gba/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/gba/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Game-Boy-Advance.cfg"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '5i custom_viewport_width = "1005"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '6i custom_viewport_height = "645"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '7i custom_viewport_x = "455"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '8i custom_viewport_y = "215"' /opt/retropie/configs/gba/retroarch.cfg
  else
    cp /opt/retropie/configs/gba/retroarch.cfg /opt/retropie/configs/gba/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Game-Boy-Advance.cfg"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '5i custom_viewport_width = "1005"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '6i custom_viewport_height = "645"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '7i custom_viewport_x = "455"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '8i custom_viewport_y = "215"' /opt/retropie/configs/gba/retroarch.cfg
  fi  
  ;;  
gba_720)
  ifexist=`cat /opt/retropie/configs/gba/retroarch.cfg |grep "input_overlay" |wc -l` 
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/gba/retroarch.cfg /opt/retropie/configs/gba/retroarch.cfg.bkp
    cat /opt/retropie/configs/gba/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/gba/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Game-Boy-Advance.cfg"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '5i custom_viewport_width = "467"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '6i custom_viewport_height = "316"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '7i custom_viewport_x = "405"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '8i custom_viewport_y = "190"' /opt/retropie/configs/gba/retroarch.cfg
  else
    cp /opt/retropie/configs/gba/retroarch.cfg /opt/retropie/configs/gba/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Game-Boy-Advance.cfg"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '5i custom_viewport_width = "467"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '6i custom_viewport_height = "316"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '7i custom_viewport_x = "405"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '8i custom_viewport_y = "190"' /opt/retropie/configs/gba/retroarch.cfg
  fi  
  ;;
gba_other)
  ifexist=`cat /opt/retropie/configs/gba/retroarch.cfg |grep "input_overlay" |wc -l` 
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/gba/retroarch.cfg /opt/retropie/configs/gba/retroarch.cfg.bkp
    cat /opt/retropie/configs/gba/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/gba/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Game-Boy-Advance.cfg"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '5i custom_viewport_width = "720"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '6i custom_viewport_height = "455"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '7i custom_viewport_x = "320"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '8i custom_viewport_y = "155"' /opt/retropie/configs/gba/retroarch.cfg
  else
    cp /opt/retropie/configs/gba/retroarch.cfg /opt/retropie/configs/gba/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Game-Boy-Advance.cfg"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '5i custom_viewport_width = "720"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '6i custom_viewport_height = "455"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '7i custom_viewport_x = "320"' /opt/retropie/configs/gba/retroarch.cfg
    sed -i '8i custom_viewport_y = "155"' /opt/retropie/configs/gba/retroarch.cfg
  fi
  ;;
gbc_1080)
  ifexist=`cat /opt/retropie/configs/gbc/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/gbc/retroarch.cfg /opt/retropie/configs/gbc/retroarch.cfg.bkp
    cat /opt/retropie/configs/gbc/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Game-Boy-Color.cfg"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '5i custom_viewport_width = "625"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '6i custom_viewport_height = "565"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '7i custom_viewport_x = "645"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '8i custom_viewport_y = "235"' /opt/retropie/configs/gbc/retroarch.cfg
  else
    cp /opt/retropie/configs/gbc/retroarch.cfg /opt/retropie/configs/gbc/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Game-Boy-Color.cfg"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '5i custom_viewport_width = "625"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '6i custom_viewport_height = "565"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '7i custom_viewport_x = "645"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '8i custom_viewport_y = "235"' /opt/retropie/configs/gbc/retroarch.cfg
  fi  
  ;;  
gbc_720)
  ifexist=`cat /opt/retropie/configs/gbc/retroarch.cfg |grep "input_overlay" |wc -l` 
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/gbc/retroarch.cfg /opt/retropie/configs/gbc/retroarch.cfg.bkp
    cat /opt/retropie/configs/gbc/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Game-Boy-Color.cfg"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '5i custom_viewport_width = "430"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '6i custom_viewport_height = "380"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '7i custom_viewport_x = "425"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '8i custom_viewport_y = "155"' /opt/retropie/configs/gbc/retroarch.cfg
  else
    cp /opt/retropie/configs/gbc/retroarch.cfg /opt/retropie/configs/gbc/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Game-Boy-Color.cfg"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '5i custom_viewport_width = "430"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '6i custom_viewport_height = "380"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '7i custom_viewport_x = "425"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '8i custom_viewport_y = "155"' /opt/retropie/configs/gbc/retroarch.cfg
  fi  
  ;;
gbc_other)
  ifexist=`cat /opt/retropie/configs/gbc/retroarch.cfg |grep "input_overlay" |wc -l` 
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/gbc/retroarch.cfg /opt/retropie/configs/gbc/retroarch.cfg.bkp
    cat /opt/retropie/configs/gbc/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Game-Boy-Color.cfg"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '5i custom_viewport_width = "455"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '6i custom_viewport_height = "405"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '7i custom_viewport_x = "455"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '8i custom_viewport_y = "165"' /opt/retropie/configs/gbc/retroarch.cfg
  else
    cp /opt/retropie/configs/gbc/retroarch.cfg /opt/retropie/configs/gbc/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Game-Boy-Color.cfg"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '5i custom_viewport_width = "455"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '6i custom_viewport_height = "405"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '7i custom_viewport_x = "455"' /opt/retropie/configs/gbc/retroarch.cfg
    sed -i '8i custom_viewport_y = "165"' /opt/retropie/configs/gbc/retroarch.cfg
  fi
  ;;
ngp_1080)
  ifexist=`cat /opt/retropie/configs/ngp/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/ngp/retroarch.cfg /opt/retropie/configs/ngp/retroarch.cfg.bkp
    cat /opt/retropie/configs/ngp/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/SNK-Neo-Geo-Pocket.cfg"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '5i custom_viewport_width = "700"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '6i custom_viewport_height = "635"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '7i custom_viewport_x = "610"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '8i custom_viewport_y = "220"' /opt/retropie/configs/ngp/retroarch.cfg
  else
    cp /opt/retropie/configs/ngp/retroarch.cfg /opt/retropie/configs/ngp/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/SNK-Neo-Geo-Pocket.cfg"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '5i custom_viewport_width = "700"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '6i custom_viewport_height = "635"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '7i custom_viewport_x = "610"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '8i custom_viewport_y = "220"' /opt/retropie/configs/ngp/retroarch.cfg
  fi  
  ;;  
ngp_720)
  ifexist=`cat /opt/retropie/configs/ngp/retroarch.cfg |grep "input_overlay" |wc -l` 
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/ngp/retroarch.cfg /opt/retropie/configs/ngp/retroarch.cfg.bkp
    cat /opt/retropie/configs/ngp/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/SNK-Neo-Geo-Pocket.cfg"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '5i custom_viewport_width = "461"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '6i custom_viewport_height = "428"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '7i custom_viewport_x = "407"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '8i custom_viewport_y = "145"' /opt/retropie/configs/ngp/retroarch.cfg
  else
    cp /opt/retropie/configs/ngp/retroarch.cfg /opt/retropie/configs/ngp/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/SNK-Neo-Geo-Pocket.cfg"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '5i custom_viewport_width = "461"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '6i custom_viewport_height = "428"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '7i custom_viewport_x = "407"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '8i custom_viewport_y = "145"' /opt/retropie/configs/ngp/retroarch.cfg
  fi  
  ;;
ngp_other)
  ifexist=`cat /opt/retropie/configs/ngp/retroarch.cfg |grep "input_overlay" |wc -l` 
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/ngp/retroarch.cfg /opt/retropie/configs/ngp/retroarch.cfg.bkp
    cat /opt/retropie/configs/ngp/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/SNK-Neo-Geo-Pocket.cfg"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '5i custom_viewport_width = "490"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '6i custom_viewport_height = "455"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '7i custom_viewport_x = "435"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '8i custom_viewport_y = "155"' /opt/retropie/configs/ngp/retroarch.cfg
  else
    cp /opt/retropie/configs/ngp/retroarch.cfg /opt/retropie/configs/ngp/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/SNK-Neo-Geo-Pocket.cfg"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '5i custom_viewport_width = "490"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '6i custom_viewport_height = "455"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '7i custom_viewport_x = "435"' /opt/retropie/configs/ngp/retroarch.cfg
    sed -i '8i custom_viewport_y = "155"' /opt/retropie/configs/ngp/retroarch.cfg
  fi
  ;;
ngpc_1080)
  ifexist=`cat /opt/retropie/configs/ngpc/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/ngpc/retroarch.cfg /opt/retropie/configs/ngpc/retroarch.cfg.bkp
    cat /opt/retropie/configs/ngpc/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/SNK-Neo-Geo-Pocket-Color.cfg"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '5i custom_viewport_width = "700"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '6i custom_viewport_height = "640"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '7i custom_viewport_x = "610"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '8i custom_viewport_y = "215"' /opt/retropie/configs/ngpc/retroarch.cfg
  else
    cp /opt/retropie/configs/ngpc/retroarch.cfg /opt/retropie/configs/ngpc/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/SNK-Neo-Geo-Pocket-Color.cfg"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '5i custom_viewport_width = "700"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '6i custom_viewport_height = "640"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '7i custom_viewport_x = "610"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '8i custom_viewport_y = "215"' /opt/retropie/configs/ngpc/retroarch.cfg
  fi  
  ;;  
ngpc_720)
  ifexist=`cat /opt/retropie/configs/ngpc/retroarch.cfg |grep "input_overlay" |wc -l` 
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/ngpc/retroarch.cfg /opt/retropie/configs/ngpc/retroarch.cfg.bkp
    cat /opt/retropie/configs/ngpc/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/SNK-Neo-Geo-Pocket-Color.cfg"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '5i custom_viewport_width = "460"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '6i custom_viewport_height = "428"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '7i custom_viewport_x = "407"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '8i custom_viewport_y = "145"' /opt/retropie/configs/ngpc/retroarch.cfg
  else
    cp /opt/retropie/configs/ngpc/retroarch.cfg /opt/retropie/configs/ngpc/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/SNK-Neo-Geo-Pocket-Color.cfg"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '5i custom_viewport_width = "460"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '6i custom_viewport_height = "428"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '7i custom_viewport_x = "407"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '8i custom_viewport_y = "145"' /opt/retropie/configs/ngpc/retroarch.cfg
  fi  
  ;;
ngpc_other)
  ifexist=`cat /opt/retropie/configs/ngpc/retroarch.cfg |grep "input_overlay" |wc -l` 
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/ngpc/retroarch.cfg /opt/retropie/configs/ngpc/retroarch.cfg.bkp
    cat /opt/retropie/configs/ngpc/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/SNK-Neo-Geo-Pocket-Color.cfg"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '5i custom_viewport_width = "490"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '6i custom_viewport_height = "455"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '7i custom_viewport_x = "435"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '8i custom_viewport_y = "155"' /opt/retropie/configs/ngpc/retroarch.cfg
  else
    cp /opt/retropie/configs/ngpc/retroarch.cfg /opt/retropie/configs/ngpc/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/SNK-Neo-Geo-Pocket-Color.cfg"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '5i custom_viewport_width = "490"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '6i custom_viewport_height = "455"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '7i custom_viewport_x = "435"' /opt/retropie/configs/ngpc/retroarch.cfg
    sed -i '8i custom_viewport_y = "155"' /opt/retropie/configs/ngpc/retroarch.cfg
  fi
  ;;
psp_1080)
  ifexist=`cat /opt/retropie/configs/psp/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/psp/retroarch.cfg /opt/retropie/configs/psp/retroarch.cfg.bkp
    cat /opt/retropie/configs/psp/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/psp/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sony-PSP.cfg"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '5i custom_viewport_width = "1430"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '6i custom_viewport_height = "820"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '7i custom_viewport_x = "250"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '8i custom_viewport_y = "135"' /opt/retropie/configs/psp/retroarch.cfg
  else
    cp /opt/retropie/configs/psp/retroarch.cfg /opt/retropie/configs/psp/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sony-PSP.cfg"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '5i custom_viewport_width = "1430"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '6i custom_viewport_height = "820"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '7i custom_viewport_x = "250"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '8i custom_viewport_y = "135"' /opt/retropie/configs/psp/retroarch.cfg
  fi  
  ;;  
psp_720)
  ifexist=`cat /opt/retropie/configs/psp/retroarch.cfg |grep "input_overlay" |wc -l` 
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/psp/retroarch.cfg /opt/retropie/configs/psp/retroarch.cfg.bkp
    cat /opt/retropie/configs/psp/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/psp/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sony-PSP.cfg"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '5i custom_viewport_width = "950"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '6i custom_viewport_height = "540"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '7i custom_viewport_x = "165"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '8i custom_viewport_y = "90"' /opt/retropie/configs/psp/retroarch.cfg
  else
    cp /opt/retropie/configs/psp/retroarch.cfg /opt/retropie/configs/psp/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sony-PSP.cfg"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '5i custom_viewport_width = "950"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '6i custom_viewport_height = "540"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '7i custom_viewport_x = "165"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '8i custom_viewport_y = "90"' /opt/retropie/configs/psp/retroarch.cfg
  fi  
  ;;
psp_other)
  ifexist=`cat /opt/retropie/configs/psp/retroarch.cfg |grep "input_overlay" |wc -l` 
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/psp/retroarch.cfg /opt/retropie/configs/psp/retroarch.cfg.bkp
    cat /opt/retropie/configs/psp/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/psp/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sony-PSP.cfg"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '5i custom_viewport_width = "1015"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '6i custom_viewport_height = "575"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '7i custom_viewport_x = "175"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '8i custom_viewport_y = "95"' /opt/retropie/configs/psp/retroarch.cfg
  else
    cp /opt/retropie/configs/psp/retroarch.cfg /opt/retropie/configs/psp/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sony-PSP.cfg"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '5i custom_viewport_width = "1015"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '6i custom_viewport_height = "575"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '7i custom_viewport_x = "175"' /opt/retropie/configs/psp/retroarch.cfg
    sed -i '8i custom_viewport_y = "95"' /opt/retropie/configs/psp/retroarch.cfg
  fi
  ;;
pspminis_1080)
  ifexist=`cat /opt/retropie/configs/pspminis/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/pspminis/retroarch.cfg /opt/retropie/configs/pspminis/retroarch.cfg.bkp
    cat /opt/retropie/configs/pspminis/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sony-PSP.cfg"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '5i custom_viewport_width = "1430"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '6i custom_viewport_height = "820"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '7i custom_viewport_x = "250"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '8i custom_viewport_y = "135"' /opt/retropie/configs/pspminis/retroarch.cfg
  else
    cp /opt/retropie/configs/pspminis/retroarch.cfg /opt/retropie/configs/pspminis/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sony-PSP.cfg"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '5i custom_viewport_width = "1430"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '6i custom_viewport_height = "820"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '7i custom_viewport_x = "250"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '8i custom_viewport_y = "135"' /opt/retropie/configs/pspminis/retroarch.cfg
  fi  
  ;;  
pspminis_720)
  ifexist=`cat /opt/retropie/configs/pspminis/retroarch.cfg |grep "input_overlay" |wc -l` 
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/pspminis/retroarch.cfg /opt/retropie/configs/pspminis/retroarch.cfg.bkp
    cat /opt/retropie/configs/pspminis/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sony-PSP.cfg"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '5i custom_viewport_width = "950"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '6i custom_viewport_height = "540"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '7i custom_viewport_x = "165"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '8i custom_viewport_y = "90"' /opt/retropie/configs/pspminis/retroarch.cfg
  else
    cp /opt/retropie/configs/pspminis/retroarch.cfg /opt/retropie/configs/pspminis/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sony-PSP.cfg"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '5i custom_viewport_width = "950"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '6i custom_viewport_height = "540"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '7i custom_viewport_x = "165"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '8i custom_viewport_y = "90"' /opt/retropie/configs/pspminis/retroarch.cfg
  fi  
  ;;
pspminis_other)
  ifexist=`cat /opt/retropie/configs/pspminis/retroarch.cfg |grep "input_overlay" |wc -l` 
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/pspminis/retroarch.cfg /opt/retropie/configs/pspminis/retroarch.cfg.bkp
    cat /opt/retropie/configs/pspminis/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sony-PSP.cfg"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '5i custom_viewport_width = "1015"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '6i custom_viewport_height = "575"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '7i custom_viewport_x = "175"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '8i custom_viewport_y = "95"' /opt/retropie/configs/pspminis/retroarch.cfg
  else
    cp /opt/retropie/configs/pspminis/retroarch.cfg /opt/retropie/configs/pspminis/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sony-PSP.cfg"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '5i custom_viewport_width = "1015"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '6i custom_viewport_height = "575"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '7i custom_viewport_x = "175"' /opt/retropie/configs/pspminis/retroarch.cfg
    sed -i '8i custom_viewport_y = "95"' /opt/retropie/configs/pspminis/retroarch.cfg
  fi
  ;;
virtualboy_1080)
  ifexist=`cat /opt/retropie/configs/virtualboy/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/virtualboy/retroarch.cfg /opt/retropie/configs/virtualboy/retroarch.cfg.bkp
    cat /opt/retropie/configs/virtualboy/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Virtual-Boy.cfg"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '5i custom_viewport_width = "1115"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '6i custom_viewport_height = "695"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '7i custom_viewport_x = "405"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '8i custom_viewport_y = "215"' /opt/retropie/configs/virtualboy/retroarch.cfg
  else
    cp /opt/retropie/configs/virtualboy/retroarch.cfg /opt/retropie/configs/virtualboy/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Virtual-Boy.cfg"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '5i custom_viewport_width = "1115"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '6i custom_viewport_height = "695"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '7i custom_viewport_x = "405"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '8i custom_viewport_y = "215"' /opt/retropie/configs/virtualboy/retroarch.cfg
  fi  
  ;;  
virtualboy_720)
  ifexist=`cat /opt/retropie/configs/virtualboy/retroarch.cfg |grep "input_overlay" |wc -l` 
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/virtualboy/retroarch.cfg /opt/retropie/configs/virtualboy/retroarch.cfg.bkp
    cat /opt/retropie/configs/virtualboy/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Virtual-Boy.cfg"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '5i custom_viewport_width = "740"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '6i custom_viewport_height = "470"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '7i custom_viewport_x = "270"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '8i custom_viewport_y = "140"' /opt/retropie/configs/virtualboy/retroarch.cfg
  else
    cp /opt/retropie/configs/virtualboy/retroarch.cfg /opt/retropie/configs/virtualboy/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Virtual-Boy.cfg"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '5i custom_viewport_width = "740"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '6i custom_viewport_height = "470"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '7i custom_viewport_x = "270"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '8i custom_viewport_y = "140"' /opt/retropie/configs/virtualboy/retroarch.cfg
  fi  
  ;;
virtualboy_other)
  ifexist=`cat /opt/retropie/configs/virtualboy/retroarch.cfg |grep "input_overlay" |wc -l` 
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/virtualboy/retroarch.cfg /opt/retropie/configs/virtualboy/retroarch.cfg.bkp
    cat /opt/retropie/configs/virtualboy/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Virtual-Boy.cfg"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '5i custom_viewport_width = "787"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '6i custom_viewport_height = "494"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '7i custom_viewport_x = "290"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '8i custom_viewport_y = "153"' /opt/retropie/configs/virtualboy/retroarch.cfg
  else
    cp /opt/retropie/configs/virtualboy/retroarch.cfg /opt/retropie/configs/virtualboy/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Nintendo-Virtual-Boy.cfg"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '5i custom_viewport_width = "787"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '6i custom_viewport_height = "494"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '7i custom_viewport_x = "290"' /opt/retropie/configs/virtualboy/retroarch.cfg
    sed -i '8i custom_viewport_y = "153"' /opt/retropie/configs/virtualboy/retroarch.cfg
  fi
  ;;
wonderswan_1080)
  ifexist=`cat /opt/retropie/configs/wonderswan/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/wonderswan/retroarch.cfg /opt/retropie/configs/wonderswan/retroarch.cfg.bkp
    cat /opt/retropie/configs/wonderswan/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Bandai-WonderSwan-Horizontal.cfg"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '5i custom_viewport_width = "950"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '6i custom_viewport_height = "605"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '7i custom_viewport_x = "495"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '8i custom_viewport_y = "225"' /opt/retropie/configs/wonderswan/retroarch.cfg
  else
    cp /opt/retropie/configs/wonderswan/retroarch.cfg /opt/retropie/configs/wonderswan/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Bandai-WonderSwan-Horizontal.cfg"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '5i custom_viewport_width = "950"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '6i custom_viewport_height = "605"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '7i custom_viewport_x = "495"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '8i custom_viewport_y = "225"' /opt/retropie/configs/wonderswan/retroarch.cfg
  fi  
  ;;  
wonderswan_720)
  ifexist=`cat /opt/retropie/configs/wonderswan/retroarch.cfg |grep "input_overlay" |wc -l` 
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/wonderswan/retroarch.cfg /opt/retropie/configs/wonderswan/retroarch.cfg.bkp
    cat /opt/retropie/configs/wonderswan/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Bandai-WonderSwan-Horizontal.cfg"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '5i custom_viewport_width = "645"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '6i custom_viewport_height = "407"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '7i custom_viewport_x = "325"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '8i custom_viewport_y = "148"' /opt/retropie/configs/wonderswan/retroarch.cfg
  else
    cp /opt/retropie/configs/wonderswan/retroarch.cfg /opt/retropie/configs/wonderswan/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Bandai-WonderSwan-Horizontal.cfg"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '5i custom_viewport_width = "645"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '6i custom_viewport_height = "407"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '7i custom_viewport_x = "325"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '8i custom_viewport_y = "148"' /opt/retropie/configs/wonderswan/retroarch.cfg
  fi  
  ;;
wonderswan_other)
  ifexist=`cat /opt/retropie/configs/wonderswan/retroarch.cfg |grep "input_overlay" |wc -l` 
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/wonderswan/retroarch.cfg /opt/retropie/configs/wonderswan/retroarch.cfg.bkp
    cat /opt/retropie/configs/wonderswan/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Bandai-WonderSwan-Horizontal.cfg"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '5i custom_viewport_width = "690"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '6i custom_viewport_height = "435"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '7i custom_viewport_x = "345"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '8i custom_viewport_y = "155"' /opt/retropie/configs/wonderswan/retroarch.cfg
  else
    cp /opt/retropie/configs/wonderswan/retroarch.cfg /opt/retropie/configs/wonderswan/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Bandai-WonderSwan-Horizontal.cfg"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '5i custom_viewport_width = "690"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '6i custom_viewport_height = "435"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '7i custom_viewport_x = "345"' /opt/retropie/configs/wonderswan/retroarch.cfg
    sed -i '8i custom_viewport_y = "155"' /opt/retropie/configs/wonderswan/retroarch.cfg
  fi
  ;;
wonderswancolor_1080)
  ifexist=`cat /opt/retropie/configs/wonderswancolor/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/wonderswancolor/retroarch.cfg /opt/retropie/configs/wonderswancolor/retroarch.cfg.bkp
    cat /opt/retropie/configs/wonderswancolor/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Bandai-WonderSwan-Color-Horizontal.cfg"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '5i custom_viewport_width = "950"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '6i custom_viewport_height = "605"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '7i custom_viewport_x = "490"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '8i custom_viewport_y = "225"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
  else
    cp /opt/retropie/configs/wonderswancolor/retroarch.cfg /opt/retropie/configs/wonderswancolor/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Bandai-WonderSwan-Color-Horizontal.cfg"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '5i custom_viewport_width = "950"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '6i custom_viewport_height = "605"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '7i custom_viewport_x = "490"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '8i custom_viewport_y = "225"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
  fi  
  ;;  
wonderswancolor_720)
  ifexist=`cat /opt/retropie/configs/wonderswancolor/retroarch.cfg |grep "input_overlay" |wc -l` 
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/wonderswancolor/retroarch.cfg /opt/retropie/configs/wonderswancolor/retroarch.cfg.bkp
    cat /opt/retropie/configs/wonderswancolor/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Bandai-WonderSwan-Color-Horizontal.cfg"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '5i custom_viewport_width = "643"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '6i custom_viewport_height = "405"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '7i custom_viewport_x = "325"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '8i custom_viewport_y = "150"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
  else
    cp /opt/retropie/configs/wonderswancolor/retroarch.cfg /opt/retropie/configs/wonderswancolor/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Bandai-WonderSwan-Color-Horizontal.cfg"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '5i custom_viewport_width = "643"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '6i custom_viewport_height = "405"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '7i custom_viewport_x = "325"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '8i custom_viewport_y = "150"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
  fi  
  ;;
wonderswancolor_other)
  ifexist=`cat /opt/retropie/configs/wonderswancolor/retroarch.cfg |grep "input_overlay" |wc -l` 
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/wonderswancolor/retroarch.cfg /opt/retropie/configs/wonderswancolor/retroarch.cfg.bkp
    cat /opt/retropie/configs/wonderswancolor/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Bandai-WonderSwan-Color-Horizontal.cfg"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '5i custom_viewport_width = "690"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '6i custom_viewport_height = "435"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '7i custom_viewport_x = "345"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '8i custom_viewport_y = "155"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
  else
    cp /opt/retropie/configs/wonderswancolor/retroarch.cfg /opt/retropie/configs/wonderswancolor/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Bandai-WonderSwan-Color-Horizontal.cfg"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '4i aspect_ratio_index = "22"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '5i custom_viewport_width = "690"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '6i custom_viewport_height = "435"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '7i custom_viewport_x = "345"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
    sed -i '8i custom_viewport_y = "155"' /opt/retropie/configs/wonderswancolor/retroarch.cfg
  fi
  ;;
amstradcpc)
  ifexist=`cat /opt/retropie/configs/amstradcpc/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/amstradcpc/retroarch.cfg /opt/retropie/configs/amstradcpc/retroarch.cfg.bkp
    cat /opt/retropie/configs/amstradcpc/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/amstradcpc/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Amstrad-CPC.cfg"' /opt/retropie/configs/amstradcpc/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/amstradcpc/retroarch.cfg
  else
    cp /opt/retropie/configs/amstradcpc/retroarch.cfg /opt/retropie/configs/amstradcpc/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Amstrad-CPC.cfg"' /opt/retropie/configs/amstradcpc/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/amstradcpc/retroarch.cfg
  fi
  ;;
atari800)
  ifexist=`cat /opt/retropie/configs/atari800/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/atari800/retroarch.cfg /opt/retropie/configs/atari800/retroarch.cfg.bkp
    cat /opt/retropie/configs/atari800/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/atari800/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Atari-800.cfg"' /opt/retropie/configs/atari800/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/atari800/retroarch.cfg
  else
    cp /opt/retropie/configs/atari800/retroarch.cfg /opt/retropie/configs/atari800/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Atari-800.cfg"' /opt/retropie/configs/atari800/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/atari800/retroarch.cfg
  fi
  ;;
atarist)
  ifexist=`cat /opt/retropie/configs/atarist/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/atarist/retroarch.cfg /opt/retropie/configs/atarist/retroarch.cfg.bkp
    cat /opt/retropie/configs/atarist/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/atarist/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Atari-ST.cfg"' /opt/retropie/configs/atarist/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/atarist/retroarch.cfg
  else
    cp /opt/retropie/configs/atarist/retroarch.cfg /opt/retropie/configs/atarist/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Atari-ST.cfg"' /opt/retropie/configs/atarist/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/atarist/retroarch.cfg
  fi
  ;;
c64)
  ifexist=`cat /opt/retropie/configs/c64/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/c64/retroarch.cfg /opt/retropie/configs/c64/retroarch.cfg.bkp
    cat /opt/retropie/configs/c64/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/c64/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Commodore-64.cfg"' /opt/retropie/configs/c64/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/c64/retroarch.cfg
  else
    cp /opt/retropie/configs/c64/retroarch.cfg /opt/retropie/configs/c64/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Commodore-64.cfg"' /opt/retropie/configs/c64/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/c64/retroarch.cfg
  fi
  ;;
msx)
  ifexist=`cat /opt/retropie/configs/msx/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/msx/retroarch.cfg /opt/retropie/configs/msx/retroarch.cfg.bkp
    cat /opt/retropie/configs/msx/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/msx/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Microsoft-MSX.cfg"' /opt/retropie/configs/msx/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/msx/retroarch.cfg
  else
    cp /opt/retropie/configs/msx/retroarch.cfg /opt/retropie/configs/msx/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Microsoft-MSX.cfg"' /opt/retropie/configs/msx/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/msx/retroarch.cfg
  fi
  ;;
msx2)
  ifexist=`cat /opt/retropie/configs/msx2/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/msx2/retroarch.cfg /opt/retropie/configs/msx2/retroarch.cfg.bkp
    cat /opt/retropie/configs/msx2/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/msx2/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Microsoft-MSX2.cfg"' /opt/retropie/configs/msx2/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/msx2/retroarch.cfg
  else
    cp /opt/retropie/configs/msx2/retroarch.cfg /opt/retropie/configs/msx2/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Microsoft-MSX2.cfg"' /opt/retropie/configs/msx2/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/msx2/retroarch.cfg
  fi
  ;;
videopac)
  ifexist=`cat /opt/retropie/configs/videopac/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/videopac/retroarch.cfg /opt/retropie/configs/videopac/retroarch.cfg.bkp
    cat /opt/retropie/configs/videopac/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/videopac/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Magnavox-Odyssey-2.cfg"' /opt/retropie/configs/videopac/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/videopac/retroarch.cfg
  else
    cp /opt/retropie/configs/videopac/retroarch.cfg /opt/retropie/configs/videopac/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Magnavox-Odyssey-2.cfg"' /opt/retropie/configs/videopac/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/videopac/retroarch.cfg
  fi
  ;;
x68000)
  ifexist=`cat /opt/retropie/configs/x68000/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/x68000/retroarch.cfg /opt/retropie/configs/x68000/retroarch.cfg.bkp
    cat /opt/retropie/configs/x68000/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/x68000/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sharp-X68000.cfg"' /opt/retropie/configs/x68000/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/x68000/retroarch.cfg
  else
    cp /opt/retropie/configs/x68000/retroarch.cfg /opt/retropie/configs/x68000/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sharp-X68000.cfg"' /opt/retropie/configs/x68000/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/x68000/retroarch.cfg
  fi
  ;;
zxspectrum)
  ifexist=`cat /opt/retropie/configs/zxspectrum/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/zxspectrum/retroarch.cfg /opt/retropie/configs/zxspectrum/retroarch.cfg.bkp
    cat /opt/retropie/configs/zxspectrum/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/zxspectrum/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sinclair-ZX-Spectrum.cfg"' /opt/retropie/configs/zxspectrum/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/zxspectrum/retroarch.cfg
  else
    cp /opt/retropie/configs/zxspectrum/retroarch.cfg /opt/retropie/configs/zxspectrum/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Sinclair-ZX-Spectrum.cfg"' /opt/retropie/configs/zxspectrum/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/zxspectrum/retroarch.cfg
  fi
  ;;
supergamemachine)
  ifexist=`cat /opt/retropie/configs/supergamemachine/retroarch.cfg |grep "input_overlay" |wc -l`
  if [[ ${ifexist} > 0 ]]
  then
    cp /opt/retropie/configs/supergamemachine/retroarch.cfg /opt/retropie/configs/supergamemachine/retroarch.cfg.bkp
    cat /opt/retropie/configs/supergamemachine/retroarch.cfg |grep -v input_overlay |grep -v aspect_ratio |grep -v custom_viewport > /tmp/retroarch.cfg
    cp /tmp/retroarch.cfg /opt/retropie/configs/supergamemachine/retroarch.cfg
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/Atari-2600.cfg"' /opt/retropie/configs/supergamemachine/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/supergamemachine/retroarch.cfg
  else
    cp /opt/retropie/configs/supergamemachine/retroarch.cfg /opt/retropie/configs/supergamemachine/retroarch.cfg.bkp
    sed -i '2i input_overlay = "/opt/retropie/configs/all/retroarch/overlay/SuperGameMachine.cfg"' /opt/retropie/configs/supergamemachine/retroarch.cfg
    sed -i '3i input_overlay_opacity = "1.000000"' /opt/retropie/configs/supergamemachine/retroarch.cfg
  fi
  ;;
esac
}

function retroarch_bezelinfo() {

echo "The Bezel Project is setup with the following sytem-to-core mapping." > /tmp/bezelprojectinfo.txt
echo "" >> /tmp/bezelprojectinfo.txt

echo "To show a specific game bezel, Retroarch must have an override config file for each game.  These " >> /tmp/bezelprojectinfo.txt
echo "configuration files are saved in special directories that are named according to the Retroarch " >> /tmp/bezelprojectinfo.txt
echo "emulator core that system uses." >> /tmp/bezelprojectinfo.txt
echo "" >> /tmp/bezelprojectinfo.txt

echo "The supplied Retroarch configuration files for the bezel utility are setup to use certain " >> /tmp/bezelprojectinfo.txt
echo "emulators for certain systems." >> /tmp/bezelprojectinfo.txt
echo "" >> /tmp/bezelprojectinfo.txt

echo "In order for the supplied bezels to be shown, you must be using the proper Retroarch emulator " >> /tmp/bezelprojectinfo.txt
echo "for a system listed in the table below." >> /tmp/bezelprojectinfo.txt
echo "" >> /tmp/bezelprojectinfo.txt

echo "This table lists all of the systems that have the abilty to show bezels that The Bezel Project " >> /tmp/bezelprojectinfo.txt
echo "hopes to make bezels for." >> /tmp/bezelprojectinfo.txt
echo "" >> /tmp/bezelprojectinfo.txt
echo "" >> /tmp/bezelprojectinfo.txt

echo "System                                          Retroarch Emulator" >> /tmp/bezelprojectinfo.txt
echo "Atari 2600                                      lr-stella" >> /tmp/bezelprojectinfo.txt
echo "Atari 5200                                      lr-atari800" >> /tmp/bezelprojectinfo.txt
echo "Atari 7800                                      lr-prosystem" >> /tmp/bezelprojectinfo.txt
echo "ColecoVision                                    lr-bluemsx" >> /tmp/bezelprojectinfo.txt
echo "GCE Vectrex                                     lr-vecx" >> /tmp/bezelprojectinfo.txt
echo "NEC PC Engine CD                                lr-beetle-pce-fast" >> /tmp/bezelprojectinfo.txt
echo "NEC PC Engine                                   lr-beetle-pce-fast" >> /tmp/bezelprojectinfo.txt
echo "NEC SuperGrafx                                  lr-beetle-supergrafx" >> /tmp/bezelprojectinfo.txt
echo "NEC TurboGrafx-CD                               lr-beetle-pce-fast" >> /tmp/bezelprojectinfo.txt
echo "NEC TurboGrafx-16                               lr-beetle-pce-fast" >> /tmp/bezelprojectinfo.txt
echo "Nintendo 64                                     lr-Mupen64plus" >> /tmp/bezelprojectinfo.txt
echo "Nintendo Entertainment System                   lr-fceumm, lr-nestopia" >> /tmp/bezelprojectinfo.txt
echo "Nintendo Famicom Disk System                    lr-fceumm, lr-nestopia" >> /tmp/bezelprojectinfo.txt
echo "Nintendo Famicom                                lr-fceumm, lr-nestopia" >> /tmp/bezelprojectinfo.txt
echo "Nintendo Super Famicom                          lr-snes9x, lr-snes9x2010" >> /tmp/bezelprojectinfo.txt
echo "Sega 32X                                        lr-picodrive, lr-genesis-plus-gx" >> /tmp/bezelprojectinfo.txt
echo "Sega CD                                         lr-picodrive, lr-genesis-plus-gx" >> /tmp/bezelprojectinfo.txt
echo "Sega Genesis                                    lr-picodrive, lr-genesis-plus-gx" >> /tmp/bezelprojectinfo.txt
echo "Sega Master System                              lr-picodrive, lr-genesis-plus-gx" >> /tmp/bezelprojectinfo.txt
echo "Sega Mega Drive                                 lr-picodrive, lr-genesis-plus-gx" >> /tmp/bezelprojectinfo.txt
echo "Sega Mega Drive Japan                           lr-picodrive, lr-genesis-plus-gx" >> /tmp/bezelprojectinfo.txt
echo "Sega SG-1000                                    lr-genesis-plus-gx" >> /tmp/bezelprojectinfo.txt
echo "Sony PlayStation                                lr-pcsx-rearmed" >> /tmp/bezelprojectinfo.txt
echo "Super Nintendo Entertainment System             lr-snes9x, lr-snes9x2010" >> /tmp/bezelprojectinfo.txt
echo "" >> /tmp/bezelprojectinfo.txt

dialog --backtitle "The Bezel Project" \
--title "The Bezel Project - Bezel Pack Utility" \
--textbox /tmp/bezelprojectinfo.txt 30 110
}
