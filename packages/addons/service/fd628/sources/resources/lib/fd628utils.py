################################################################################
#      This file is part of CoreELEC - http://coreelec.org
#      Copyright (C) 2018-present Arthur Liberman (arthur_liberman@hotmail.com)
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
#  along with CoreELEC.  If not, see <http://www.gnu.org/licenses/>.
################################################################################

import xbmc
import xbmcaddon
import threading

addonName = xbmcaddon.Addon(id='service.fd628').getAddonInfo('name')

def kodiLog(message, level = xbmc.LOGDEBUG):
	xbmc.log('{0} -> {1}'.format(addonName, str(message)), level)

def kodiLogError(message):
	kodiLog(message, xbmc.LOGERROR)

def kodiLogWarning(message):
	kodiLog(message, xbmc.LOGWARNING)

def kodiLogNotice(message):
	kodiLog(message, xbmc.LOGNOTICE)

class fd628Timer(object):
	def __init__(self, interval, function, args=[], kwargs={}):
		self._timer = threading.Timer(interval, self._callback, args, kwargs)
		self.interval = interval
		self.function = function
		self.args = args
		self.kwargs = kwargs
		self._stopped = True
		self._isReady = True

	def setInterval(self, interval):
		self.interval = interval

	def reset(self):
		self.cancel()
		self._timer = threading.Timer(self.interval, self._callback, self.args, self.kwargs)
		self._isReady = True

	def cancel(self):
		self._timer.cancel()
		self._stopped = True

	def start(self):
		self._isReady = False
		self._stopped = False
		self._timer.start()

	def isAlive(self):
		return not self._stopped

	def isReady(self):
		return self._isReady

	def _callback(self):
		self._stopped = True
		self.function(*self.args, **self.kwargs)
