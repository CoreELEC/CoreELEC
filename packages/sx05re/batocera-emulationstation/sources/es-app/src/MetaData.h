#pragma once
#ifndef ES_APP_META_DATA_H
#define ES_APP_META_DATA_H

#include <map>
#include <vector>
#include <functional>

class SystemData;

namespace pugi { class xml_node; }

enum MetaDataType
{
	//generic types
	MD_STRING,
	MD_INT,
	MD_FLOAT,
	MD_BOOL,

	//specialized types
	MD_MULTILINE_STRING,
	MD_PATH,
	MD_RATING,
	MD_DATE,
	MD_TIME, //used for lastplayed
        MD_LIST // batocera
};

struct MetaDataDecl
{
	unsigned char id;

	std::string key;
	MetaDataType type;
	std::string defaultValue;
	bool isStatistic; //if true, ignore scraper values for this metadata
	std::string displayName; // displayed as this in editors
	std::string displayPrompt; // phrase displayed in editors when prompted to enter value (currently only for strings)

  // batocera
	MetaDataDecl(unsigned char id, std::string key, MetaDataType type, std::string defaultValue, bool isStatistic, std::string displayName, std::string displayPrompt) 
	{
		this->id = id;
		this->key = key;
		this->type = type;
		this->defaultValue = defaultValue;
		this->isStatistic = isStatistic;
		this->displayName = displayName;
		this->displayPrompt = displayPrompt;
	}

	// batocera 
	MetaDataDecl(unsigned char id, std::string key, MetaDataType type, std::string defaultValue, bool isStatistic) 
	{
		this->id = id;
		this->key = key;
		this->type = type;
		this->defaultValue = defaultValue;
		this->isStatistic = isStatistic;
	}
};

enum MetaDataListType
{
	GAME_METADATA,
	FOLDER_METADATA
};

const std::vector<MetaDataDecl>& getMDDByType(MetaDataListType type);

class MetaDataList
{
public:
	static void initMetadata();

	static MetaDataList createFromXML(MetaDataListType type, pugi::xml_node& node, SystemData* system);
	void appendToXML(pugi::xml_node& parent, bool ignoreDefaults, const std::string& relativeTo) const;

	MetaDataList(MetaDataListType type);
	
	void set(const std::string& key, const std::string& value);

	const std::string get(const std::string& key) const;
	int getInt(const std::string& key) const;
	float getFloat(const std::string& key) const;

	bool wasChanged() const;
	void resetChangedFlag();

	inline MetaDataListType getType() const { return mType; }
	inline const std::vector<MetaDataDecl>& getMDD() const { return getMDDByType(getType()); }

	const std::string& getName() const;

private:
	std::string		mName;
	MetaDataListType mType;
	std::map<unsigned char, std::string> mMap;
	bool mWasChanged;
	SystemData*		mRelativeTo;

	inline MetaDataType getType(unsigned char id) const;
	inline unsigned char getId(const std::string& key) const;
};

#endif // ES_APP_META_DATA_H
