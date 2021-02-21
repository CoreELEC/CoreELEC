#!/bin/bash

# This file is part of The RetroArena (TheRA)
#
# The RetroArena (TheRA) is the legal property of its developers, whose names are
# too numerous to list here. Please refer to the COPYRIGHT.md file distributed with this source.
#
# See the LICENSE.md file at the top-level directory of this distribution and
# at https://raw.githubusercontent.com/Retro-Arena/RetroArena-Setup/master/LICENSE.md
#

rp_module_id="systeminfo"
rp_module_desc="View your CPU temps, IP connectivity, and storage to include external storage addons."
rp_module_section="config"

function gui_systeminfo() {
    echo "                         Storage System" > /tmp/line1
    echo " " > /tmp/free
    df -h | tail -n +4 > /tmp/temp-disk3
    sed '1 i "Name          Total      Used      Free Load Mountpoint"' /tmp/temp-disk3 > /tmp/temp-disk2
    sed '/^tmpfs/ d' /tmp/temp-disk2 > /tmp/temp-disk4
    sed '/^none/ d' /tmp/temp-disk4 > /tmp/temp-disk
    sed -i 's,'"/dev/data          "',/dev/data,' /tmp/temp-disk > /tmp/tt
    sed -i 's,'"/dev/loop0          "',/dev/loop0,' /tmp/temp-disk > /tmp/tt
    sed -i 's,'"/dev/sda1    "',USB,' /tmp/temp-disk > /tmp/tt
    sed -i 's,'"/var"',,' /tmp/temp-disk > /tmp/tt
    echo "                     Temperature Monitoring" > /tmp/temph
    cpuTempC=$(($(cat /sys/class/thermal/thermal_zone0/temp)/1000)) && cpuTempF=$((cpuTempC*9/5+32))
    echo $cpuTempC > /tmp/tempc
    sed 's/^/Cpu Temperature in Celcius degree :   /' /tmp/tempc > /tmp/tempC
    echo $cpuTempF > /tmp/tempf
    sed 's/^/Cpu Temperature in Farenheit degree : /' /tmp/tempf > /tmp/tempF
    echo "                         Network Info" > /tmp/net
    ip route get 8.8.8.8 2>/dev/null | awk '{print $NF; exit}' > /tmp/ip
    sed 's/^/Local Ip address :           /' /tmp/ip > /tmp/IP
    wget -qO- http://ipecho.net/plain > /tmp/wan
    sed 's/^/Public Ip address :          /' /tmp/wan > /tmp/WAN
    cat /sys/class/net/eth0/operstate > /tmp/wired
    sed 's/^/Ethernet Connection status : /' /tmp/wired > /tmp/WIRED
    cat /sys/class/net/lo/operstate > /tmp/loop
    sed 's/^/Loopback interface status :  /' /tmp/loop > /tmp/LOOP
    cat /sys/class/net/wlan0/operstate > /tmp/wlan
    sed 's/^/Wireless Connection status : /' /tmp/wlan > /tmp/WLAN
    sed h /tmp/line1 /tmp/free /tmp/temp-disk /tmp/free /tmp/free /tmp/temph /tmp/free /tmp/tempC /tmp/tempF /tmp/free /tmp/free /tmp/net /tmp/free /tmp/IP /tmp/WAN /tmp/LOOP /tmp/WIRED /tmp/WLAN > /tmp/display
    rm /tmp/free /tmp/tt /tmp/line1 /tmp/temp* /tmp/ip /tmp/loop /tmp/wan /tmp/wired /tmp/wlan /tmp/net /tmp/IP /tmp/WAN /tmp/LOOP /tmp/WIRED /tmp/WLAN
    dialog --ascii-lines --backtitle "emuELEC System Information" --title "emuELEC System Information" --textbox /tmp/display 27 72
    rm /tmp/display
}
