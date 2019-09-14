#include "ApiSystem.h"
#include <stdlib.h>
#if !defined(WIN32)
#include <sys/statvfs.h>
#endif
#include <sstream>
#include "Settings.h"
#include <iostream>
#include <fstream>
#include "Log.h"
#include "HttpReq.h"

#include "AudioManager.h"
#include "VolumeControl.h"
#include "InputManager.h"
#include <SystemConf.h>

#include <stdio.h>
#include <sys/types.h>
#if !defined(WIN32)
#include <ifaddrs.h>
#include <netinet/in.h>
#endif
#include <string.h>
#if !defined(WIN32)
#include <arpa/inet.h>
#endif
#include "utils/FileSystemUtil.h"
#include "utils/StringUtil.h"
#include <fstream>
#include <SDL.h>
#include <Sound.h>

#if defined(WIN32)
#include <Windows.h>
#include "EmulationStation.h"
#define popen _popen
#define pclose _pclose
#endif

ApiSystem::ApiSystem() 
{
}

ApiSystem* ApiSystem::instance = NULL;

ApiSystem *ApiSystem::getInstance() 
{
	if (ApiSystem::instance == NULL)
		ApiSystem::instance = new ApiSystem();

	return ApiSystem::instance;
}

unsigned long ApiSystem::getFreeSpaceGB(std::string mountpoint) 
{
#if !defined(WIN32)
	struct statvfs fiData;
	const char *fnPath = mountpoint.c_str();
	int free = 0;
	if ((statvfs(fnPath, &fiData)) >= 0) {
		free = (fiData.f_bfree * fiData.f_bsize) / (1024 * 1024 * 1024);
	}
	return free;
#else

	unsigned __int64 i64FreeBytesToCaller, i64TotalBytes, i64FreeBytes;

	std::string drive = Utils::FileSystem::getHomePath()[0] + std::string(":");

	BOOL  fResult = GetDiskFreeSpaceExA(drive.c_str(),
		(PULARGE_INTEGER)&i64FreeBytesToCaller,
		(PULARGE_INTEGER)&i64TotalBytes,
		(PULARGE_INTEGER)&i64FreeBytes);

	if (fResult)
		return i64FreeBytes / (1024 * 1024 * 1024);

	return 0;
#endif
}

std::string ApiSystem::getFreeSpaceInfo() 
{
	std::string sharePart = "/storage/.config/emuelec/";
	if (sharePart.size() > 0) {
		const char *fnPath = sharePart.c_str();
#if !defined(WIN32)
		struct statvfs fiData;
		if ((statvfs(fnPath, &fiData)) < 0) {
			return "";
		}
		else {
			unsigned long total = (fiData.f_blocks * (fiData.f_bsize / 1024)) / (1024L * 1024L);
			unsigned long free = (fiData.f_bfree * (fiData.f_bsize / 1024)) / (1024L * 1024L);
			unsigned long used = total - free;
			unsigned long percent = 0;
			std::ostringstream oss;
			if (total != 0) {  //for small SD card ;) with share < 1GB
				percent = used * 100 / total;
				oss << used << "GB/" << total << "GB (" << percent << "%)";
			}
			else
				oss << "N/A";
			return oss.str();
		}
#else
		unsigned __int64 i64FreeBytesToCaller, i64TotalBytes, i64FreeBytes;

		std::string drive = Utils::FileSystem::getHomePath()[0] + std::string(":");

		BOOL  fResult = GetDiskFreeSpaceExA(drive.c_str(),
			(PULARGE_INTEGER)&i64FreeBytesToCaller,
			(PULARGE_INTEGER)&i64TotalBytes,
			(PULARGE_INTEGER)&i64FreeBytes);

		if (fResult)
		{
			unsigned long total = i64TotalBytes / (1024L * 1024L);
			unsigned long free = i64FreeBytes / (1024L * 1024L);
			unsigned long used = total - free;
			unsigned long percent = 0;
			std::ostringstream oss;
			if (total != 0) {  //for small SD card ;) with share < 1GB
				percent = used * 100 / total;
				oss << used << "GB/" << total << "GB (" << percent << "%)";
			}
			else
				oss << "N/A";

			return oss.str();
		}

		return "N/A";
#endif
	}
	else
		return "ERROR";	
}

bool ApiSystem::isFreeSpaceLimit() {
	std::string sharePart = "/storage/.config/emuelec/";
	if (sharePart.size() > 0) {
		return getFreeSpaceGB(sharePart) < 2;
	}
	else {
		return "ERROR";
	}

}

std::string ApiSystem::getVersion() 
{
#if WIN32
	return PROGRAM_VERSION_STRING;
#endif

	std::string version = "/usr/share/batocera/batocera.version";
	if (version.size() > 0) {
		std::ifstream ifs(version);

		if (ifs.good()) {
			std::string contents;
			std::getline(ifs, contents);
			return contents;
		}
	}
	return "";
}

bool ApiSystem::needToShowVersionMessage() {
	createLastVersionFileIfNotExisting();
	std::string versionFile = "/storage/.config/emuelec/logs/update.done";
	if (versionFile.size() > 0) {
		std::ifstream lvifs(versionFile);
		if (lvifs.good()) {
			std::string lastVersion;
			std::getline(lvifs, lastVersion);
			std::string currentVersion = getVersion();
			if (lastVersion == currentVersion) {
				return false;
			}
		}
	}
	return true;
}

bool ApiSystem::createLastVersionFileIfNotExisting() {
	std::string versionFile = "/storage/.config/emuelec/logs/update.done";

	FILE *file;
	if (file = fopen(versionFile.c_str(), "r")) {
		fclose(file);
		return true;
	}
	return updateLastVersionFile();
}

bool ApiSystem::updateLastVersionFile() {
	std::string versionFile = "/storage/.config/emuelec/logs/update.done";
	std::string currentVersion = getVersion();
	std::ostringstream oss;
	oss << "echo " << currentVersion << " > " << versionFile;
	if (system(oss.str().c_str())) {
		LOG(LogWarning) << "Error executing " << oss.str().c_str();
		return false;
	}
	else {
		LOG(LogError) << "Last version file updated";
		return true;
	}
}

bool ApiSystem::setOverscan(bool enable) {

	std::ostringstream oss;
	oss << "batocera-config" << " " << "overscan";
	if (enable) {
		oss << " " << "enable";
	}
	else {
		oss << " " << "disable";
	}
	std::string command = oss.str();
	LOG(LogInfo) << "Launching " << command;
	if (system(command.c_str())) {
		LOG(LogWarning) << "Error executing " << command;
		return false;
	}
	else {
		LOG(LogInfo) << "Overscan set to : " << enable;
		return true;
	}

}

bool ApiSystem::setOverclock(std::string mode) {
	if (mode != "") {
		std::ostringstream oss;
		oss << "batocera-overclock set " << mode;
		std::string command = oss.str();
		LOG(LogInfo) << "Launching " << command;
		if (system(command.c_str())) {
			LOG(LogWarning) << "Error executing " << command;
			return false;
		}
		else {
			LOG(LogInfo) << "Overclocking set to " << mode;
			return true;
		}
	}

	return false;
}


std::pair<std::string, int> ApiSystem::updateSystem(BusyComponent* ui) {
	std::string updatecommand = "batocera-upgrade";
	FILE *pipe = popen(updatecommand.c_str(), "r");
	char line[1024] = "";
	if (pipe == NULL) {
		return std::pair<std::string, int>(std::string("Cannot call update command"), -1);
	}

	FILE *flog = fopen("/storage/.config/emuelec/logs/batocera-upgrade.log", "w");
	while (fgets(line, 1024, pipe)) {
		strtok(line, "\n");
		if (flog != NULL) fprintf(flog, "%s\n", line);
		ui->setText(std::string(line));
	}
	if (flog != NULL) fclose(flog);

	int exitCode = pclose(pipe);
	return std::pair<std::string, int>(std::string(line), exitCode);
}

std::pair<std::string, int> ApiSystem::backupSystem(BusyComponent* ui, std::string device) {
	std::string updatecommand = std::string("batocera-sync sync ") + device;
	FILE *pipe = popen(updatecommand.c_str(), "r");
	char line[1024] = "";
	if (pipe == NULL) {
		return std::pair<std::string, int>(std::string("Cannot call sync command"), -1);
	}

	FILE *flog = fopen("/storage/.config/emuelec/logs/batocera-sync.log", "w");
	while (fgets(line, 1024, pipe)) {
		strtok(line, "\n");
		if (flog != NULL) fprintf(flog, "%s\n", line);
		ui->setText(std::string(line));
	}
	if (flog != NULL) fclose(flog);

	int exitCode = pclose(pipe);
	return std::pair<std::string, int>(std::string(line), exitCode);
}

std::pair<std::string, int> ApiSystem::installSystem(BusyComponent* ui, std::string device, std::string architecture) {
	std::string updatecommand = std::string("batocera-install install ") + device + " " + architecture;
	FILE *pipe = popen(updatecommand.c_str(), "r");
	char line[1024] = "";
	if (pipe == NULL) {
		return std::pair<std::string, int>(std::string("Cannot call install command"), -1);
	}

	FILE *flog = fopen("/storage/.config/emuelec/logs/batocera-install.log", "w");
	while (fgets(line, 1024, pipe)) {
		strtok(line, "\n");
		if (flog != NULL) fprintf(flog, "%s\n", line);
		ui->setText(std::string(line));
	}
	if (flog != NULL) fclose(flog);

	int exitCode = pclose(pipe);
	return std::pair<std::string, int>(std::string(line), exitCode);
}

std::pair<std::string, int> ApiSystem::scrape(BusyComponent* ui) {
	std::string scrapecommand = std::string("batocera-scraper");
	FILE *pipe = popen(scrapecommand.c_str(), "r");
	char line[1024] = "";
	if (pipe == NULL) {
		return std::pair<std::string, int>(std::string("Cannot call scrape command"), -1);
	}

	FILE *flog = fopen("/storage/.config/emuelec/logs/batocera-scraper.log", "w");
	while (fgets(line, 1024, pipe)) {
		strtok(line, "\n");
		if (flog != NULL) fprintf(flog, "%s\n", line);


		if (Utils::String::startsWith(line, "GAME: ")) {
			ui->setText(std::string(line));
		}
	}
	if (flog != NULL) fclose(flog);

	int exitCode = pclose(pipe);
	return std::pair<std::string, int>(std::string(line), exitCode);
}

bool ApiSystem::ping() 
{
#if WIN32
	bool connected = false;

	HMODULE hWinInet = LoadLibrary("WININET.DLL");
	if (hWinInet != NULL)
	{
		typedef BOOL(WINAPI *PF_INETGETCONNECTEDSTATE)(LPDWORD, DWORD);
		PF_INETGETCONNECTEDSTATE pfInternetGetConnectedState;

		pfInternetGetConnectedState = (PF_INETGETCONNECTEDSTATE) ::GetProcAddress(hWinInet, "InternetGetConnectedState");
		// affectation du pointeur sur la fonction
		if (pfInternetGetConnectedState != NULL)
		{
			DWORD TypeCon;
			if (pfInternetGetConnectedState(&TypeCon, 0))
				connected = true;			
		}

		FreeLibrary(hWinInet);
	}

	return connected;
#endif

	std::string updateserver = "batocera-linux.xorhub.com";
	std::string s("timeout 1 fping -c 1 -t 1000 " + updateserver);
	int exitcode = system(s.c_str());
	return exitcode == 0;
}

bool ApiSystem::canUpdate(std::vector<std::string>& output) {
	int res;
	int exitCode;
	std::ostringstream oss;
	oss << "batocera-config" << " " << "canupdate";
	std::string command = oss.str();
	LOG(LogInfo) << "Launching " << command;

	FILE *pipe = popen(oss.str().c_str(), "r");
	char line[1024];

	if (pipe == NULL) {
		return false;
	}

	while (fgets(line, 1024, pipe)) {
		strtok(line, "\n");
		output.push_back(std::string(line));
	}
	exitCode = pclose(pipe);

#ifdef WIN32
	res = exitCode;
#else
	res = WEXITSTATUS(exitCode);
#endif

	if (res == 0) {
		LOG(LogInfo) << "Can update ";
		return true;
	}
	else {
		LOG(LogInfo) << "Cannot update ";
		return false;
	}
}

void ApiSystem::launchExternalWindow_before(Window *window) {
	AudioManager::getInstance()->deinit();
	VolumeControl::getInstance()->deinit();
	window->deinit();
}

void ApiSystem::launchExternalWindow_after(Window *window) {
	window->init();
	VolumeControl::getInstance()->init();
	AudioManager::getInstance()->init();
	window->normalizeNextUpdate();

	if (SystemConf::getInstance()->get("audio.bgmusic") != "0") {
		AudioManager::getInstance()->playRandomMusic();
	}
}

bool ApiSystem::launchKodi(Window *window) {
	std::string commandline = InputManager::getInstance()->configureEmulators();
	std::string command = "python /usr/lib/python2.7/site-packages/configgen/emulatorlauncher.py -system kodi -rom '' " + commandline;

	ApiSystem::launchExternalWindow_before(window);

	int exitCode = system(command.c_str());
#if !defined(WIN32)
	// WIFEXITED returns a nonzero value if the child process terminated normally with exit or _exit.
	// https://www.gnu.org/software/libc/manual/html_node/Process-Completion-Status.html
	if (WIFEXITED(exitCode)) {
		exitCode = WEXITSTATUS(exitCode);
	}
#endif

	ApiSystem::launchExternalWindow_after(window);

	// handle end of kodi
	switch (exitCode) {
	case 10: // reboot code
		reboot();
		return true;
		break;
	case 11: // shutdown code
		shutdown();
		return true;
		break;
	}

	return exitCode == 0;

}

bool ApiSystem::launchFileManager(Window *window) {
	std::string command = "filemanagerlauncher";

	ApiSystem::launchExternalWindow_before(window);

	int exitCode = system(command.c_str());
#if !defined(WIN32)
	if (WIFEXITED(exitCode)) {
		exitCode = WEXITSTATUS(exitCode);
	}
#endif

	ApiSystem::launchExternalWindow_after(window);

	return exitCode == 0;
}

bool ApiSystem::enableWifi(std::string ssid, std::string key) {
	std::ostringstream oss;

	Utils::String::replace(ssid, "\"", "\\\"");
	Utils::String::replace(key, "\"", "\\\"");

	oss << "batocera-config" << " "
		<< "wifi" << " "
		<< "enable" << " \""
		<< ssid << "\" \"" << key << "\"";
	std::string command = oss.str();
	LOG(LogInfo) << "Launching " << command;
	if (system(command.c_str()) == 0) {
		LOG(LogInfo) << "Wifi enabled ";
		return true;
	}
	else {
		LOG(LogInfo) << "Cannot enable wifi ";
		return false;
	}
}

bool ApiSystem::disableWifi() {
	std::ostringstream oss;
	oss << "batocera-config" << " "
		<< "wifi" << " "
		<< "disable";
	std::string command = oss.str();
	LOG(LogInfo) << "Launching " << command;
	if (system(command.c_str()) == 0) {
		LOG(LogInfo) << "Wifi disabled ";
		return true;
	}
	else {
		LOG(LogInfo) << "Cannot disable wifi ";
		return false;
	}
}


bool ApiSystem::halt(bool reboot, bool fast) {
	SDL_Event *quit = new SDL_Event();
	if (fast)
		if (reboot)
			quit->type = SDL_FAST_QUIT | SDL_SYS_REBOOT;
		else
			quit->type = SDL_FAST_QUIT | SDL_SYS_SHUTDOWN;
	else
		if (reboot)
			quit->type = SDL_QUIT | SDL_SYS_REBOOT;
		else
			quit->type = SDL_QUIT | SDL_SYS_SHUTDOWN;
	SDL_PushEvent(quit);
	return 0;
}

bool ApiSystem::reboot() {
	return halt(true, false);
}

bool ApiSystem::fastReboot() {
	return halt(true, true);
}

bool ApiSystem::shutdown() {
	return halt(false, false);
}

bool ApiSystem::fastShutdown() {
	return halt(false, true);
}


std::string ApiSystem::getIpAdress() {
#if !defined(WIN32)
	struct ifaddrs *ifAddrStruct = NULL;
	struct ifaddrs *ifa = NULL;
	void *tmpAddrPtr = NULL;

	std::string result = "NOT CONNECTED";
	getifaddrs(&ifAddrStruct);

	for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
		if (!ifa->ifa_addr) {
			continue;
		}
		if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
			// is a valid IP4 Address
			tmpAddrPtr = &((struct sockaddr_in *) ifa->ifa_addr)->sin_addr;
			char addressBuffer[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
			printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
			if (std::string(ifa->ifa_name).find("eth") != std::string::npos ||
				std::string(ifa->ifa_name).find("wlan") != std::string::npos) {
				result = std::string(addressBuffer);
			}
		}
	}
	// Seeking for ipv6 if no IPV4
	if (result.compare("NOT CONNECTED") == 0) {
		for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
			if (!ifa->ifa_addr) {
				continue;
			}
			if (ifa->ifa_addr->sa_family == AF_INET6) { // check it is IP6
				// is a valid IP6 Address
				tmpAddrPtr = &((struct sockaddr_in6 *) ifa->ifa_addr)->sin6_addr;
				char addressBuffer[INET6_ADDRSTRLEN];
				inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
				printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
				if (std::string(ifa->ifa_name).find("eth") != std::string::npos ||
					std::string(ifa->ifa_name).find("wlan") != std::string::npos) {
					return std::string(addressBuffer);
				}
			}
		}
	}
	if (ifAddrStruct != NULL) freeifaddrs(ifAddrStruct);
	return result;
#else
	return std::string("127.0.0.1");
#endif
}

bool ApiSystem::scanNewBluetooth() {
	std::vector<std::string> *res = new std::vector<std::string>();
	std::ostringstream oss;
	oss << "batocera-bt-pair-device";
	FILE *pipe = popen(oss.str().c_str(), "r");
	char line[1024];

	if (pipe == NULL) {
		return false;
	}

	while (fgets(line, 1024, pipe)) {
		strtok(line, "\n");
		res->push_back(std::string(line));
	}

	int exitCode = pclose(pipe);
	return exitCode == 0;
}

std::vector<std::string> ApiSystem::getAvailableStorageDevices() 
{
	std::vector<std::string> res;

#if WIN32
	res.push_back("DEFAULT");
	return res;
#endif

	std::ostringstream oss;
	oss << "batocera-config" << " " << "storage list";
	FILE *pipe = popen(oss.str().c_str(), "r");
	char line[1024];

	if (pipe == NULL) {
		return res;
	}

	while (fgets(line, 1024, pipe)) {
		strtok(line, "\n");
		res.push_back(std::string(line));
	}
	pclose(pipe);

	return res;
}

std::vector<std::string> ApiSystem::getVideoModes() {
	std::vector<std::string> res;
	std::ostringstream oss;
	oss << "batocera-resolution listModes";
	FILE *pipe = popen(oss.str().c_str(), "r");
	char line[1024];

	if (pipe == NULL) {
		return res;
	}

	while (fgets(line, 1024, pipe)) {
		strtok(line, "\n");
		res.push_back(std::string(line));
	}
	pclose(pipe);

	return res;
}

std::vector<std::string> ApiSystem::getAvailableBackupDevices() {

	std::vector<std::string> res;
	std::ostringstream oss;
	oss << "batocera-sync list";
	FILE *pipe = popen(oss.str().c_str(), "r");
	char line[1024];

	if (pipe == NULL) {
		return res;
	}

	while (fgets(line, 1024, pipe)) {
		strtok(line, "\n");
		res.push_back(std::string(line));
	}
	pclose(pipe);

	return res;
}

std::vector<std::string> ApiSystem::getAvailableInstallDevices() {

	std::vector<std::string> res;
	std::ostringstream oss;
	oss << "batocera-install listDisks";
	FILE *pipe = popen(oss.str().c_str(), "r");
	char line[1024];

	if (pipe == NULL) {
		return res;
	}

	while (fgets(line, 1024, pipe)) {
		strtok(line, "\n");
		res.push_back(std::string(line));
	}
	pclose(pipe);

	return res;
}

std::vector<std::string> ApiSystem::getAvailableInstallArchitectures() {

	std::vector<std::string> res;
	std::ostringstream oss;
	oss << "batocera-install listArchs";
	FILE *pipe = popen(oss.str().c_str(), "r");
	char line[1024];

	if (pipe == NULL) {
		return res;
	}

	while (fgets(line, 1024, pipe)) {
		strtok(line, "\n");
		res.push_back(std::string(line));
	}
	pclose(pipe);

	return res;
}

std::vector<std::string> ApiSystem::getAvailableOverclocking() 
{
	std::vector<std::string> res;

#ifndef WIN32
	std::ostringstream oss;
	oss << "batocera-overclock list";
	FILE *pipe = popen(oss.str().c_str(), "r");
	char line[1024];

	if (pipe == NULL) {
		return res;
	}

	while (fgets(line, 1024, pipe)) {
		strtok(line, "\n");
		res.push_back(std::string(line));
	}
	pclose(pipe);
#endif

	return res;
}

std::vector<std::string> ApiSystem::getSystemInformations() 
{
	std::vector<std::string> res;

#if WIN32
	int CPUInfo[4] = { -1 };
	unsigned   nExIds, i = 0;
	char CPUBrandString[0x40];
	// Get the information associated with each extended ID.
	__cpuid(CPUInfo, 0x80000000);
	nExIds = CPUInfo[0];
	for (i = 0x80000000; i <= nExIds; ++i)
	{
		__cpuid(CPUInfo, i);
		// Interpret CPU brand string
		if (i == 0x80000002)
			memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000003)
			memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000004)
			memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
	}

	//string includes manufacturer, model and clockspeed
	res.push_back("CPU:" + std::string(CPUBrandString));

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);	
	res.push_back("CORES:" + std::to_string(sysInfo.dwNumberOfProcessors));

	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	res.push_back("MEMORY:" + std::to_string((statex.ullTotalPhys / 1024) / 1024) + "MB");

	return res;
#endif

	FILE *pipe = popen("batocera-info", "r");
	char line[1024];

	if (pipe == NULL) {
		return res;
	}

	while (fgets(line, 1024, pipe)) {
		strtok(line, "\n");
		res.push_back(std::string(line));
	}
	pclose(pipe);

	return res;
}

std::vector<BiosSystem> ApiSystem::getBiosInformations() {
	std::vector<BiosSystem> res;
	BiosSystem current;
	bool isCurrent = false;

#if WIN32 && _DEBUG
	current.name = "atari5200";

	BiosFile biosFile;
	biosFile.md5 = "281f20ea4320404ec820fb7ec0693b38";
	biosFile.path = "bios/5200.rom";
	biosFile.status = "missing";
	current.bios.push_back(biosFile);

	res.push_back(current);
	return res;
#endif

	FILE *pipe = popen("batocera-systems", "r");

	char line[1024];

	if (pipe == NULL) {
		return res;
	}

	while (fgets(line, 1024, pipe)) {
		strtok(line, "\n");
		if (Utils::String::startsWith(line, "> ")) {
			if (isCurrent) {
				res.push_back(current);
			}
			isCurrent = true;
			current.name = std::string(std::string(line).substr(2));
			current.bios.clear();
		}
		else {
			BiosFile biosFile;
			std::vector<std::string> tokens = Utils::String::split(line, ' ');
			if (tokens.size() >= 3) {
				biosFile.status = tokens.at(0);
				biosFile.md5 = tokens.at(1);

				// concatenat the ending words
				std::string vname = "";
				for (unsigned int i = 2; i < tokens.size(); i++) {
					if (i > 2) vname += " ";
					vname += tokens.at(i);
				}
				biosFile.path = vname;

				current.bios.push_back(biosFile);
			}
		}
	}
	if (isCurrent) {
		res.push_back(current);
	}
	pclose(pipe);
	return res;
}

bool ApiSystem::generateSupportFile() {
#if !defined(WIN32)
	std::string cmd = "batocera-support";
	int exitcode = system(cmd.c_str());
	if (WIFEXITED(exitcode)) {
		exitcode = WEXITSTATUS(exitcode);
	}
	return exitcode == 0;
#else
	return false;
#endif
}

std::string ApiSystem::getCurrentStorage() 
{
#if WIN32
	return "DEFAULT";
#endif

	std::ostringstream oss;
	oss << "batocera-config" << " " << "storage current";
	FILE *pipe = popen(oss.str().c_str(), "r");
	char line[1024];

	if (pipe == NULL) {
		return "";
	}

	if (fgets(line, 1024, pipe)) {
		strtok(line, "\n");
		pclose(pipe);
		return std::string(line);
	}
	return "INTERNAL";
}

bool ApiSystem::setStorage(std::string selected) {
	std::ostringstream oss;
	oss << "batocera-config" << " " << "storage" << " " << selected;
	int exitcode = system(oss.str().c_str());
	return exitcode == 0;
}

bool ApiSystem::forgetBluetoothControllers() {
	std::ostringstream oss;
	oss << "batocera-config" << " " << "forgetBT";
	int exitcode = system(oss.str().c_str());
	return exitcode == 0;
}

std::string ApiSystem::getRootPassword() {

	std::ostringstream oss;
	oss << "batocera-config" << " " << "getRootPassword";
	FILE *pipe = popen(oss.str().c_str(), "r");
	char line[1024];

	if (pipe == NULL) {
		return "";
	}

	if (fgets(line, 1024, pipe)) {
		strtok(line, "\n");
		pclose(pipe);
		return std::string(line);
	}
	return oss.str().c_str();
}

std::vector<std::string> ApiSystem::getAvailableAudioOutputDevices() {

	std::vector<std::string> res;
	std::ostringstream oss;
	oss << "batocera-config" << " " << "lsaudio";
	FILE *pipe = popen(oss.str().c_str(), "r");
	char line[1024];

	if (pipe == NULL) {
		return res;
	}

	while (fgets(line, 1024, pipe)) {
		strtok(line, "\n");
		res.push_back(std::string(line));
	}
	pclose(pipe);

	return res;
}

std::vector<std::string> ApiSystem::getAvailableVideoOutputDevices() {

	std::vector<std::string> res;
	std::ostringstream oss;
	oss << "batocera-config" << " " << "lsoutputs";
	FILE *pipe = popen(oss.str().c_str(), "r");
	char line[1024];

	if (pipe == NULL) {
		return res;
	}

	while (fgets(line, 1024, pipe)) {
		strtok(line, "\n");
		res.push_back(std::string(line));
	}
	pclose(pipe);

	return res;
}

std::string ApiSystem::getCurrentAudioOutputDevice() {

	std::ostringstream oss;
	oss << "batocera-config" << " " << "getaudio";
	FILE *pipe = popen(oss.str().c_str(), "r");
	char line[1024];

	if (pipe == NULL) {
		return "";
	}

	if (fgets(line, 1024, pipe)) {
		strtok(line, "\n");
		pclose(pipe);
		return std::string(line);
	}
	return "";
}

bool ApiSystem::setAudioOutputDevice(std::string selected) {
	std::ostringstream oss;

	AudioManager::getInstance()->deinit();
	VolumeControl::getInstance()->deinit();

	oss << "batocera-config" << " " << "audio" << " '" << selected << "'";
	int exitcode = system(oss.str().c_str());

	VolumeControl::getInstance()->init();
	AudioManager::getInstance()->init();
	Sound::get("/usr/share/emulationstation/resources/checksound.ogg")->play();

	return exitcode == 0;
}

// Batocera
std::vector<std::string> ApiSystem::getRetroAchievements() {

	std::vector<std::string> res;
	std::ostringstream oss;
	oss << "batocera-retroachievements-info";
	FILE *pipe = popen(oss.str().c_str(), "r");
	char line[1024];

	if (pipe == NULL) {
		return res;
	}

	while (fgets(line, 1024, pipe)) {
		strtok(line, "\n");
		res.push_back(std::string(line));
	}
	pclose(pipe);
	return res;
}

std::vector<std::string> ApiSystem::getBatoceraThemesList() 
{

	std::vector<std::string> res;

#if WIN32 && _DEBUG
	// http://batocera-linux.xorhub.com/upgrades/themes.txt
	res.push_back("[A]\tAlekfull\thttps://github.com/jdorigao/es-theme-alekfull");
	res.push_back("[A]\tArt-book\thttps://github.com/anthonycaccese/es-theme-art-book");
	res.push_back("[A]\tBatopicase\thttps://github.com/Genetik57/es-theme-simply-batopicase");
	res.push_back("[A]\tFundamental\thttps://github.com/jdorigao/es-theme-fundamental");
	res.push_back("[I]\tMinimal\thttps://github.com/crcerror/es-theme-minimal");
	res.push_back("[A]\tRVGM\thttps://github.com/Darknior/RVGM-ES-Theme");
	res.push_back("[A]\tSimple\thttps://github.com/RetroPie/es-theme-simple");
	res.push_back("[A]\tZoid\thttps://github.com/RetroPie/es-theme-zoid");
	res.push_back("[A]\tVideoGame\thttps://github.com/jdorigao/es-theme-videogame");
	return res;
#endif

	std::ostringstream oss;
	oss << "batocera-es-theme" << " " << "list";
	FILE *pipe = popen(oss.str().c_str(), "r");
	char line[1024];
	char *pch;

	if (pipe == NULL) {
		return res;
	}
	while (fgets(line, 1024, pipe)) {
		strtok(line, "\n");
		// provide only themes that are [A]vailable or [I]nstalled as a result
		// (Eliminate [?] and other non-installable lines of text)
		if ((strncmp(line, "[A]", 3) == 0) || (strncmp(line, "[I]", 3) == 0))
			res.push_back(std::string(line));
	}
	pclose(pipe);
	return res;
}

std::pair<std::string, int> ApiSystem::installBatoceraTheme(BusyComponent* ui, std::string thname) {
	std::string updatecommand = std::string("batocera-es-theme install ") + thname;
	LOG(LogWarning) << "Installing theme " << thname;
	FILE *pipe = popen(updatecommand.c_str(), "r");
	char line[1024] = "";
	if (pipe == NULL) {
		return std::pair<std::string, int>(std::string("Error starting `batocera-es-theme` command."), -1);
	}

	while (fgets(line, 1024, pipe)) {
		strtok(line, "\n");
		LOG(LogWarning) << "Theme install: " << line;
		// Long theme names/URL can crash the GUI MsgBox
		// "48" found by trials and errors. Ideally should be fixed
		// in es-core MsgBox -- FIXME
		if (strlen(line) > 48)
			line[47] = '\0';
		ui->setText(std::string(line));
	}

	int exitCode = pclose(pipe);
	return std::pair<std::string, int>(std::string(line), exitCode);
}


std::vector<std::string> ApiSystem::getBatoceraBezelsList() {
	std::vector<std::string> res;
	std::ostringstream oss;
	oss << "batocera-es-thebezelproject list";
	FILE *pipe = popen(oss.str().c_str(), "r");
	char line[1024];
	char *pch;

	if (pipe == NULL) {
		return res;
	}
	while (fgets(line, 1024, pipe)) {
		strtok(line, "\n");
		// provide only themes that are [A]vailable or [I]nstalled as a result
		// (Eliminate [?] and other non-installable lines of text)
		if ((strncmp(line, "[A]", 3) == 0) || (strncmp(line, "[I]", 3) == 0))
			res.push_back(std::string(line));
	}
	pclose(pipe);
	return res;
}

std::pair<std::string, int> ApiSystem::installBatoceraBezel(BusyComponent* ui, std::string bezelsystem) {
	std::string updatecommand = std::string("batocera-es-thebezelproject install ") + bezelsystem;
	LOG(LogWarning) << "Installing bezels for " << bezelsystem;
	FILE *pipe = popen(updatecommand.c_str(), "r");
	char line[1024] = "";
	if (pipe == NULL) {
		return std::pair<std::string, int>(std::string("Error starting `batocera-es-thebezelproject install` command."), -1);
	}

	while (fgets(line, 1024, pipe)) {
		strtok(line, "\n");
		// Long theme names/URL can crash the GUI MsgBox
		// "48" found by trials and errors. Ideally should be fixed
		// in es-core MsgBox -- FIXME
		if (strlen(line) > 48)
			line[47] = '\0';
		ui->setText(std::string(line));
	}

	int exitCode = pclose(pipe);
	return std::pair<std::string, int>(std::string(line), exitCode);
}

std::pair<std::string, int> ApiSystem::uninstallBatoceraBezel(BusyComponent* ui, std::string bezelsystem) {
	std::string updatecommand = std::string("batocera-es-thebezelproject remove ") + bezelsystem;
	LOG(LogWarning) << "Removing bezels for " << bezelsystem;
	FILE *pipe = popen(updatecommand.c_str(), "r");
	char line[1024] = "";
	if (pipe == NULL) {
		return std::pair<std::string, int>(std::string("Error starting `batocera-es-thebezelproject remove` command."), -1);
	}

	while (fgets(line, 1024, pipe)) {
		strtok(line, "\n");
		// Long theme names/URL can crash the GUI MsgBox
		// "48" found by trials and errors. Ideally should be fixed
		// in es-core MsgBox -- FIXME
		if (strlen(line) > 48)
			line[47] = '\0';
		ui->setText(std::string(line));
	}

	int exitCode = pclose(pipe);
	return std::pair<std::string, int>(std::string(line), exitCode);
}
