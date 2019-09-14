#include "FileData.h"

#include "utils/FileSystemUtil.h"
#include "utils/StringUtil.h"
#include "utils/TimeUtil.h"
#include "AudioManager.h"
#include "CollectionSystemManager.h"
#include "FileFilterIndex.h"
#include "FileSorts.h"
#include "Log.h"
#include "MameNames.h"
#include "platform.h"
#include "Scripting.h"
#include "SystemData.h"
#include "VolumeControl.h"
#include "Window.h"
#include "views/UIModeController.h"
#include <assert.h>
#include "SystemConf.h"
#include "InputManager.h"

FileData::FileData(FileType type, const std::string& path, SystemData* system)
	: mType(type), mSystem(system), mParent(NULL), metadata(type == GAME ? GAME_METADATA : FOLDER_METADATA) // metadata is REALLY set in the constructor!
{
	mPath = Utils::FileSystem::createRelativePath(path, getSystemEnvData()->mStartPath, false);

	// metadata needs at least a name field (since that's what getName() will return)
	if (metadata.get("name").empty())
		metadata.set("name", getDisplayName());	
	
	metadata.resetChangedFlag();
}

const std::string FileData::getPath() const
{ 	
	if (mPath.empty())
		return getSystemEnvData()->mStartPath;

	return Utils::FileSystem::resolveRelativePath(mPath, getSystemEnvData()->mStartPath, true);	
}

inline SystemEnvironmentData* FileData::getSystemEnvData() const
{ 
	return mSystem->getSystemEnvData(); 
}

std::string FileData::getSystemName() const
{
	return mSystem->getName();
}

FileData::~FileData()
{
	if(mParent)
		mParent->removeChild(this);

	if(mType == GAME)
		mSystem->removeFromIndex(this);	
}

std::string FileData::getDisplayName() const
{
	std::string stem = Utils::FileSystem::getStem(getPath());
	if(mSystem && mSystem->hasPlatformId(PlatformIds::ARCADE) || mSystem->hasPlatformId(PlatformIds::NEOGEO))
		stem = MameNames::getInstance()->getRealName(stem);

	return stem;
}

std::string FileData::getCleanName() const
{
	return Utils::String::removeParenthesis(this->getDisplayName());
}

const std::string FileData::getThumbnailPath() const
{
	std::string thumbnail = metadata.get("thumbnail");

	// no thumbnail, try image
	if(thumbnail.empty())
	{
		thumbnail = metadata.get("image");
		
		// no image, try to use local image
		if(thumbnail.empty() && Settings::getInstance()->getBool("LocalArt"))
		{
			const char* extList[2] = { ".png", ".jpg" };
			for(int i = 0; i < 2; i++)
			{
				if(thumbnail.empty())
				{
					std::string path = getSystemEnvData()->mStartPath + "/images/" + getDisplayName() + "-image" + extList[i];
					if(Utils::FileSystem::exists(path))
						thumbnail = path;
				}
			}
		}
	}

	return thumbnail;
}

const bool FileData::getFavorite()
{
	return metadata.get("favorite") == "true";
}

const bool FileData::getHidden()
{
	return metadata.get("hidden") == "true";
}

const bool FileData::getKidGame()
{
	return metadata.get("kidgame") != "false";
}

const std::string FileData::getName()
{
	return metadata.getName();
}

const std::string FileData::getCore() const
{
	return metadata.get("core");	
}

const std::string FileData::getEmulator() const
{
	return metadata.get("emulator");
}

const std::string FileData::getVideoPath() const
{
	std::string video = metadata.get("video");
	
	// no video, try to use local video
	if(video.empty() && Settings::getInstance()->getBool("LocalArt"))
	{
		std::string path = getSystemEnvData()->mStartPath + "/images/" + getDisplayName() + "-video.mp4";
		if (Utils::FileSystem::exists(path))
			video = path;
	}
	
	return video;
}

const std::string FileData::getMarqueePath() const
{
	std::string marquee = metadata.get("marquee");

	// no marquee, try to use local marquee
	if (marquee.empty() && Settings::getInstance()->getBool("LocalArt"))
	{
		const char* extList[2] = { ".png", ".jpg" };
		for(int i = 0; i < 2; i++)
		{
			if(marquee.empty())
			{
				std::string path = getSystemEnvData()->mStartPath + "/images/" + getDisplayName() + "-marquee" + extList[i];
				if(Utils::FileSystem::exists(path))
					marquee = path;
			}
		}
	}

	return marquee;
}

const std::string FileData::getImagePath() const
{
	std::string image = metadata.get("image");

	// no image, try to use local image
	if(image.empty())
	{
		const char* extList[2] = { ".png", ".jpg" };
		for(int i = 0; i < 2; i++)
		{
			if(image.empty())
			{
				std::string path = getSystemEnvData()->mStartPath + "/images/" + getDisplayName() + "-image" + extList[i];
				if(Utils::FileSystem::exists(path))
					image = path;
			}
		}
	}

	return image;
}

std::string FileData::getKey() {
	return getFileName();
}

const bool FileData::isArcadeAsset()
{
	if (mSystem && (mSystem->hasPlatformId(PlatformIds::ARCADE) || mSystem->hasPlatformId(PlatformIds::NEOGEO)))
	{	
		const std::string stem = Utils::FileSystem::getStem(getPath());
		return MameNames::getInstance()->isBios(stem) || MameNames::getInstance()->isDevice(stem);		
	}

	return false;
}

FileData* FileData::getSourceFileData()
{
	return this;
}


void FileData::launchGame(Window* window)
{
	LOG(LogInfo) << "Attempting to launch game...";

	AudioManager::getInstance()->deinit(); // batocera
	VolumeControl::getInstance()->deinit();
	const std::string controllersConfig = InputManager::getInstance()->configureEmulators(); // batocera / must be done before window->deinit while it closes joysticks
	window->deinit();

	std::string command = getSystemEnvData()->mLaunchCommand;

	const std::string rom = Utils::FileSystem::getEscapedPath(getPath());
	const std::string basename = Utils::FileSystem::getStem(getPath());
	const std::string rom_raw = Utils::FileSystem::getPreferredPath(getPath());

	command = Utils::String::replace(command, "%SYSTEM%", getSourceFileData()->getSystem()->getName()); // batocera
	command = Utils::String::replace(command, "%ROM%", rom);
	command = Utils::String::replace(command, "%BASENAME%", basename);
	command = Utils::String::replace(command, "%ROM_RAW%", rom_raw);
	command = Utils::String::replace(command, "%CONTROLLERSCONFIG%", controllersConfig); // batocera

	Scripting::fireEvent("game-start", rom, basename);

	LOG(LogInfo) << "	" << command;
	int exitCode = runSystemCommand(command);

	if (exitCode != 0)
	{
		LOG(LogWarning) << "...launch terminated with nonzero exit code " << exitCode << "!";
	}

	Scripting::fireEvent("game-end");

	window->init();
	VolumeControl::getInstance()->init();

	// mSystem can be NULL
	//AudioManager::getInstance()->setName(mSystem->getName()); // batocera system-specific music
	AudioManager::getInstance()->init(); // batocera
	window->normalizeNextUpdate();

	//update number of times the game has been launched

	FileData* gameToUpdate = getSourceFileData();

	int timesPlayed = gameToUpdate->metadata.getInt("playcount") + 1;
	gameToUpdate->metadata.set("playcount", std::to_string(static_cast<long long>(timesPlayed)));

	//update last played time
	gameToUpdate->metadata.set("lastplayed", Utils::Time::DateTime(Utils::Time::now()));
	CollectionSystemManager::get()->refreshCollectionSystems(gameToUpdate);

	// batocera
	if (SystemConf::getInstance()->get("audio.bgmusic") != "0")
		AudioManager::getInstance()->playRandomMusic();
}

CollectionFileData::CollectionFileData(FileData* file, SystemData* system)
	: FileData(file->getSourceFileData()->getType(), "", system)
{
	mSourceFileData = file->getSourceFileData();
	mParent = NULL;
	metadata = mSourceFileData->metadata;	
	mDirty = true;
}

SystemEnvironmentData* CollectionFileData::getSystemEnvData() const
{ 
	return mSourceFileData->getSystemEnvData();
}

const std::string CollectionFileData::getPath() const
{
	return mSourceFileData->getPath();
}

std::string CollectionFileData::getSystemName() const
{
	return mSourceFileData->getSystem()->getName();
}

CollectionFileData::~CollectionFileData()
{
	// need to remove collection file data at the collection object destructor
	if(mParent)
		mParent->removeChild(this);
	mParent = NULL;
}

std::string CollectionFileData::getKey() {
	return getFullPath();
}

FileData* CollectionFileData::getSourceFileData()
{
	return mSourceFileData;
}

void CollectionFileData::refreshMetadata()
{
	metadata = mSourceFileData->metadata;
	mDirty = true;
}

const std::string CollectionFileData::getName()
{
	if (mDirty) {
		mCollectionFileName = Utils::String::removeParenthesis(mSourceFileData->metadata.get("name"));
		mCollectionFileName += " [" + Utils::String::toUpper(mSourceFileData->getSystem()->getName()) + "]";
		mDirty = false;
	}

	if (Settings::getInstance()->getBool("CollectionShowSystemInfo"))
		return mCollectionFileName;
		
	return Utils::String::removeParenthesis(mSourceFileData->metadata.get("name"));
}

// returns Sort Type based on a string description
FolderData::SortType getSortTypeFromString(std::string desc) {
	std::vector<FolderData::SortType> SortTypes = FileSorts::SortTypes;
	// find it
	for(unsigned int i = 0; i < FileSorts::SortTypes.size(); i++)
	{
		const FolderData::SortType& sort = FileSorts::SortTypes.at(i);
		if(sort.description == desc)
		{
			return sort;
		}
	}
	// if not found default to name, ascending
	return FileSorts::SortTypes.at(0);
}

const std::vector<FileData*> FolderData::getChildrenListToDisplay() 
{
	std::vector<FileData*> ret;

	bool flatFolders = Settings::getInstance()->getBool("FlatFolders");
	bool showHiddenFiles = Settings::getInstance()->getBool("ShowHiddenFiles");
	bool filterKidGame = false;

	if (!Settings::getInstance()->getBool("ForceDisableFilters"))
	{
		if (UIModeController::getInstance()->isUIModeKiosk())
			showHiddenFiles = false;

		if (UIModeController::getInstance()->isUIModeKid())
			filterKidGame = true;
	}

	if (!flatFolders && !showHiddenFiles && !filterKidGame)
		return mChildren;

	auto sys = CollectionSystemManager::get()->getSystemToView(mSystem);

	FileFilterIndex* idx = sys->getIndex(false);
	if (idx != nullptr && !idx->isFiltered())
		idx = nullptr;

	std::vector<FileData*>* items = &mChildren;
	
	std::vector<FileData*> flatGameList;
	if (flatFolders)
	{
		flatGameList = getFlatGameList(false, sys);
		items = &flatGameList;
	}

	for (auto it = items->cbegin(); it != items->cend(); it++)
	{
		if (idx != nullptr && !idx->showFile((*it)))
			continue;

		if (!showHiddenFiles && (*it)->getHidden())
			continue;

		if (filterKidGame && !(*it)->getKidGame())
			continue;

		ret.push_back(*it);
	}

	return ret;
}

std::vector<FileData*> FolderData::getFlatGameList(bool displayedOnly, SystemData* system) const 
{
	std::vector<FileData*> ret = getFilesRecursive(GAME, displayedOnly, system);

	unsigned int currentSortId = system->getSortId();
	if (currentSortId < 0 || currentSortId >FileSorts::SortTypes.size())
		currentSortId = 0;

	auto sort = FileSorts::SortTypes.at(currentSortId);

	std::stable_sort(ret.begin(), ret.end(), sort.comparisonFunction);

	if (!sort.ascending)
		std::reverse(ret.begin(), ret.end());

	return ret;
}

std::vector<FileData*> FolderData::getFilesRecursive(unsigned int typeMask, bool displayedOnly, SystemData* system) const
{
	std::vector<FileData*> out;

	FileFilterIndex* idx = (system != nullptr ? system : mSystem)->getIndex(false);

	for (auto it = mChildren.cbegin(); it != mChildren.cend(); it++)
	{
		if ((*it)->getType() & typeMask)
		{
			if (!displayedOnly || idx == nullptr || !idx->isFiltered() || idx->showFile(*it))
				out.push_back(*it);
		}

		if ((*it)->getType() != FOLDER)
			continue;

		FolderData* folder = (FolderData*)(*it);
		if (folder->getChildren().size() > 0)
		{
			std::vector<FileData*> subchildren = folder->getFilesRecursive(typeMask, displayedOnly, system);
			out.insert(out.cend(), subchildren.cbegin(), subchildren.cend());
		}
	}

	return out;
}

void FolderData::addChild(FileData* file)
{
	assert(mType == FOLDER);
	assert(file->getParent() == NULL);

	const std::string key = file->getKey();

	mChildren.push_back(file);
	file->setParent(this);	
}

void FolderData::removeChild(FileData* file)
{
	assert(mType == FOLDER);
	assert(file->getParent() == this);

	for (auto it = mChildren.cbegin(); it != mChildren.cend(); it++)
	{
		if (*it == file)
		{
			file->setParent(NULL);
			mChildren.erase(it);
			return;
		}
	}

	// File somehow wasn't in our children.
	assert(false);

}

void FolderData::sort(ComparisonFunction& comparator, bool ascending)
{
	std::stable_sort(mChildren.begin(), mChildren.end(), comparator);

	for (auto it = mChildren.cbegin(); it != mChildren.cend(); it++)
	{
		if ((*it)->getType() != FOLDER)
			continue;

		FolderData* folder = (FolderData*)(*it);

		if (folder->getChildren().size() > 0)
			folder->sort(comparator, ascending);
	}

	if (!ascending)
		std::reverse(mChildren.begin(), mChildren.end());
}

void FolderData::sort(const SortType& type)
{
	sort(*type.comparisonFunction, type.ascending);
}

FileData* FolderData::FindByPath(const std::string& path)
{
	std::vector<FileData*> children = getChildren();

	for (std::vector<FileData*>::const_iterator it = children.cbegin(); it != children.cend(); ++it)
	{
		if ((*it)->getPath() == path)
			return (*it);

		if ((*it)->getType() != FOLDER)
			continue;
		
		auto item = ((FolderData*)(*it))->FindByPath(path);
		if (item != nullptr)
			return item;
	}

	return nullptr;
}

void FolderData::createChildrenByFilenameMap(std::unordered_map<std::string, FileData*>& map)
{
	std::vector<FileData*> children = getChildren();

	for (std::vector<FileData*>::const_iterator it = children.cbegin(); it != children.cend(); ++it)
	{
		if ((*it)->getType() == FOLDER)
			((FolderData*)(*it))->createChildrenByFilenameMap(map);			
		else 
			map[(*it)->getKey()] = (*it);
	}	
}
