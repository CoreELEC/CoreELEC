#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2021-present Shanti Gilbert (https://github.com/shantigilbert)

. /etc/profile

ee_console enable
echo "Checking JDK..." > /dev/console

JDKDEST="/storage/roms/bios/jdk"
JDKNAME="zulu11.48.21-ca-jdk11.0.11"

mkdir -p ${JDKDEST}

# Check if the jdk does not already exists
[ "$(ls -A ${JDKDEST})" ] && JDKINSTALLED="yes" || JDKINSTALLED="no"

if [ ${JDKINSTALLED} == "no" ]; then

echo -e "GET http://google.com HTTP/1.0\n\n" | nc google.com 80 > /dev/null 2>&1
if [ $? -ne 0 ]; then
    echo "No internet connection, exiting..." > /dev/console
    text_viewer -e -w -t "No Internet!" -m "You need to be connected to the internet to download the JDK\nNo internet connection, exiting...";
    exit 1
fi

    echo "Downloading JDK please be patient..." > /dev/console
    cd ${JDKDEST}/..
    wget "https://cdn.azul.com/zulu-embedded/bin/zulu11.48.21-ca-jdk11.0.11-linux_aarch64.tar.gz" > /dev/console 2>&1
    echo "Inflating JDK please be patient..." > /dev/console
    tar xvfz ${JDKNAME}-linux_aarch64.tar.gz zulu11.48.21-ca-jdk11.0.11-linux_aarch64/lib > /dev/console 2>&1
    tar xvfz ${JDKNAME}-linux_aarch64.tar.gz zulu11.48.21-ca-jdk11.0.11-linux_aarch64/bin > /dev/console 2>&1
    tar xvfz ${JDKNAME}-linux_aarch64.tar.gz zulu11.48.21-ca-jdk11.0.11-linux_aarch64/conf > /dev/console 2>&1
    rm zulu11.48.21-ca-jdk11.0.11-linux_aarch64/lib/*.zip
    mv ${JDKNAME}-linux_aarch64/* jdk
    rm -rf ${JDKNAME}-linux_aarch64*
    
    for del in jmods include demo legal man DISCLAIMER LICENSE readme.txt release Welcome.html; do
        rm -rf ${JDKDEST}/${del}
    done
    echo "JDK done! loading core!" > /dev/console
    cp -rf /usr/lib/libretro/freej2me-lr.jar /storage/roms/bios
fi

clear > /dev/console < /dev/null 2>&1
echo "Loading..." > /dev/console
ee_console disable
exit 0
