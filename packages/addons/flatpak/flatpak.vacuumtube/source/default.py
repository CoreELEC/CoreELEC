# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

from flatpakhelper.addon import FlatpakAddon
import xbmcaddon

flatpak = FlatpakAddon(addon=xbmcaddon.Addon(), appid="rocks.shy.VacuumTube")
flatpak.start(args=['--no-sandbox'])
