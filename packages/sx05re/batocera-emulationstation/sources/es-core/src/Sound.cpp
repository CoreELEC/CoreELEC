#include "Sound.h"

#include "AudioManager.h"
#include "Log.h"
#include "Settings.h"
#include "ThemeData.h"

std::map< std::string, std::shared_ptr<Sound> > Sound::sMap;

std::shared_ptr<Sound> Sound::get(const std::string& path)
{
	auto it = sMap.find(path);
	if (it != sMap.cend())
		return it->second;

	std::shared_ptr<Sound> sound = std::shared_ptr<Sound>(new Sound(path));
	AudioManager::getInstance()->registerSound(sound);
	sMap[path] = sound;
	return sound;
}

std::shared_ptr<Sound> Sound::getFromTheme(const std::shared_ptr<ThemeData>& theme, const std::string& view, const std::string& element)
{
	LOG(LogInfo) << " req sound [" << view << "." << element << "]";

	const ThemeData::ThemeElement* elem = theme->getElement(view, element, "sound");
	if (!elem || !elem->has("path"))
	{
		LOG(LogInfo) << "   (missing)";
		return get("");
	}

	return get(elem->get<std::string>("path"));
}

Sound::Sound(const std::string & path) : mSampleData(NULL), mPlayingChannel(-1)
{
	loadFile(path);
}

Sound::~Sound()
{
	deinit();
}

void Sound::loadFile(const std::string & path)
{
	mPath = path;
	init();
}

void Sound::init()
{
	deinit();

	if (!AudioManager::isInitialized())
		return;

	if (mPath.empty() || !Utils::FileSystem::exists(mPath))
		return;

	if (!Settings::getInstance()->getBool("EnableSounds"))
		return;

	//load wav file via SDL
	mSampleData = Mix_LoadWAV(mPath.c_str());
	if (mSampleData == nullptr)
	{
		LOG(LogError) << "Error loading sound \"" << mPath << "\"!\n" << "	" << SDL_GetError();
		return;
	}
}

void Sound::deinit()
{
	if (mSampleData == nullptr)
		return;

	stop();
	Mix_FreeChunk(mSampleData);
	mSampleData = nullptr;	
}

void Sound::play()
{
	if (mSampleData == nullptr)
		return;

	if (!Settings::getInstance()->getBool("EnableSounds"))
		return;

	mPlayingChannel = Mix_PlayChannel(-1, mSampleData, 0);
}

bool Sound::isPlaying() const
{	
	return (mPlayingChannel >= 0);
}

void Sound::stop()
{
	if (mPlayingChannel < 0)
		return;

	//Mix_HaltChannel(mPlayingChannel);
	mPlayingChannel = -1;
}
