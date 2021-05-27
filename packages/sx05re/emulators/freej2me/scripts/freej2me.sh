#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2021-present Shanti Gilbert (https://github.com/shantigilbert)

. /etc/profile

ee_console enable
echo "Checking JDK..." > /dev/console

JDKDEST="/emuelec/configs/jdk"
JDKNAME="zulu11.48.21-ca-jdk11.0.11"

mkdir -p ${JDKDEST}

# Check if the jdk does not already exists
[ "$(ls -A ${JDKDEST})" ] && JDKINSTALLED="yes" || JDKINSTALLED="no"

if [ ${JDKINSTALLED} == "no" ]; then
    echo "Downloading JDK please be patient..." > /dev/console
    cd ${JDKDEST}/..
    wget "https://cdn.azul.com/zulu-embedded/bin/${JDKNAME}-linux_aarch64.tar.gz" > /dev/console
    echo "Inflating JDK please be patient..." > /dev/console
    tar xvfz ${JDKNAME}-linux_aarch64.tar.gz
    mv ${JDKNAME}-linux_aarch64/* jdk
    rm -rf ${JDKNAME}-linux_aarch64*
    
    for del in jmods include demo legal man DISCLAIMER LICENSE readme.txt release Welcome.html; do
        rm -rf ${JDKDEST}/${del}
    done
    echo "JDK done! loading core!" > /dev/console
#    cp -rf /usr/lib/libretro/freej2me*.jar /storage/roms/bios
fi

ee_console disable
exit 0
