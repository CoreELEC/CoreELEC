#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

source /emuelec/scripts/env.sh
joy2keyStart

dialog --ascii-lines --msgbox "Use this link to ask for help:\n\n $(emueleclogs.sh)"  0 0
