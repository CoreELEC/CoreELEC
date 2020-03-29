#!/bin/sh

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

EE_VERSION1=$(cat /storage/.config/EE_VERSION)
EE_VERSION2=$(cat /usr/config/EE_VERSION)

if [ "$EE_VERSION1" != "$EE_VERSION2" ]; then

rm -rf /emuelec/scripts/*
cp -rf /usr/config/emuelec/scripts/* /emuelec/scripts

fi
