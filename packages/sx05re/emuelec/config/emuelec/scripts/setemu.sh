#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

## workaround for ES performance with big conf files

EE_CONF="/emuelec/configs/emuoptions.conf"

[ ! -f ${EE_CONF} ] && touch ${EE_CONF}

case "$1" in
"set")
	PAT=$(echo "$2" | sed -e 's|\"|\\"|g' | sed -e 's|\[|\\\[|g' | sed -e 's|\]|\\\]|g')
	sed -i "/$PAT/d" "${EE_CONF}"
	if [ "$3" != "auto" ]; then
		[ $3 == "disable" ] && echo "#${2}=" >> "${EE_CONF}" || echo "${2}=${3}" >> "${EE_CONF}"
	fi
	;;
"get")
	PAT=$(echo ${2} | sed -e 's|\"|\\"|g' | sed -e 's|\[|\\\[|g' | sed -e 's|\]|\\\]|g' | sed -e 's|(|\\\(|g' | sed -e 's|)|\\\)|g')
	PAT="^${PAT}=(.*)"
	EES=$(cat "${EE_CONF}" | grep -oE "${PAT}")
	echo "${EES##*=}"
	;;
esac
