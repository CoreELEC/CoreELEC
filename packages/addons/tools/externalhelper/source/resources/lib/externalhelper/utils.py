# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

import json
import os
import subprocess

import xbmc
import xbmcaddon
import xbmcgui

ADDON_ID = 'tools.externalhelper'
addon = xbmcaddon.Addon(ADDON_ID)
RUN_EXTERNAL = os.path.join(addon.getAddonInfo('path'), 'bin', 'kodi-run-external')


def get_kodi_setting(setting: str):
    query = '{"jsonrpc":"2.0", "method":"Settings.GetSettingValue", "params":{"setting":"' + setting + '"}, "id":1}'

    rpc_result = xbmc.executeJSONRPC(query)
    json_result = json.loads(rpc_result)

    if 'result' in json_result and 'value' in json_result['result']:
        return json_result['result']['value']
    else:
        return None


def get_confirm_start_setting() -> bool:
    confirm = addon.getSettingBool('confirmstart')
    if confirm is None:
        return True
    else:
        return confirm


def get_display_config_setting() -> str | None:
    config = addon.getSettingInt('displayconfig')
    match config:
        case None:
            return None
        case 0:
            return 'default'
        case 1:
            return '3840x2160'
        case 2:
            return '1920x1080'
        case 3:
            return '1280x720'
        case 999:
            custom = addon.getSettingString('customdisplayconfig')
            if custom is None or custom.strip() == '':
                return None
            else:
                return custom
        case _:
            xbmc.log(f'invalid displayconfig {config}', xbmc.LOGERROR)
            return None


def get_kodi_keyboard_layout() -> str | None:
    return get_kodi_setting('input.libinputkeyboardlayout')


def get_kodi_audio_device() -> str | None:
    device = get_kodi_setting('audiooutput.audiodevice')

    if device is None:
        return None

    device = device.split('|')[0]

    if device.startswith('PULSE:') or device.startswith('PIPEWIRE:'):
        return device
    elif device.startswith('ALSA:'):
        alsadev = device[5:]
        if alsadev.startswith('@'):
            return 'ALSA:sysdefault'
        else:
            return device
    else:
        return 'ALSA:sysdefault'


def run_external_program(executable: str, args: list | None = None, env: dict | None = None, name: str = '') -> bool:
    addon = xbmcaddon.Addon(ADDON_ID)

    if get_confirm_start_setting():
        str = addon.getLocalizedString(30000).format(name=name)
        ok = xbmcgui.Dialog().yesno(name, str)
        if not ok:
            return False

    # try to determine keyboard layout
    layout = get_kodi_keyboard_layout()
    audiodev = get_kodi_audio_device()
    displayconfig = get_display_config_setting()

    environment = {}

    if displayconfig is not None:
        environment['KODI_DISPLAY_CONFIG'] = displayconfig
    if layout is not None:
        environment['KODI_KEYBOARD_LAYOUT'] = layout
    if audiodev is not None:
        environment['KODI_AUDIO_DEVICE'] = audiodev

    if env is not None:
        environment.update(env)

    cmd = ['systemd-run', '-u', 'kodi-run-external']

    for k, v in environment.items():
        cmd.append(f'-E{k}={v}')

    cmd.append(RUN_EXTERNAL)
    cmd.append(executable)

    if args is not None:
        cmd.extend(args)

    xbmc.log(f'starting {cmd!r}')

    result = subprocess.run(cmd, shell=False, close_fds=True, capture_output=True, text=True)
    if result.returncode != 0:
        xbmc.log(f'failed to start {cmd!r}: {result.stderr.strip()}', xbmc.LOGERROR)
    return result.returncode == 0
