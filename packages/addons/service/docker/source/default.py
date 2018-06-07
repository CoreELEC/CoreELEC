################################################################################
#      This file is part of LibreELEC - https://libreelec.tv
#      Copyright (C) 2009-2016 Lukas Rusak (lrusak@libreelec.tv)
#
#  LibreELEC is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 2 of the License, or
#  (at your option) any later version.
#
#  LibreELEC is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with LibreELEC.  If not, see <http://www.gnu.org/licenses/>.
################################################################################

import os
import subprocess
import sys
import threading
import time
import xbmc
import xbmcaddon
import xbmcgui

sys.path.append('/usr/share/kodi/addons/service.coreelec.settings')
import oe

__author__      = 'lrusak'
__addon__       = xbmcaddon.Addon()
__path__        = __addon__.getAddonInfo('path')

sys.path.append(__path__ + '/lib')
import dockermon

# docker events for api 1.23 (docker version 1.11.x)
# https://docs.docker.com/engine/reference/api/docker_remote_api_v1.23/#monitor-docker-s-events

docker_events = {
                  'container': {
                                 'string': 30030,
                                 'event': {
                                            'attach':       {
                                                              'string': 30031,
                                                              'enabled': '',
                                                            },
                                            'commit':       {
                                                              'string': 30032,
                                                              'enabled': '',
                                                            },
                                            'copy':         {
                                                              'string': 30033,
                                                              'enabled': '',
                                                            },
                                            'create':       {
                                                              'string': 30034,
                                                              'enabled': '',
                                                            },
                                            'destroy':      {
                                                              'string': 30035,
                                                              'enabled': '',
                                                            },
                                            'die':          {
                                                              'string': 30036,
                                                              'enabled': '',
                                                            },
                                            'exec_create':  {
                                                              'string': 30037,
                                                              'enabled': '',
                                                            },
                                            'exec_start':   {
                                                              'string': 30038,
                                                              'enabled': '',
                                                            },
                                            'export':       {
                                                              'string': 30039,
                                                              'enabled': '',
                                                            },
                                            'kill':         {
                                                              'string': 30040,
                                                              'enabled': True,
                                                            },
                                            'oom':          {
                                                              'string': 30041,
                                                              'enabled': True,
                                                            },
                                            'pause':        {
                                                              'string': 30042,
                                                              'enabled': '',
                                                            },
                                            'rename':       {
                                                              'string': 30043,
                                                              'enabled': '',
                                                            },
                                            'resize':       {
                                                              'string': 30044,
                                                              'enabled': '',
                                                            },
                                            'restart':      {
                                                              'string': 30045,
                                                              'enabled': '',
                                                            },
                                            'start':        {
                                                              'string': 30046,
                                                              'enabled': True,
                                                            },
                                            'stop':         {
                                                              'string': 30047,
                                                              'enabled': True,
                                                            },
                                            'top':          {
                                                              'string': 30048,
                                                              'enabled': '',
                                                            },
                                            'unpause':      {
                                                              'string': 30049,
                                                              'enabled': '',
                                                            },
                                            'update':       {
                                                              'string': 30050,
                                                              'enabled': '',
                                                            },
                                          },
                               },
                  'image':     {
                                 'string': 30060,
                                 'event': {
                                            'delete':       {
                                                              'string': 30061,
                                                              'enabled': '',
                                                            },
                                            'import':       {
                                                              'string': 30062,
                                                              'enabled': '',
                                                            },
                                            'pull':         {
                                                              'string': 30063,
                                                              'enabled': True,
                                                            },
                                            'push':         {
                                                              'string': 30064,
                                                              'enabled': '',
                                                            },
                                            'tag':          {
                                                              'string': 30065,
                                                              'enabled': '',
                                                            },
                                            'untag':        {
                                                              'string': 30066,
                                                              'enabled': '',
                                                            },
                                          },
                               },
                  'volume':    {
                                 'string': 30070,
                                 'event': {
                                            'create':       {
                                                              'string': 30071,
                                                              'enabled': '',
                                                            },
                                            'mount':        {
                                                              'string': 30072,
                                                              'enabled': '',
                                                            },
                                            'unmount':      {
                                                              'string': 30073,
                                                              'enabled': '',
                                                            },
                                            'destroy':      {
                                                              'string': 30074,
                                                              'enabled': '',
                                                            },
                                          },
                               },
                  'network':   {
                                 'string': 30080,
                                 'event': {
                                            'create':       {
                                                              'string': 30081,
                                                              'enabled': '',
                                                            },
                                            'connect':      {
                                                              'string': 30082,
                                                              'enabled': '',
                                                            },
                                            'disconnect':   {
                                                              'string': 30083,
                                                              'enabled': '',
                                                            },
                                            'destroy':      {
                                                              'string': 30084,
                                                              'enabled': '',
                                                            },
                                          },
                                },
                }

def print_notification(json_data):
    event_string = docker_events[json_data['Type']]['event'][json_data['Action']]['string']
    if __addon__.getSetting('notifications') is '0': # default
        if docker_events[json_data['Type']]['event'][json_data['Action']]['enabled']:
            try:
                message = unicode(' '.join([__addon__.getLocalizedString(30010),
                                            json_data['Actor']['Attributes']['name'],
                                            '|',
                                            __addon__.getLocalizedString(30012),
                                            __addon__.getLocalizedString(event_string)]))
            except KeyError as e:
                message = unicode(' '.join([__addon__.getLocalizedString(30011),
                                            json_data['Type'],
                                            '|',
                                            __addon__.getLocalizedString(30012),
                                            __addon__.getLocalizedString(event_string)]))

    elif __addon__.getSetting('notifications') is '1': # all
        try:
            message = unicode(' '.join([__addon__.getLocalizedString(30010),
                                        json_data['Actor']['Attributes']['name'],
                                        '|',
                                        __addon__.getLocalizedString(30012),
                                        __addon__.getLocalizedString(event_string)]))
        except KeyError as e:
            message = unicode(' '.join([__addon__.getLocalizedString(30011),
                                        json_data['Type'],
                                        '|',
                                        __addon__.getLocalizedString(30012),
                                        __addon__.getLocalizedString(event_string)]))

    elif __addon__.getSetting('notifications') is '2': # none
        pass

    elif __addon__.getSetting('notifications') is '3': # custom
        if __addon__.getSetting(json_data['Action']) == 'true':
            try:
                message = unicode(' '.join([__addon__.getLocalizedString(30010),
                                            json_data['Actor']['Attributes']['name'],
                                            '|',
                                            __addon__.getLocalizedString(30012),
                                            __addon__.getLocalizedString(event_string)]))
            except KeyError as e:
                message = unicode(' '.join([__addon__.getLocalizedString(30011),
                                            json_data['Type'],
                                            '|',
                                            __addon__.getLocalizedString(30012),
                                            __addon__.getLocalizedString(event_string)]))

    dialog = xbmcgui.Dialog()
    try:
        if message is not '':
            length = int(__addon__.getSetting('notification_length')) * 1000
            dialog.notification('Docker', message, '/storage/.kodi/addons/service.system.docker/resources/icon.png', length)
            xbmc.log('## service.system.docker ## ' + unicode(message))
    except NameError as e:
        pass

class dockermonThread(threading.Thread):

    def __init__(self):
        threading.Thread.__init__(self)
        self._is_running = True

    def run(self):
        while self._is_running:
            dockermon.watch(print_notification)

    def stop(self):
        self._is_running = False

class Main(object):

    def __init__(self, *args, **kwargs):

        #############################
        # Temp cleanup for old method

        restart_docker = False

        if os.path.islink('/storage/.config/system.d/service.system.docker.socket'):
            os.remove('/storage/.config/system.d/service.system.docker.socket')
        if os.path.islink('/storage/.config/system.d/docker.socket'):
            os.remove('/storage/.config/system.d/docker.socket')

        if os.path.islink('/storage/.config/system.d/service.system.docker.service'):
            if 'systemd' in os.readlink('/storage/.config/system.d/service.system.docker.service'):
                os.remove('/storage/.config/system.d/service.system.docker.service')
                restart_docker = True

        if os.path.islink('/storage/.config/system.d/docker.service'):
            if 'systemd' in os.readlink('/storage/.config/system.d/docker.service'):
                os.remove('/storage/.config/system.d/docker.service')
                restart_docker = True

        if os.path.islink('/storage/.config/system.d/multi-user.target.wants/service.system.docker.service'):
            if 'systemd' in os.readlink('/storage/.config/system.d/multi-user.target.wants/service.system.docker.service'):
                os.remove('/storage/.config/system.d/multi-user.target.wants/service.system.docker.service')
                restart_docker = True

        if restart_docker:
            oe.execute('systemctl enable  /storage/.kodi/addons/service.system.docker/system.d/service.system.docker.service')
            oe.execute('systemctl restart /storage/.kodi/addons/service.system.docker/system.d/service.system.docker.service')

        # end temp cleanup
        #############################

        monitor = DockerMonitor(self)

        while not monitor.abortRequested():
            if monitor.waitForAbort():
                # we don't want to stop or disable docker while it's installed
                pass

class DockerMonitor(xbmc.Monitor):

    def __init__(self, *args, **kwargs):
        xbmc.Monitor.__init__(self)

    def onSettingsChanged(self):
        pass

if ( __name__ == "__main__" ):
    dockermonThread().start()
    Main()

    del DockerMonitor
    dockermonThread().stop()
