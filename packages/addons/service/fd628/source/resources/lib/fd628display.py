################################################################################
#      This file is part of CoreELEC - http://coreelec.org
#      Copyright (C) 2018 Arthur Liberman (arthur_liberman (at) hotmail.com)
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

import ctypes
import struct
import threading
import xbmc
from fd628utils import *

class _fd628_time_date(ctypes.Structure):
	_fields_ = [
		("seconds", ctypes.c_uint8),
		("minutes", ctypes.c_uint8),
		("hours", ctypes.c_uint8),
		("day_of_week", ctypes.c_uint8),
		("day", ctypes.c_uint8),
		("months", ctypes.c_uint8),
		("year", ctypes.c_ushort)
	]

class _fd628_time_secondary(ctypes.Structure):
	_fields_ = [
		("seconds", ctypes.c_uint8),
		("minutes", ctypes.c_uint8),
		("hours", ctypes.c_uint8),
		("_reserved", ctypes.c_uint8)
	]

class _fd628_channel_data(ctypes.Structure):
	_fields_ = [
		("channel", ctypes.c_ushort),
		("channel_count", ctypes.c_ushort)
	]

class fd628_display_data(ctypes.Structure):
	_fields_ = [
		("mode", ctypes.c_ushort),
		("colon_on", ctypes.c_uint8),
		("temperature", ctypes.c_uint8),
		("time_date", _fd628_time_date),
		("time_secondary", _fd628_time_secondary),
		("channel_data", _fd628_channel_data),
		("string_main", ctypes.c_char * 512),
		("string_secondary", ctypes.c_char * 128)
	]

	def __init__(self):
		super(ctypes.Structure, self).__init__()
		self._buffer = ctypes.create_string_buffer(ctypes.sizeof(self))

	def getBuffer(self):
		ctypes.memmove(self._buffer, ctypes.addressof(self), ctypes.sizeof(self))
		return self._buffer.raw

class fd628DisplayModeBase(object):
	def __init__(self, manager, settings):
		self._manager = manager
		self._settings = settings
		self._data = fd628_display_data()
		self._timerInterval = fd628Timer(0, self._show)
		self._timerHide = fd628Timer(0, self._hide)
		self._intervalShow = 0
		self._intervalDuration = 0
		self._enabled = True

	def onSettingsChanged(self):
		kodiLog('{0}: In onSettingsChanged'.format(str(self)))
		self._manager.removeLayer(self)
		self._timerInterval.cancel()
		self._timerHide.cancel()
		self._timerInterval.setInterval(self._intervalShow - self._intervalDuration)
		self._timerHide.setInterval(self._intervalDuration)
		self.startShowTimer()

	def enable(self, state):
		if (self._enabled != state):
			self._enabled = state
			self.onSettingsChanged()

	def startShowTimer(self):
		kodiLog('{0}: In startShowTimer'.format(str(self)))
		if (self._enabled and self._intervalShow > 0 and not self._timerInterval.isAlive()):
			self._timerInterval.reset()
			self._timerInterval.start()

	def startHideTimer(self):
		kodiLog('{0}: In startHideTimer'.format(str(self)))
		if (not self._timerHide.isAlive()):
			self._timerHide.reset()
			self._timerHide.start()

	def update(self):
		raise NotImplementedError

	def isAlwaysOnTop(self):
		return False

	def getDataBuffer(self):
		return self._data.getBuffer()

	def _show(self):
		kodiLog('{0}: In _show'.format(str(self)))
		if (self._intervalShow > 0):
			self._manager.addLayer(self)

	def _hide(self):
		kodiLog('{0}: In _hide'.format(str(self)))
		self._manager.removeLayer(self)

	def _stopTimers(self):
		kodiLog('{0}: In _stopTimers'.format(str(self)))
		self._timerInterval.reset()
		self._timerHide.reset()

class fd628DisplayModeTemperature(fd628DisplayModeBase):
	def __init__(self, manager, settings):
		super(fd628DisplayModeTemperature, self).__init__(manager, settings)
		self._data.mode = 5
		self.onSettingsChanged()

	def onSettingsChanged(self):
		self._intervalShow = self._settings.getModeTempInterval()
		self._intervalDuration = self._settings.getModeTempDuration()
		super(fd628DisplayModeTemperature, self).onSettingsChanged()

	def update(self):
		if (self._enabled):
			self._data.temperature = 0
			try:
				with open("/sys/class/thermal/thermal_zone0/temp", "r") as temp:
					self._data.temperature = int(temp.read()) / 1000
			except Exception as inst:
				kodiLogError(inst)

	def __str__(self):
		return 'fd628DisplayModeTemperature'

class fd628DisplayModeDate(fd628DisplayModeBase):
	def __init__(self, manager, settings):
		super(fd628DisplayModeDate, self).__init__(manager, settings)
		self.onSettingsChanged()

	def onSettingsChanged(self):
		self._intervalShow = self._settings.getModeDateInterval()
		self._intervalDuration = self._settings.getModeDateDuration()
		self._data = ctypes.create_string_buffer(struct.pack('BBB', 2, 6, self._settings.getModeDateFormat()))
		super(fd628DisplayModeDate, self).onSettingsChanged()

	def update(self):
		pass

	def getDataBuffer(self):
		return self._data.raw

	def __str__(self):
		return 'fd628DisplayModeDate'

class fd628DisplayModePlaybackTime(xbmc.Player):
	def __init__(self):
		super(xbmc.Player, self).__init__()
		self._data = fd628_display_data()
		self._data.mode = 3
		self._timerHide = None
		self._enabled = True

	def factory(manager, settings):
		instance = fd628DisplayModePlaybackTime()
		instance._manager = manager
		instance._settings = settings
		instance.onSettingsChanged()
		return instance
	factory = staticmethod(factory)

	def onSettingsChanged(self):
		if (self._settings.isPlaybackTimeEnabled()):
			self.onPlayBackStarted()
		else:
			self.onPlayBackStopped()

	def onPlayBackStarted(self):
		self._stopTimers()
		if (self._isPlayingWithTimeEnabled()):
			self._manager.addLayer(self)

	def onPlayBackStopped(self):
		self._stopTimers()
		self._data.string_main = "\0"
		self._manager.removeLayer(self)

	def enable(self, state):
		if (self._enabled != state):
			self._enabled = state
			if (state):
				self.onPlayBackStarted()
			else:
				self.onPlayBackStopped()

	def startShowTimer(self):
		pass

	def startHideTimer(self):
		if (self._settings.getModePlaybackTimeDuration() > 0):
			self._timerHide = threading.Timer(self._settings.getModePlaybackTimeDuration(), self.onPlayBackStopped)
			self._timerHide.start()

	def update(self):
		if (self.isPlaying() and self._enabled):
			behavior = self._settings.getModePlaybackTimeBehavior()
			time = self.getTime()
			totalTime = self.getTotalTime()
			if (behavior == 0):
				time = totalTime - time
			elif (behavior == 1):
				pass # keep "time = self.getTime()"
			elif (behavior == 2):
				time = totalTime
			self._getTimeFromSeconds(self._data.time_date, time)
			self._getTimeFromSeconds(self._data.time_secondary, totalTime)
			if (self._data.time_date.hours > 0):
				self._data.colon_on = not self._data.colon_on
			else:
				self._data.colon_on = 1
			infoTag = None
			if (self.isPlayingVideo()):
				try:
					infoTag = self.getVideoInfoTag()
				except Exception as inst:
					kodiLog(inst)
			elif (self.isPlayingAudio()):
				try:
					infoTag = self.getMusicInfoTag()
				except Exception as inst:
					kodiLog(inst)
			if (infoTag is not None):
				self._data.string_main = unicode(infoTag.getTitle(), "utf-8").encode("ascii", "ignore")
			else:
				self._data.string_main = "\0"

	def isAlwaysOnTop(self):
		return self._settings.getModePlaybackTimeDuration() == 0

	def getDataBuffer(self):
		return self._data.getBuffer()

	def _isPlayingWithTimeEnabled(self):
		return (self._enabled and self._settings.isPlaybackTimeEnabled() and
			self.isPlaying() and not self.getPlayingFile().startswith("pvr://"))

	def _getTimeFromSeconds(self, time, secs):
		time.hours = int(secs / 3600)
		secs = secs - (time.hours * 3600)
		time.minutes = int(secs / 60)
		secs = secs - (time.minutes * 60)
		time.seconds = int(secs)

	def _stopTimers(self):
		if (self._timerHide is not None):
			self._timerHide.cancel()
			self._timerHide = None

class fd628DisplayManager(object):
	def __init__(self, pipePath, rlock):
		self._layerStack = []
		self._pipePath = pipePath
		self._containsAlwaysOnTopLayer = False
		self._rlock = rlock

	def addLayer(self, mode):
		if (self._rlock.acquire()):
			if (not mode in self._layerStack):
				if (self._containsAlwaysOnTopLayer):
					if (mode.isAlwaysOnTop()):
						kodiLogNotice("Display manager already contains an always on top layer, new layer ignored.")
					else:
						self._layerStack.insert(len(self._layerStack) - 1, mode)
				else:
					self._layerStack.append(mode)
					self._containsAlwaysOnTopLayer = self._containsAlwaysOnTopLayer or mode.isAlwaysOnTop()
			self._update(True)
			self._rlock.release()

	def removeLayer(self, mode):
		if (self._rlock.acquire()):
			if (mode in self._layerStack):
				self._layerStack.remove(mode)
				mode.startShowTimer()
				if (mode.isAlwaysOnTop()):
					self._containsAlwaysOnTopLayer = False
			self._update(True)
			self._rlock.release()

	def clear(self):
		if (self._rlock.acquire()):
			del self._layerStack[:]
			self._show(None)
			self._rlock.release()

	def update(self):
		self._update()

	def _update(self, forceUpdate = False):
		if (self._rlock.acquire()):
			if (len(self._layerStack) > 0):
				if (forceUpdate):
					self._layerStack[0].startHideTimer()
				self._layerStack[0].update()
				self._show(self._layerStack[0])
			elif (forceUpdate):
				self._show(None)
			self._rlock.release()

	def _show(self, mode):
		if (self._rlock.acquire()):
			try:
				with open(self._pipePath, "wb") as pipe:
					if (mode):
						pipe.write(mode.getDataBuffer())
					else:
						pipe.write("\0") #return to clock.
			except Exception as inst:
				kodiLogError(inst)
			self._rlock.release()
