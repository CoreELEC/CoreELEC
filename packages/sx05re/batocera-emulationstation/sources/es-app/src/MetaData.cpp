#include "MetaData.h"

#include "utils/FileSystemUtil.h"
#include "utils/StringUtil.h"
#include "Log.h"
#include <pugixml/src/pugixml.hpp>
#include "SystemData.h"
#include "LocaleES.h"

static std::vector<MetaDataDecl> gameMDD;
static std::vector<MetaDataDecl> folderMDD;

static std::string mDefaultGameMap[18];
static std::string mDefaultFolderMap[13];

static MetaDataType mGameTypeMap[18];
static MetaDataType mFolderTypeMap[13];

static std::map<std::string, unsigned char> mGameIdMap;
static std::map<std::string, unsigned char> mFolderIdMap;

void MetaDataList::initMetadata()
{
	//								id,   key,         type,                   default,            statistic,          name in GuiMetaDataEd,          prompt in GuiMetaDataEd
	gameMDD.push_back(MetaDataDecl(0, "name", MD_STRING, "", false, _("Name"), _("enter game name")));
	//  gameMDD.push_back(MetaDataDecl(1, "sortname",		MD_STRING,		"", false, _("Sort name"), _("enter game sort name")));
	gameMDD.push_back(MetaDataDecl(2, "desc", MD_MULTILINE_STRING, "", false, _("Description"), _("enter description")));
	gameMDD.push_back(MetaDataDecl(3, "image", MD_PATH, "", false, _("Image"), _("enter path to image")));
	gameMDD.push_back(MetaDataDecl(4, "video", MD_PATH, "", false, _("Path to video"), _("enter path to thumbnail")));
	gameMDD.push_back(MetaDataDecl(5, "marquee", MD_PATH, "", false, _("Path to marquee"), _("enter path to thumbnail")));
	gameMDD.push_back(MetaDataDecl(6, "thumbnail", MD_PATH, "", false, _("Thumbnail"), _("enter path to thumbnail")));
	gameMDD.push_back(MetaDataDecl(7, "rating", MD_RATING, "0.000000", false, _("Rating"), _("enter rating")));
	gameMDD.push_back(MetaDataDecl(8, "releasedate", MD_DATE, "not-a-date-time", false, _("Release date"), _("enter release date")));
	gameMDD.push_back(MetaDataDecl(9, "developer", MD_STRING, "unknown", false, _("Developer"), _("enter game developer")));
	gameMDD.push_back(MetaDataDecl(10, "publisher", MD_STRING, "unknown", false, _("Publisher"), _("enter game publisher")));
	gameMDD.push_back(MetaDataDecl(11, "genre", MD_STRING, "unknown", false, _("Genre"), _("enter game genre")));
	gameMDD.push_back(MetaDataDecl(12, "players", MD_INT, "1", false, _("Players"), _("enter number of players")));
	gameMDD.push_back(MetaDataDecl(13, "favorite", MD_BOOL, "false", false, _("Favorite"), _("enter favorite")));
	gameMDD.push_back(MetaDataDecl(14, "hidden", MD_BOOL, "false", false, _("Hidden"), _("set hidden")));
	gameMDD.push_back(MetaDataDecl(15, "kidgame", MD_BOOL, "false", false, _("Kidgame"), _("set kidgame")));
	gameMDD.push_back(MetaDataDecl(16, "playcount", MD_INT, "0", true, _("Play count"), _("enter number of times played")));
	gameMDD.push_back(MetaDataDecl(17, "lastplayed", MD_TIME, "0", true, _("Last played"), _("enter last played date")));

	folderMDD.push_back(MetaDataDecl(0, "name", MD_STRING, "", false, _("name"), _("enter game name")));
	//  folderMDD.push_back(MetaDataDecl(1, "sortname",	MD_STRING,		"", 		false, _("sortname"),    _("enter game sort name")));
	folderMDD.push_back(MetaDataDecl(2, "desc", MD_MULTILINE_STRING, "", false, _("description"), _("enter description")));
	folderMDD.push_back(MetaDataDecl(3, "image", MD_PATH, "", false, _("image"), _("enter path to image")));
	folderMDD.push_back(MetaDataDecl(4, "thumbnail", MD_PATH, "", false, _("thumbnail"), _("enter path to thumbnail")));
	folderMDD.push_back(MetaDataDecl(5, "video", MD_PATH, "", false, _("video"), _("enter path to video")));
	folderMDD.push_back(MetaDataDecl(6, "marquee", MD_PATH, "", false, _("marquee"), _("enter path to marquee")));
	folderMDD.push_back(MetaDataDecl(7, "rating", MD_RATING, "0.000000", false, _("Rating"), _("enter rating")));
	folderMDD.push_back(MetaDataDecl(8, "releasedate", MD_DATE, "not-a-date-time", false, _("Release date"), _("enter release date")));
	folderMDD.push_back(MetaDataDecl(9, "developer", MD_STRING, "unknown", false, _("Developer"), _("enter game developer")));
	folderMDD.push_back(MetaDataDecl(10, "publisher", MD_STRING, "unknown", false, _("Publisher"), _("enter game publisher")));
	folderMDD.push_back(MetaDataDecl(11, "genre", MD_STRING, "unknown", false, _("Genre"), _("enter game genre")));
	folderMDD.push_back(MetaDataDecl(12, "players", MD_INT, "1", false, _("Players"), _("enter number of players")));

	// Build Game maps
	{
		const std::vector<MetaDataDecl>& mdd = getMDDByType(GAME_METADATA);
		for (auto iter = mdd.cbegin(); iter != mdd.cend(); iter++)
		{
			mDefaultGameMap[iter->id] = iter->defaultValue;
			mGameTypeMap[iter->id] = iter->type;
			mGameIdMap[iter->key] = iter->id;
		}
	}

	// Build Folder maps
	{
		const std::vector<MetaDataDecl>& mdd = getMDDByType(FOLDER_METADATA);
		for (auto iter = mdd.cbegin(); iter != mdd.cend(); iter++)
		{
			mDefaultFolderMap[iter->id] = iter->defaultValue;
			mFolderTypeMap[iter->id] = iter->type;
			mFolderIdMap[iter->key] = iter->id;
		}
	}
}

MetaDataType MetaDataList::getType(unsigned char id) const
{
	return mType == GAME_METADATA ? mGameTypeMap[id] : mFolderTypeMap[id];
}

unsigned char MetaDataList::getId(const std::string& key) const
{
	return mType == GAME_METADATA ? mGameIdMap[key] : mFolderIdMap[key];
}

const std::vector<MetaDataDecl>& getMDDByType(MetaDataListType type)
{
	return type == FOLDER_METADATA ? folderMDD : gameMDD;
}

MetaDataList::MetaDataList(MetaDataListType type) : mType(type), mWasChanged(false), mRelativeTo(nullptr)
{

}


MetaDataList MetaDataList::createFromXML(MetaDataListType type, pugi::xml_node& node, SystemData* system)
{
	MetaDataList mdl(type);
	mdl.mRelativeTo = system;

	//std::string relativeTo = system->getStartPath();

	const std::vector<MetaDataDecl>& mdd = mdl.getMDD();

	for(auto iter = mdd.cbegin(); iter != mdd.cend(); iter++)
	{
		pugi::xml_node md = node.child(iter->key.c_str());
		if(md)
		{
			// if it's a path, resolve relative paths
			std::string value = md.text().get();

//			if (iter->type == MD_PATH)
//				value = Utils::FileSystem::resolveRelativePath(value, relativeTo, true);

			if (value == iter->defaultValue)
				continue;

			if (iter->type == MD_BOOL)
				value = Utils::String::toLower(value);
			
			if (iter->id == 0)
				mdl.mName = value;
			else
				mdl.set(iter->key, value);
		}
	}

	return mdl;
}

void MetaDataList::appendToXML(pugi::xml_node& parent, bool ignoreDefaults, const std::string& relativeTo) const
{
	const std::vector<MetaDataDecl>& mdd = getMDD();

	for(auto mddIter = mdd.cbegin(); mddIter != mdd.cend(); mddIter++)
	{
		if (mddIter->id == 0)
		{
			parent.append_child("name").text().set(mName.c_str());
			continue;
		}

		auto mapIter = mMap.find(mddIter->id);
		if(mapIter != mMap.cend())
		{
			// we have this value!
			// if it's just the default (and we ignore defaults), don't write it
			if(ignoreDefaults && mapIter->second == mddIter->defaultValue)
				continue;
			
			// try and make paths relative if we can
			std::string value = mapIter->second;
			if (mddIter->type == MD_PATH)
				value = Utils::FileSystem::createRelativePath(value, relativeTo, true);

			parent.append_child(mddIter->key.c_str()).text().set(value.c_str());
		}
	}
}

const std::string& MetaDataList::getName() const
{
	return mName;
}

void MetaDataList::set(const std::string& key, const std::string& value)
{
	if (key == "name")
	{
		if (mName == value)
			return;

		mName = value;
	}
	else
	{
		auto id = getId(key);

		auto prev = mMap.find(id);
		if (prev != mMap.cend() && prev->second == value)
			return;

		mMap[id] = value;
	}

	mWasChanged = true;
}

const std::string MetaDataList::get(const std::string& key) const
{
	if (key == "name")
		return mName;

	auto id = getId(key);

	auto it = mMap.find(id);
	if (it != mMap.end())
	{
		if (getType(id) == MD_PATH && mRelativeTo != nullptr) // if it's a path, resolve relative paths				
			return Utils::FileSystem::resolveRelativePath(it->second, mRelativeTo->getStartPath(), true);

		return it->second;
	}

	if (mType == GAME_METADATA)
		return mDefaultGameMap[id];

	return mDefaultFolderMap[id];
}

int MetaDataList::getInt(const std::string& key) const
{
	return atoi(get(key).c_str());
}

float MetaDataList::getFloat(const std::string& key) const
{
	return (float)atof(get(key).c_str());
}

bool MetaDataList::wasChanged() const
{
	return mWasChanged;
}

void MetaDataList::resetChangedFlag()
{
	mWasChanged = false;
}
