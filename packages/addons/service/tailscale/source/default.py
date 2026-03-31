# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

import json
import subprocess
import sys
import xbmc
import xbmcaddon
import xbmcgui

SOCKET = '/run/tailscale/tailscaled.sock'


def select_exit_node():
    addon = xbmcaddon.Addon()
    tailscale = addon.getAddonInfo('path') + '/bin/tailscale'
    strings = addon.getLocalizedString

    try:
        result = subprocess.check_output(
            [tailscale, '--socket', SOCKET, 'status', '--json'],
            stderr=subprocess.STDOUT
        )
        data = json.loads(result)
    except Exception:
        xbmcgui.Dialog().ok('Tailscale', strings(30023))
        return

    peers = []
    for peer in data.get('Peer', {}).values():
        if peer.get('ExitNodeOption') and peer.get('Online'):
            dns_name = peer.get('DNSName', '').rstrip('.')
            machine_name = dns_name.split('.')[0] if dns_name else ''
            ips = peer.get('TailscaleIPs', [])
            ip = ips[0] if ips else ''
            if machine_name and ip:
                peers.append((machine_name, ip))

    if not peers:
        xbmcgui.Dialog().ok('Tailscale', strings(30024))
        return

    labels = [name for name, ip in peers]
    sel = xbmcgui.Dialog().select(strings(30025), labels)
    if sel >= 0:
        addon.setSetting('ts_exit_node_host', peers[sel][1])


class Monitor(xbmc.Monitor):

    def __init__(self, *args, **kwargs):
        xbmc.Monitor.__init__(self)
        self.addon = xbmcaddon.Addon()
        self.id = self.addon.getAddonInfo('id')

    def onSettingsChanged(self):
        strings = self.addon.getLocalizedString
        if xbmcgui.Dialog().yesno('Tailscale', strings(30026)):
            subprocess.call(['systemctl', 'restart', self.id])


if __name__ == '__main__':
    if len(sys.argv) > 1 and sys.argv[1] == 'select_exit_node':
        select_exit_node()
    else:
        Monitor().waitForAbort()
