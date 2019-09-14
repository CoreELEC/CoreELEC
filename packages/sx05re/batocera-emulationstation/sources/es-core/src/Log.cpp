#include "Log.h"

#include "utils/FileSystemUtil.h"
#include "platform.h"
#include <iostream>
#include "Settings.h"

LogLevel Log::reportingLevel = LogInfo;
bool Log::dirty = false;
FILE* Log::file = NULL;

LogLevel Log::getReportingLevel()
{
	return reportingLevel;
}

std::string Log::getLogPath()
{
	//return Utils::FileSystem::getHomePath() + "/.config/emuelec/logs/es_log.txt"; /* < emuelec */
	if (Settings::getInstance()->getString("LogPath") != "") {
		return Settings::getInstance()->getString("LogPath") + "/es_log.txt";
	} else {
		std::string home = Utils::FileSystem::getHomePath();
		return home + "/.emulationstation/es_log.txt";
	}
}

void Log::setReportingLevel(LogLevel level)
{
	reportingLevel = level;
}

void Log::init()
{
	if (file != NULL)
		close();

	if (!Settings::getInstance()->getBool("EnableLogging"))
	{
		remove(getLogPath().c_str());
		return;
	}

	remove((getLogPath() + ".bak").c_str());

	// rename previous log file
	rename(getLogPath().c_str(), (getLogPath() + ".bak").c_str());

	file = fopen(getLogPath().c_str(), "w");
	dirty = false;
}

std::ostringstream& Log::get(LogLevel level)
{
	os << "lvl" << level << ": \t";
	messageLevel = level;

	return os;
}

void Log::flush()
{
	if (!dirty)
		return;

	fflush(file);
	dirty = false;
}

void Log::close()
{
	if (file != NULL)
	{
		fflush(file);
		fclose(file);
	}

	dirty = false;
	file = NULL;
}

Log::~Log()
{
	if (file != NULL)
	{
		os << std::endl;
		fprintf(file, "%s", os.str().c_str());
		dirty = true;
	}

	// If it's an error, also print to console
	// print all messages if using --debug
	if(messageLevel == LogError || reportingLevel >= LogDebug)
		fprintf(stderr, "%s", os.str().c_str());
}
