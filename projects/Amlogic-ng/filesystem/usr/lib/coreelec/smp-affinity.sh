#!/bin/sh

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Team CoreELEC (https://coreelec.org)

SMP="../smp_affinity"
MESONIR="`find /proc/irq/ -name ir-meson`"
ETH0="`find /proc/irq/ -name eth0`"
AOCECB="`find /proc/irq/ -name hdmi_aocecb`"
USB3="`find /proc/irq/ -name xhci-hcd:usb1`"
IRQ="$AOCECB $ETH0 $USB3 $MESONIR"

cpu=0
aff=1
for i in $IRQ; do
    if [ -f "$i/$SMP" ];then
	cpu=$((cpu + 1))
	if [ -d "/sys/devices/system/cpu/cpu$cpu" ];then
	    aff=`printf '%x\n' $((aff * 2))`
	else
	    aff=2
	    cpu=0
	fi
        echo "echo $aff > $i/$SMP"
        echo $aff > $i/$SMP
    fi
done

exit 0
