#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

# Source predefined functions and variables
. /etc/profile

ROMFILE="emuelec_copy_roms_from_here"

function copy_from_where() {
FULLPATHTOROMS="$(find /media/*/roms/ -name ${ROMFILE} -maxdepth 1 | head -n 1)"

if [[ -z "${FULLPATHTOROMS}" ]]; then
	ROMSNOTFOUND="yes"
else
	ROMSNOTFOUND="no"
	PATHTOROMS="${FULLPATHTOROMS%$ROMFILE}"
fi 

echo $PATHTOROMS
}

ROMFOLDER="$(copy_from_where)"

function copy_confirm() {
if [ -z "${ROMFOLDER}" ]; then 
    echo -en "No USB media with the file \"${ROMFILE}\" is connected! Did you create the file?\n\nYou need to create a file (NOT A FOLDER/DIRECTORY!) named\n\n\"${ROMFILE}\" (WITH NO EXTENSION!)\n\nin the USB:/roms folder before runing this script! " > /tmp/display
    text_viewer -e -t "ERROR!" -f 24 /tmp/display
    rm /tmp/display
	exit 1
fi
	echo -en "This will copy all files from \"${ROMFOLDER}\", to \"/storage/roms\" on the device.\n\nMAKE SURE YOU HAVE ENOUGH SPACE IN YOUR DEVICE!\n\nWARNING: Existing files in \"/storage/roms\" with the same name will be overwriten\nNO BACKUP WILL BE CREATED!\n\nare you sure you want to continue?"> /tmp/display
    text_viewer -y -t "Copy roms from USB to SD" -f 24 /tmp/display
    [[ $? == 21 ]] && copy_roms || exit 0;
 }

function copy_roms() {
	# Sanity checks
	[[ -L "/storage/roms" ]] && rm /storage/roms
	[[ -d /storage/roms2 ]] && mv /storage/roms2 /storage/roms
	[[ ! -d /storage/roms ]] && mkdir -p /storage/roms
    rm /tmp/display
	# End sanity
	
    ee_console enable

    echo "Copying, please wait!..." > /dev/tty0
	rsync -ahI --progress "${ROMFOLDER}"* /storage/roms > /dev/tty0
    COPY=$(cat /tmp/copy)
    
    ee_console disable
    
	# Clean up
	[[ -f /storage/roms/${ROMFILE} ]] && rm /storage/roms/${ROMFILE}
	[[ -f /storage/roms/emuelecroms ]] && rm /storage/roms/emuelecroms
    echo -en "Copy finished!\n\n" >> /tmp/display
	
	echo -en "Remove the USB media from the device and press YES to restart ES\n\nPressing NO will return to ES without restarting!" >> /tmp/display
    echo -en "\n\n\n$COPY" >> /tmp/display
	if ! text_viewer -y -t "Copy roms from USB to SD" -f 24 /tmp/display >&1; then
        rm /tmp/display
        systemctl restart emustation
    else
        exit 0;
    fi
}

copy_confirm
rm /tmp/display > /dev/null 2>&1
