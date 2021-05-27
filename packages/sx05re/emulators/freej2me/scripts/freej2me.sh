#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2021-present Shanti Gilbert (https://github.com/shantigilbert)

JDKDEST="/storage/roms/bios/jdk"
JDKNAME="zulu11.48.21-ca-jdk11.0.11"

# Check if the jdk does not already exists
[ "$(ls -A ${JDKDEST})" ] && JDKINSTALLED="yes" || JDKINSTALLED="no"

if [ ${JDKINSTALLED} == "no" ]; then
    mkdir ${JDKDEST}
    cd ${JDKDEST}/..
    wget "https://cdn.azul.com/zulu-embedded/bin/${JDKNAME}-linux_aarch64.tar.gz"
    tar xvfz ${JDKNAME}-linux_aarch64.tar.gz
    mv ${JDKNAME}-linux_aarch64 jdk
else
    exit 0
fi
