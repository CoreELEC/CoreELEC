#!/bin/sh

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

ARG=${1//[\\]/}
         
SDL_AUDIODRIVER=alsa PPSSPPSDL --fullscreen "$ARG"
