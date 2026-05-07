# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

import sys
import urllib.parse

import xbmc
import xbmcgui
import xbmcplugin
from resources.lib.flatpakhelper import gui as flatpakgui

base_url = sys.argv[0]
addon_handle = int(sys.argv[1])
args = dict(urllib.parse.parse_qsl(sys.argv[2][1:]))

gui = flatpakgui.FlatpakGui()
fp = gui.flatpak


def build_url(query) -> str:
    return base_url + '?' + urllib.parse.urlencode(query)


def show_apps():
    apps = fp.get_installed_applications()
    if apps is None:
        gui.toast(gui.ls(30100))
        return

    items = []

    apps.sort(key=lambda a: a.get('name'))

    if len(apps) > 0:
        updateallurl = build_url({'action': 'updateall'})
        item = xbmcgui.ListItem(gui.ls(30101))
        item.setArt({'icon': gui.addon_icon, 'thumb': gui.addon_icon})
        items.append((updateallurl, item, False))

    for a in apps:
        appid = a.get('appid')
        name = a.get('name')
        item = xbmcgui.ListItem(name)
        icon = fp.find_application_icon(appid)
        if icon is None:
            icon = gui.addon_icon

        item.setArt({'icon': icon, 'thumb': icon})

        updateurl = build_url({'action': 'update', 'appid': appid})
        uninstallurl = build_url({'action': 'uninstall', 'appid': appid})
        starturl = build_url({'action': 'start', 'appid': appid})
        infourl = build_url({'action': 'info', 'appid': appid})

        item.addContextMenuItems(
            [
                (gui.ls(30102), f'RunPlugin({infourl})'),
                (gui.ls(30103), f'RunPlugin({updateurl})'),
                (gui.ls(30104), f'RunPlugin({uninstallurl})'),
            ],
            replaceItems=True,
        )
        items.append((starturl, item, False))

    xbmcplugin.addDirectoryItems(addon_handle, items, len(items))
    xbmcplugin.endOfDirectory(addon_handle)


def refresh():
    xbmc.executebuiltin('Container.Refresh')


def start():
    fp.start(args.get('appid'), name=args.get('name'))


if __name__ == '__main__':
    xbmc.log(f'flatpak plugin: args = {args!r}')
    action = args.get('action', None)
    match action:
        case None:
            show_apps()
        case 'info':
            gui.show_info(args['appid'])
        case 'update':
            gui.check_for_updates(args['appid'])
            refresh()
        case 'updateall':
            gui.confirm_and_update_all()
            refresh()
        case 'uninstall':
            gui.confirm_and_uninstall(args['appid'])
            refresh()
        case 'start':
            gui.start(args['appid'])
        case _:
            xbmc.log(f'unknown action {action}')
