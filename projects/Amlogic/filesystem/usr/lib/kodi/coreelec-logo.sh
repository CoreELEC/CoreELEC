#!/bin/sh

################################################################################
#      This file is part of CoreELEC - http://coreelec.org
#      Copyright (C) 2018-present Team CoreELEC
#
#  CoreELEC is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 2 of the License, or
#  (at your option) any later version.
#
#  CoreELEC is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with LibreELEC.  If not, see <http://www.gnu.org/licenses/>.
################################################################################

if [ ! -e /storage/.cache/coreelec_init ]; then
  if [ -e /storage/.kodi/userdata/Database/Textures13.db ]; then
    CACHEDURL=`/usr/bin/sqlite3 /storage/.kodi/userdata/Database/Textures13.db "SELECT cachedurl FROM texture WHERE url = '/usr/share/kodi/addons/service.libreelec.settings/resources/icon.png';"`
    CACHEDFILE=/storage/.kodi/userdata/Thumbnails/$CACHEDURL
    if [ -e $CACHEDFILE ]; then
      rm $CACHEDFILE
      /usr/bin/sqlite3 /storage/.kodi/userdata/Database/Textures13.db "DELETE FROM texture WHERE url = '/usr/share/kodi/addons/service.libreelec.settings/resources/icon.png';"
    fi
  fi
fi
touch /storage/.cache/coreelec_init
