# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2020-present Team LibreELEC (https://libreelec.tv)

import subprocess
import xbmc
import xbmcaddon


def systemctl(command):
    subprocess.call(
        ['systemctl', command, xbmcaddon.Addon().getAddonInfo('id')])


class Monitor(xbmc.Monitor):

    def onSettingsChanged(self):
        systemctl('restart')


if __name__ == '__main__':
    Monitor().waitForAbort()
