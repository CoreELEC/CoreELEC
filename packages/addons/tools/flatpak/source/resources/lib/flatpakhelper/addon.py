# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

import xbmcaddon

from .gui import FlatpakGui


class FlatpakAddon:
    def __init__(
        self,
        addon: xbmcaddon.Addon,
        appid: str,
        remote: str = 'flathub',
        remote_url: str = 'https://dl.flathub.org/repo/flathub.flatpakrepo',
    ):

        self.calling_addon = addon
        self.name = addon.getAddonInfo('name')

        self.appid = appid
        self.remote = remote
        self.remote_url = remote_url

        self.gui = FlatpakGui()

    def start(self, args: list | None = None, env: dict | None = None, run_env: dict | None = None) -> bool:

        return self.gui.start(
            appid=self.appid,
            name=self.name,
            remote=self.remote,
            remote_url=self.remote_url,
            args=args,
            env=env,
            run_env=run_env,
        )
