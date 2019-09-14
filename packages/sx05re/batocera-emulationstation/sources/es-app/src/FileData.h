#pragma once
#ifndef ES_APP_FILE_DATA_H
#define ES_APP_FILE_DATA_H

#include "utils/FileSystemUtil.h"
#include "MetaData.h"
#include <unordered_map>

class SystemData;
class Window;
struct SystemEnvironmentData;

enum FileType
{
	GAME = 1,   // Cannot have children.
	FOLDER = 2,
	PLACEHOLDER = 3
};

enum FileChangeType
{
	FILE_ADDED,
	FILE_METADATA_CHANGED,
	FILE_REMOVED,
	FILE_SORTED
};

class FolderData;

// A tree node that holds information for a file.
class FileData
{
public:
	FileData(FileType type, const std::string& path, SystemData* system);
	virtual ~FileData();

	virtual const std::string getName();

	inline FileType getType() const { return mType; }
	
	inline FolderData* getParent() const { return mParent; }
	void setParent(FolderData* parent) { mParent = parent; }

	inline SystemData* getSystem() const { return mSystem; }

	virtual const std::string getPath() const;

	virtual SystemEnvironmentData* getSystemEnvData() const;

	virtual const std::string getThumbnailPath() const;
	virtual const std::string getVideoPath() const;
	virtual const std::string getMarqueePath() const;
	virtual const std::string getImagePath() const;

	virtual const std::string getCore() const;
	virtual const std::string getEmulator() const;

	virtual const bool getHidden();
	virtual const bool getFavorite();
	virtual const bool getKidGame();

	inline bool isPlaceHolder() { return mType == PLACEHOLDER; };

	virtual inline void refreshMetadata() { return; };

	virtual std::string getKey();
	const bool isArcadeAsset();
	inline std::string getFullPath() { return getPath(); };
	inline std::string getFileName() { return Utils::FileSystem::getFileName(getPath()); };
	virtual FileData* getSourceFileData();
	virtual std::string getSystemName() const;

	// Returns our best guess at the "real" name for this file (will attempt to perform MAME name translation)
	std::string getDisplayName() const;

	// As above, but also remove parenthesis
	std::string getCleanName() const;

	void launchGame(Window* window);

	MetaDataList metadata;

protected:	
	FolderData* mParent;
	std::string mPath;
	FileType mType;
	SystemData* mSystem;
};

class CollectionFileData : public FileData
{
public:
	CollectionFileData(FileData* file, SystemData* system);
	~CollectionFileData();
	const std::string getName();
	void refreshMetadata();
	FileData* getSourceFileData();
	std::string getKey();
	virtual const std::string getPath() const;

	virtual std::string getSystemName() const;
	virtual SystemEnvironmentData* getSystemEnvData() const;

private:
	// needs to be updated when metadata changes
	std::string mCollectionFileName;
	FileData* mSourceFileData;

	bool mDirty;
};

class FolderData : public FileData
{
public:
	FolderData(const std::string& startpath, SystemData* system) : FileData(FOLDER, startpath, system)
	{
	}

	~FolderData()
	{
		for (int i = mChildren.size() - 1; i >= 0; i--)
			delete mChildren.at(i);

		mChildren.clear();
	}

	typedef bool ComparisonFunction(const FileData* a, const FileData* b);
	struct SortType
	{
		ComparisonFunction* comparisonFunction;
		bool ascending;
		std::string description;
		std::string icon;

		SortType(ComparisonFunction* sortFunction, bool sortAscending, const std::string & sortDescription, const std::string & iconId = "")
			: comparisonFunction(sortFunction), ascending(sortAscending), description(sortDescription), icon(iconId) {}
	};

	void sort(ComparisonFunction& comparator, bool ascending = true);
	void sort(const SortType& type);

	FileData* FindByPath(const std::string& path);

	inline const std::vector<FileData*>& getChildren() const { return mChildren; }
	const std::vector<FileData*> getChildrenListToDisplay();
	std::vector<FileData*> getFilesRecursive(unsigned int typeMask, bool displayedOnly = false, SystemData* system = nullptr) const;
	std::vector<FileData*> getFlatGameList(bool displayedOnly, SystemData* system) const;

	void addChild(FileData* file); // Error if mType != FOLDER
	void removeChild(FileData* file); //Error if mType != FOLDER

	void createChildrenByFilenameMap(std::unordered_map<std::string, FileData*>& map);

private:
	std::vector<FileData*> mChildren;
};

FolderData::SortType getSortTypeFromString(std::string desc);

#endif // ES_APP_FILE_DATA_H
