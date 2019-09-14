#pragma once
#ifndef ES_CORE_SOUND_H
#define ES_CORE_SOUND_H

#include "SDL_mixer.h"
#include <map>
#include <memory>

class ThemeData;

class Sound
{
	std::string mPath;
	Mix_Chunk* mSampleData;
	int mPlayingChannel;

public:
	static std::shared_ptr<Sound> get(const std::string& path);
	static std::shared_ptr<Sound> getFromTheme(const std::shared_ptr<ThemeData>& theme, const std::string& view, const std::string& elem);

	~Sound();

	void init();
	void deinit();

	void loadFile(const std::string & path);

	void play();
	bool isPlaying() const;
	void stop();

private:
	Sound(const std::string & path = "");
	static std::map< std::string, std::shared_ptr<Sound> > sMap;
};

#endif // ES_CORE_SOUND_H
