#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

. /etc/profile

PLATFORM=$(cat /ee_arch)
TEST_UPDURL="https://api.github.com/repos/EmuELEC/emuelec-tests/releases/latest"
UPDURL="https://api.github.com/repos/EmuELEC/emuelec/releases/latest"
arguments="$@"
UPDTYPE=$(get_ee_setting updates.type)

function do_cleanup() {
	rm -rf /storage/.update > /dev/null 2>&1
	mkdir -p /storage/.update
	}

function no_update() {
	echo "no"
	do_cleanup
	exit 1
}
#make sure there is no old update
do_cleanup

if [[ "$arguments" == *"forceupdate"* ]]; then
	if [[ "$arguments" == *"test"* ]]; then
		UPDURL=$TEST_UPDURL
		UPDTYPE="test"
	else
		UPDTYPE="stable"
	fi
else
	[[ "$UPDTYPE" != "stable" ]] && UPDURL=$TEST_UPDURL
fi

# to avoid curl error “(23) Failed writing body”, we download the full page to /tmp/eeupd and use cat to read it then delete it
curl -s ${UPDURL} -o /tmp/eeupd
UFILE=$(cat /tmp/eeupd | grep -B 4 -m 1 "browser_download_url.*${PLATFORM}\..*.tar\b" | cut -d : -f 2,3 | tr -d \")
rm /tmp/eeupd
USIZE=$(echo ${UFILE} | cut -d, -f1)
UFILE="http"${UFILE#*"http"}
[ -z "$UFILE" ] && no_update

# if you use forceupdate as an argument you can forcibly download the latest STABLE release and call an update
if [[ "$arguments" == *"forceupdate"* ]]; then

echo "Doing a force ${UPDTYPE} update..."
read -p "Are you sure? [y/n]" -n 1 -r
echo    # (optional) move to a new line
if [[ $REPLY =~ ^[Yy]$ ]]; then
	UPDURL=$UFILE
	UFILE=${UPDURL##*/}
	sleep 5
	echo "Downloadinng ${UPDURL} to /storage/.update/${UFILE}"
	touch "/storage/.update/${UFILE}"
	wget "${UPDURL}" -O "/storage/.update/${UFILE}" || echo "Exit code: $?"
	echo "Trying to download sha256 checksum"

# Try to download an sha256 checksum
wget -q "${UPDURL}.sha256" -O "/storage/.update/${UFILE}.sha256"

if test -e "/storage/.update/${UFILE}.sha256"; then
	echo "Doing checksum..."
    DISTMD5=$(cat "/storage/.update/${UFILE}.sha256" | cut -d ' ' -f 1)
    CURRMD5=$(sha256sum "/storage/.update/${UFILE}" | cut -d ' ' -f 1)
    if test "${DISTMD5}" = "${CURRMD5}"; then
	echo "valid checksum."
    else
	echo "invalid checksum. Got +${DISTMD5}+. Attempted +${CURRMD5}+."
	exit 1
    fi
else
    echo "no checksum found. don't check the file."
fi
	echo "Aplying update"
	sync
	systemctl stop emustation
	sleep 5 #give time for ES to close
	clearconfig.sh EMUS
	systemctl reboot
	exit 0
else
	do_cleanup
	echo "Force update canceled."
	exit 1
fi
	fi
	
function check_update() { 

if [ "$UPDATEVER" -gt "$CURRENTVER" ] ; then 

if [[ "$arguments" == *"canupdate"* ]]; then
	[[ ! -z "$DVER" ]] && UVER=$DVER
	echo "$UVER"
elif [[ "$arguments" == *"geturl"* ]]; then	
	echo "$UFILE"
elif [[ "$arguments" == *"getsize"* ]]; then	
	echo "$USIZE"
fi
else 
no_update
fi 
}

CVER=$(cat /usr/config/EE_VERSION)
[[ -z "$CVER" ]] && no_update

if [[ "$CVER" == *"TEST"* ]]; then
	if [[ "$UPDTYPE" == "stable" ]]; then
		echo "no"
		exit 1
	else
		CVER=$(echo "$CVER" | sed "s|-TEST-||")
	fi
fi

if [[ "$UPDTYPE" == "stable" ]]; then
	UVER=${UFILE##*-}
	UVER=${UVER%.tar}
else
	UVER=${UFILE##*arm}
	UVER=${UVER%.tar}
	UVER=${UVER:1}
	DVER="$UVER"
	UVER=$(echo "$UVER" | sed "s|-TEST-||")
fi
[[ -z "$UVER" ]] && no_update

CURRENTVER="${CVER%%.*}${CVER#*.}"
UPDATEVER="${UVER%%.*}${UVER#*.}"

check_update
