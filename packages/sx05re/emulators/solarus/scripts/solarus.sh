#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2021-present Shanti Gilbert (https://github.com/shantigilbert)


[[ ! -d "/emuelec/configs/solarus/saves/" ]] && mkdir -p "/emuelec/configs/solarus/saves/"
solarus-run -fullscreen=yes ${1}

exit 0
