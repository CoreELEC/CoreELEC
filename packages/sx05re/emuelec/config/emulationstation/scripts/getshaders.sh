#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

find /tmp/shaders -name '*.glslp' -print0 | 
    while IFS= read -r -d '' line; do 
    echo ${line#/tmp/shaders/},
done

