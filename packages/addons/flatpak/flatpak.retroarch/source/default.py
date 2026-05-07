# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

from flatpakhelper.addon import FlatpakAddon
import xbmcaddon

flatpak=FlatpakAddon(addon=xbmcaddon.Addon(), appid='org.libretro.RetroArch')
flatpak.start()

