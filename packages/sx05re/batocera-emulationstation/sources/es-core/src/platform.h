#pragma once
#ifndef ES_CORE_PLATFORM_H
#define ES_CORE_PLATFORM_H

#include <string>

#ifdef WIN32
#include <Windows.h>
#include <intrin.h>

#define sleep Sleep
#endif

int runShutdownCommand(); // shut down the system (returns 0 if successful)
int runRestartCommand(); // restart the system (returns 0 if successful)
int runSystemCommand(const std::string& cmd_utf8); // run a utf-8 encoded in the shell (requires wstring conversion on Windows)
int quitES(const std::string& filename);
void touch(const std::string& filename);

#endif // ES_CORE_PLATFORM_H
