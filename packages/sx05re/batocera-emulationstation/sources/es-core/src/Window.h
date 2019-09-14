#pragma once
#ifndef ES_CORE_WINDOW_H
#define ES_CORE_WInDOW_H

#include "HelpPrompt.h"
#include "InputConfig.h"
#include "Settings.h"
#include "math/Vector2f.h"
#include <memory>

class FileData;
class Font;
class GuiComponent;
class HelpComponent;
class ImageComponent;
class InputConfig;
class TextCache;
class Transform4x4f;
struct HelpStyle;
class TextureResource;

class Window
{
public:
	class ScreenSaver {
	public:
		virtual void startScreenSaver() = 0;
		virtual void stopScreenSaver() = 0;
		virtual void nextVideo() = 0;
		virtual void renderScreenSaver() = 0;
		virtual bool allowSleep() = 0;
		virtual void update(int deltaTime) = 0;
		virtual bool isScreenSaverActive() = 0;
		virtual FileData* getCurrentGame() = 0;
		virtual void launchGame() = 0;
		virtual void resetCounts() = 0;
	};

	class InfoPopup {
	public:
		virtual void render(const Transform4x4f& parentTrans) = 0;
		virtual void stop() = 0;
		virtual ~InfoPopup() {};
	};

	Window();
	~Window();

	void pushGui(GuiComponent* gui);
	void displayMessage(std::string message); // batocera
	void removeGui(GuiComponent* gui);
	GuiComponent* peekGui();
	inline int getGuiStackSize() { return (int)mGuiStack.size(); }

	void textInput(const char* text);
	void input(InputConfig* config, Input input);
	void update(int deltaTime);
	void render();

	bool init();
	void deinit();

	void normalizeNextUpdate();

	inline bool isSleeping() const { return mSleeping; }
	bool getAllowSleep();
	void setAllowSleep(bool sleep);

	void renderLoadingScreen(std::string text, float percent = -1, unsigned char opacity = 255);
	void endRenderLoadingScreen();

	void renderHelpPromptsEarly(); // used to render HelpPrompts before a fade
	void setHelpPrompts(const std::vector<HelpPrompt>& prompts, const HelpStyle& style);

	void setScreenSaver(ScreenSaver* screenSaver) { mScreenSaver = screenSaver; }
	void setInfoPopup(InfoPopup* infoPopup) { if (mInfoPopup) delete mInfoPopup; mInfoPopup = infoPopup; }
	inline void stopInfoPopup() { if (mInfoPopup) mInfoPopup->stop(); };

	void startScreenSaver();
	bool cancelScreenSaver();
	void renderScreenSaver();

private:
	void onSleep();
	void onWake();

	// Returns true if at least one component on the stack is processing
	bool isProcessing();

	HelpComponent* mHelp;
	ImageComponent* mBackgroundOverlay;
	ScreenSaver*	mScreenSaver;
	InfoPopup*		mInfoPopup;
	bool			mRenderScreenSaver;

	std::vector<GuiComponent*> mGuiStack;
	std::vector<std::string> mMessages; // batocera

	std::vector< std::shared_ptr<Font> > mDefaultFonts;
	std::shared_ptr<TextureResource> mSplash;

	int mFrameTimeElapsed;
	int mFrameCountElapsed;
	int mAverageDeltaTime;

	std::unique_ptr<TextCache> mFrameDataText;

	// clock // batocera
	int mClockElapsed;

	unsigned int mClockColor;
	Vector2f mClockPos;
	std::shared_ptr<Font> mClockFont;
	std::unique_ptr<TextCache> mClockText;

	// pads // batocera
	int mplayerPads[MAX_PLAYERS];
	bool mplayerPadsIsHotkey;

	bool mNormalizeNextUpdate;

	bool mAllowSleep;
	bool mSleeping;
	unsigned int mTimeSinceLastInput;

	bool mRenderedHelpPrompts;
};

#endif // ES_CORE_WINDOW_H
