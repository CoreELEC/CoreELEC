#include "FileSorts.h"

#include "utils/StringUtil.h"
#include "LocaleES.h"

namespace FileSorts
{
	const FolderData::SortType typesArr[] = {
		FolderData::SortType(&compareName, true, "Filename, Ascending", _U("\uF15d ")),
		FolderData::SortType(&compareName, false, "Filename, descending", _U("\uF15e ")),

		FolderData::SortType(&compareRating, true, "Rating, Ascending", _U("\uF165 ")),
		FolderData::SortType(&compareRating, false, "Rating, Descending", _U("\uF164 ")),

		FolderData::SortType(&compareTimesPlayed, true, "Times played, Ascending", _U("\uF160 ")),
		FolderData::SortType(&compareTimesPlayed, false, "Times played, Descending", _U("\uF161 ")),

		FolderData::SortType(&compareLastPlayed, true, "Last played, Ascending", _U("\uF160 ")),
		FolderData::SortType(&compareLastPlayed, false, "Last played, Descending", _U("\uF161 ")),

		FolderData::SortType(&compareNumPlayers, true, "Number players, Ascending", _U("\uF162 ")),
		FolderData::SortType(&compareNumPlayers, false, "Number players, Descending", _U("\uF163 ")),

		FolderData::SortType(&compareReleaseDate, true, "Release date, Ascending"),
		FolderData::SortType(&compareReleaseDate, false, "Release date, Descending"),

		FolderData::SortType(&compareGenre, true, "Genre, Ascending"),
		FolderData::SortType(&compareGenre, false, "Genre, Descending"),

		FolderData::SortType(&compareDeveloper, true, "Developer, Ascending"),
		FolderData::SortType(&compareDeveloper, false, "Developer, Descending"),

		FolderData::SortType(&comparePublisher, true, "Publisher, Ascending"),
		FolderData::SortType(&comparePublisher, false, "Publisher, Descending"),

		FolderData::SortType(&compareSystem, true, "System, Ascending"),
		FolderData::SortType(&compareSystem, false, "System, Descending")
	};

	const std::vector<FolderData::SortType> SortTypes(typesArr, typesArr + sizeof(typesArr)/sizeof(typesArr[0]));

	//returns if file1 should come before file2
	bool compareName(const FileData* file1, const FileData* file2)
	{
		// we compare the actual metadata name, as collection files have the system appended which messes up the order
		std::string name1 = Utils::String::toUpper(file1->metadata.getName());
		std::string name2 = Utils::String::toUpper(file2->metadata.getName());
		return name1.compare(name2) < 0;
	}

	bool compareRating(const FileData* file1, const FileData* file2)
	{
		return file1->metadata.getFloat("rating") < file2->metadata.getFloat("rating");
	}

	bool compareTimesPlayed(const FileData* file1, const FileData* file2)
	{
		//only games have playcount metadata
		if(file1->metadata.getType() == GAME_METADATA && file2->metadata.getType() == GAME_METADATA)
		{
			return (file1)->metadata.getInt("playcount") < (file2)->metadata.getInt("playcount");
		}

		return false;
	}

	bool compareLastPlayed(const FileData* file1, const FileData* file2)
	{
		// since it's stored as an ISO string (YYYYMMDDTHHMMSS), we can compare as a string
		// as it's a lot faster than the time casts and then time comparisons
		return (file1)->metadata.get("lastplayed") < (file2)->metadata.get("lastplayed");
	}

	bool compareNumPlayers(const FileData* file1, const FileData* file2)
	{
		return (file1)->metadata.getInt("players") < (file2)->metadata.getInt("players");
	}

	bool compareReleaseDate(const FileData* file1, const FileData* file2)
	{
		// since it's stored as an ISO string (YYYYMMDDTHHMMSS), we can compare as a string
		// as it's a lot faster than the time casts and then time comparisons
		return (file1)->metadata.get("releasedate") < (file2)->metadata.get("releasedate");
	}

	bool compareGenre(const FileData* file1, const FileData* file2)
	{
		std::string genre1 = Utils::String::toUpper(file1->metadata.get("genre"));
		std::string genre2 = Utils::String::toUpper(file2->metadata.get("genre"));
		return genre1.compare(genre2) < 0;
	}

	bool compareDeveloper(const FileData* file1, const FileData* file2)
	{
		std::string developer1 = Utils::String::toUpper(file1->metadata.get("developer"));
		std::string developer2 = Utils::String::toUpper(file2->metadata.get("developer"));
		return developer1.compare(developer2) < 0;
	}

	bool comparePublisher(const FileData* file1, const FileData* file2)
	{
		std::string publisher1 = Utils::String::toUpper(file1->metadata.get("publisher"));
		std::string publisher2 = Utils::String::toUpper(file2->metadata.get("publisher"));
		return publisher1.compare(publisher2) < 0;
	}

	bool compareSystem(const FileData* file1, const FileData* file2)
	{
		std::string system1 = Utils::String::toUpper(file1->getSystemName());
		std::string system2 = Utils::String::toUpper(file2->getSystemName());
		return system1.compare(system2) < 0;
	}
};
