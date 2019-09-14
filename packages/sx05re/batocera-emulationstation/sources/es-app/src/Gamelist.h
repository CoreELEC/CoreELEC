#pragma once
#ifndef ES_APP_GAME_LIST_H
#define ES_APP_GAME_LIST_H

#include <unordered_map>

class SystemData;
class FileData;

// Loads gamelist.xml data into a SystemData.
void parseGamelist(SystemData* system, std::unordered_map<std::string, FileData*>& fileMap);

// Writes currently loaded metadata for a SystemData to gamelist.xml.
void updateGamelist(SystemData* system);

#endif // ES_APP_GAME_LIST_H
