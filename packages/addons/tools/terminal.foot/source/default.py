# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

import externalhelper.utils as eh
import os
import xbmcaddon

addon = xbmcaddon.Addon()
addonpath = addon.getAddonInfo('path')
name = addon.getAddonInfo('name')
exe = os.path.join(addonpath, 'bin', 'foot')

fontsize = addon.getSettingInt('fontsize')
if not fontsize:
  fontsize = 12

args = ['-F', '-f', f'monospace:size={fontsize}', '-D', '/storage']

eh.run_external_program(executable=exe, args=args, name=name)
