#include "guis/GuiMenu.h"

#include "components/OptionListComponent.h"
#include "components/SliderComponent.h"
#include "components/SwitchComponent.h"
#include "guis/GuiCollectionSystemsOptions.h"
#include "guis/GuiDetectDevice.h"
#include "guis/GuiGeneralScreensaverOptions.h"
#include "guis/GuiSlideshowScreensaverOptions.h"
#include "guis/GuiVideoScreensaverOptions.h"
#include "guis/GuiMsgBox.h"
#include "guis/GuiScraperStart.h"
#include "guis/GuiThemeInstallStart.h" //batocera
#include "guis/GuiBezelInstallStart.h" //batocera
#include "guis/GuiSettings.h"
#include "guis/GuiSystemsHide.h" //batocera
#include "views/UIModeController.h"
#include "views/ViewController.h"
#include "CollectionSystemManager.h"
#include "EmulationStation.h"
#include "Scripting.h"
#include "SystemData.h"
#include "VolumeControl.h"
#include <SDL_events.h>
#include <algorithm>
#include "platform.h"

#include "SystemConf.h"
#include "ApiSystem.h"
#include "InputManager.h"
#include "AudioManager.h"
#include <LibretroRatio.h>
#include "GuiLoading.h"
#include "guis/GuiAutoScrape.h"
#include "guis/GuiUpdate.h"
#include "guis/GuiInstallStart.h"
#include "guis/GuiTextEditPopupKeyboard.h"
#include "guis/GuiBackupStart.h"
#include "guis/GuiTextEditPopup.h"

GuiMenu::GuiMenu(Window *window) : GuiComponent(window), mMenu(window, _("MAIN MENU").c_str()), mVersion(window)
{
	// MAIN MENU
	bool isFullUI = UIModeController::getInstance()->isUIModeFull();

	// KODI >
	// GAMES SETTINGS >
	// CONTROLLERS >
	// UI SETTINGS >
	// SOUND SETTINGS >
	// NETWORK >
	// SCRAPER >
	// SYSTEM SETTINGS >
	// QUIT >

	// KODI
#ifdef _ENABLE_KODI_
	if (SystemConf::getInstance()->get("kodi.enabled") != "0")
		addEntry(_("KODI MEDIA CENTER").c_str(), false, [this] { openKodiLauncher_batocera(); }, "iconKodi");	
#endif

	if (isFullUI &&
		SystemConf::getInstance()->get("global.retroachievements") == "1" &&
		SystemConf::getInstance()->get("global.retroachievements.username") != "")
		addEntry(_("RETROACHIEVEMENTS").c_str(), true, [this] { openRetroAchievements_batocera(); });

	// GAMES SETTINGS
	if (isFullUI)
		addEntry(_("GAMES SETTINGS").c_str(), true, [this] { openGamesSettings_batocera(); }, "iconGames");

	// CONTROLLERS SETTINGS
	if (isFullUI)
		addEntry(_("CONTROLLERS SETTINGS").c_str(), true, [this] { openControllersSettings_batocera(); }, "iconControllers");

	if (isFullUI)
		addEntry(_("UI SETTINGS").c_str(), true, [this] { openUISettings(); }, "iconUI");

	// batocera
	if (isFullUI)
		addEntry(_("GAME COLLECTION SETTINGS").c_str(), true, [this] { openCollectionSystemSettings(); }, "iconAdvanced");

	if (isFullUI)
		addEntry(_("SOUND SETTINGS").c_str(), true, [this] { openSoundSettings(); }, "iconSound");

	if (isFullUI)
		addEntry(_("NETWORK SETTINGS").c_str(), true, [this] { openNetworkSettings_batocera(); }, "iconNetwork");

	if (isFullUI)
		addEntry(_("SCRAPE").c_str(), true, [this] { openScraperSettings_batocera(); }, "iconScraper");

	// SYSTEM
	if (isFullUI)
		addEntry(_("SYSTEM SETTINGS").c_str(), true, [this] { openSystemSettings_batocera(); }, "iconSystem");
	else
		addEntry(_("INFORMATIONS").c_str(), true, [this] { openSystemInformations_batocera(); }, "iconSystem");

#ifdef WIN32
	addEntry(_("QUIT").c_str(), false, [this] { openQuitMenu_batocera(); }, "iconQuit");
#else
	addEntry(_("QUIT").c_str(), true, [this] { openQuitMenu_batocera(); }, "iconQuit");
#endif
	
	addChild(&mMenu);
	addVersionInfo(); // batocera
	setSize(mMenu.getSize());

	if (Renderer::isSmallScreen())
		animateTo((Renderer::getScreenWidth() - mSize.x()) / 2, (Renderer::getScreenHeight() - mSize.y()) / 2);
	else
		animateTo(
			Vector2f((Renderer::getScreenWidth() - mSize.x()) / 2, Renderer::getScreenHeight() * 0.9),
			Vector2f((Renderer::getScreenWidth() - mSize.x()) / 2, Renderer::getScreenHeight() * 0.15f));
}

void GuiMenu::openScraperSettings()
{
	auto s = new GuiSettings(mWindow, "SCRAPER");

	std::string scraper = Settings::getInstance()->getString("Scraper");

	// scrape from
	auto scraper_list = std::make_shared< OptionListComponent< std::string > >(mWindow, "SCRAPE FROM", false);
	std::vector<std::string> scrapers = getScraperList();

	// Select either the first entry of the one read from the settings, just in case the scraper from settings has vanished.
	for(auto it = scrapers.cbegin(); it != scrapers.cend(); it++)
		scraper_list->add(*it, *it, *it == scraper);

	s->addWithLabel(_("SCRAPE FROM"), scraper_list); // batocera
	s->addSaveFunc([scraper_list] { Settings::getInstance()->setString("Scraper", scraper_list->getSelected()); });

	// scrape ratings
	auto scrape_ratings = std::make_shared<SwitchComponent>(mWindow);
	scrape_ratings->setState(Settings::getInstance()->getBool("ScrapeRatings"));
	s->addWithLabel(_("SCRAPE RATINGS"), scrape_ratings); // batocera
	s->addSaveFunc([scrape_ratings] { Settings::getInstance()->setBool("ScrapeRatings", scrape_ratings->getState()); });

	if (scraper == "ScreenScraper")
	{
		// image source
		std::string imageSourceName = Settings::getInstance()->getString("ScrapperImageSrc");
		auto imageSource = std::make_shared< OptionListComponent<std::string> >(mWindow, _("PREFERED IMAGE SOURCE"), false);
		imageSource->add(_("NONE"), "", imageSourceName.empty());
		imageSource->add(_("SCREENSHOT"), "ss", imageSourceName == "ss");
		imageSource->add(_("BOX 2D"), "box-2D", imageSourceName == "box-2D");
		imageSource->add(_("BOX 3D"), "box-3D", imageSourceName == "box-3D");
		imageSource->add(_("MIX"), "mixrbv1", imageSourceName == "mixrbv1");
		imageSource->add(_("WHEEL"), "wheel", imageSourceName == "wheel");
		s->addWithLabel("PREFERED IMAGE SOURCE", imageSource);

		s->addSaveFunc([imageSource] {
			if (Settings::getInstance()->getString("ScrapperImageSrc") != imageSource->getSelected())
				Settings::getInstance()->setString("ScrapperImageSrc", imageSource->getSelected());
		});
		
		std::string thumbSourceName = Settings::getInstance()->getString("ScrapperThumbSrc");
		auto thumbSource = std::make_shared< OptionListComponent<std::string> >(mWindow, _("PREFERED THUMBNAIL SOURCE"), false);
		thumbSource->add(_("NONE"), "", thumbSourceName.empty());
		thumbSource->add(_("SCREENSHOT"), "ss", thumbSourceName == "ss");
		thumbSource->add(_("BOX 2D"), "box-2D", thumbSourceName == "box-2D");
		thumbSource->add(_("BOX 3D"), "box-3D", thumbSourceName == "box-3D");
		thumbSource->add(_("MIX"), "mixrbv1", thumbSourceName == "mixrbv1");
		thumbSource->add(_("WHEEL"), "wheel", thumbSourceName == "wheel");
		s->addWithLabel("PREFERED THUMBNAIL SOURCE", thumbSource);

		s->addSaveFunc([thumbSource] {
			if (Settings::getInstance()->getString("ScrapperThumbSrc") != thumbSource->getSelected())
				Settings::getInstance()->setString("ScrapperThumbSrc", thumbSource->getSelected());
		});

		// scrape marquee
		auto scrape_marquee = std::make_shared<SwitchComponent>(mWindow);
		scrape_marquee->setState(Settings::getInstance()->getBool("ScrapeMarquee"));
		s->addWithLabel(_("SCRAPE MARQUEE"), scrape_marquee);
		s->addSaveFunc([scrape_marquee] { Settings::getInstance()->setBool("ScrapeMarquee", scrape_marquee->getState()); });

		// scrape video
		auto scrape_video = std::make_shared<SwitchComponent>(mWindow);
		scrape_video->setState(Settings::getInstance()->getBool("ScrapeVideos"));
		s->addWithLabel(_("SCRAPE VIDEOS"), scrape_video);
		s->addSaveFunc([scrape_video] { Settings::getInstance()->setBool("ScrapeVideos", scrape_video->getState()); });
	}

	// scrape now
	ComponentListRow row;
	auto openScrapeNow = [this] { mWindow->pushGui(new GuiScraperStart(mWindow)); };
	std::function<void()> openAndSave = openScrapeNow;
	openAndSave = [s, openAndSave] { s->save(); openAndSave(); };
	s->addEntry(_("SCRAPE NOW"), false, openAndSave, "iconScraper");
		
	scraper_list->setSelectedChangedCallback([this, s, scraper, scraper_list](std::string value)
	{		
		if (value != scraper && (scraper == "ScreenScraper" || value == "ScreenScraper"))
		{
			Settings::getInstance()->setString("Scraper", value);
			delete s;
			openScraperSettings();
		}
	});

	mWindow->pushGui(s);
}

void GuiMenu::openOtherSettings()
{
	auto s = new GuiSettings(mWindow, "OTHER SETTINGS");

	// maximum vram
	auto max_vram = std::make_shared<SliderComponent>(mWindow, 0.f, 1000.f, 10.f, "Mb");
	max_vram->setValue((float)(Settings::getInstance()->getInt("MaxVRAM")));
	s->addWithLabel("VRAM LIMIT", max_vram);
	s->addSaveFunc([max_vram] { Settings::getInstance()->setInt("MaxVRAM", (int)Math::round(max_vram->getValue())); });

	// power saver
	auto power_saver = std::make_shared< OptionListComponent<std::string> >(mWindow, "POWER SAVER MODES", false);
	std::vector<std::string> modes;
	modes.push_back("disabled");
	modes.push_back("default");
	modes.push_back("enhanced");
	modes.push_back("instant");
	for (auto it = modes.cbegin(); it != modes.cend(); it++)
		power_saver->add(*it, *it, Settings::getInstance()->getString("PowerSaverMode") == *it);
	s->addWithLabel("POWER SAVER MODES", power_saver);
	s->addSaveFunc([this, power_saver] {
		if (Settings::getInstance()->getString("PowerSaverMode") != "instant" && power_saver->getSelected() == "instant") {
			Settings::getInstance()->setString("TransitionStyle", "instant");
			Settings::getInstance()->setBool("MoveCarousel", false);
			Settings::getInstance()->setBool("EnableSounds", false);
		}
		Settings::getInstance()->setString("PowerSaverMode", power_saver->getSelected());
		PowerSaver::init();
	});

	// gamelists
	auto save_gamelists = std::make_shared<SwitchComponent>(mWindow);
	save_gamelists->setState(Settings::getInstance()->getBool("SaveGamelistsOnExit"));
	s->addWithLabel("SAVE METADATA ON EXIT", save_gamelists);
	s->addSaveFunc([save_gamelists] { Settings::getInstance()->setBool("SaveGamelistsOnExit", save_gamelists->getState()); });

	auto parse_gamelists = std::make_shared<SwitchComponent>(mWindow);
	parse_gamelists->setState(Settings::getInstance()->getBool("ParseGamelistOnly"));
	s->addWithLabel("PARSE GAMESLISTS ONLY", parse_gamelists);
	s->addSaveFunc([parse_gamelists] { Settings::getInstance()->setBool("ParseGamelistOnly", parse_gamelists->getState()); });

	auto local_art = std::make_shared<SwitchComponent>(mWindow);
	local_art->setState(Settings::getInstance()->getBool("LocalArt"));
	s->addWithLabel("SEARCH FOR LOCAL ART", local_art);
	s->addSaveFunc([local_art] { Settings::getInstance()->setBool("LocalArt", local_art->getState()); });
	/*
	// hidden files
	auto hidden_files = std::make_shared<SwitchComponent>(mWindow);
	hidden_files->setState(Settings::getInstance()->getBool("ShowHiddenFiles"));
	s->addWithLabel(_("SHOW HIDDEN FILES"), hidden_files);
	s->addSaveFunc([hidden_files] { Settings::getInstance()->setBool("ShowHiddenFiles", hidden_files->getState()); });
	*/
#ifdef _RPI_
	// Video Player - VideoOmxPlayer
	auto omx_player = std::make_shared<SwitchComponent>(mWindow);
	omx_player->setState(Settings::getInstance()->getBool("VideoOmxPlayer"));
	s->addWithLabel("USE OMX PLAYER (HW ACCELERATED)", omx_player);
	s->addSaveFunc([omx_player]
	{
		// need to reload all views to re-create the right video components
		bool needReload = false;
		if(Settings::getInstance()->getBool("VideoOmxPlayer") != omx_player->getState())
			needReload = true;

		Settings::getInstance()->setBool("VideoOmxPlayer", omx_player->getState());

		if(needReload)
			ViewController::get()->reloadAll();
	});

#endif

	// framerate
	auto framerate = std::make_shared<SwitchComponent>(mWindow);
	framerate->setState(Settings::getInstance()->getBool("DrawFramerate"));
	s->addWithLabel("SHOW FRAMERATE", framerate);
	s->addSaveFunc([framerate] { Settings::getInstance()->setBool("DrawFramerate", framerate->getState()); });


	mWindow->pushGui(s);
}

void GuiMenu::openConfigInput()
{
	Window* window = mWindow;
	window->pushGui(new GuiMsgBox(window, "ARE YOU SURE YOU WANT TO CONFIGURE INPUT?", "YES",
		[window] {
			window->pushGui(new GuiDetectDevice(window, false, nullptr));
		}, "NO", nullptr)
	);
}

void GuiMenu::addVersionInfo()
{
	std::string  buildDate = (Settings::getInstance()->getBool("Debug") ? std::string( "   (" + Utils::String::toUpper(PROGRAM_BUILT_STRING) + ")") : (""));

	auto theme = ThemeData::getMenuTheme();

	mVersion.setFont(theme->Footer.font);
	mVersion.setColor(theme->Footer.color);

	mVersion.setLineSpacing(0);
	if (!ApiSystem::getInstance()->getVersion().empty())
		mVersion.setText("BATOCERA.LINUX ES V" + ApiSystem::getInstance()->getVersion() + buildDate);

	mVersion.setHorizontalAlignment(ALIGN_CENTER);
	mVersion.setVerticalAlignment(ALIGN_CENTER);
	addChild(&mVersion);
}

void GuiMenu::openScreensaverOptions() 
{
	mWindow->pushGui(new GuiGeneralScreensaverOptions(mWindow, "SCREENSAVER SETTINGS"));
}

// new screensaver options for Batocera
void GuiMenu::openSlideshowScreensaverOptions() 
{
	mWindow->pushGui(new GuiSlideshowScreensaverOptions(mWindow, _("SLIDESHOW SETTINGS").c_str()));
}

// new screensaver options for Batocera
void GuiMenu::openVideoScreensaverOptions() {
	mWindow->pushGui(new GuiVideoScreensaverOptions(mWindow, _("RANDOM VIDEO SETTINGS").c_str()));
}


void GuiMenu::openCollectionSystemSettings() {
	mWindow->pushGui(new GuiCollectionSystemsOptions(mWindow));
}

void GuiMenu::onSizeChanged()
{
	float h = mMenu.getButtonGridHeight();

	mVersion.setSize(mSize.x(), h);
	mVersion.setPosition(0, mSize.y() - h); //  mVersion.getSize().y()
}

void GuiMenu::addEntry(std::string name, bool add_arrow, const std::function<void()>& func, const std::string iconName)
{
	auto theme = ThemeData::getMenuTheme();
	std::shared_ptr<Font> font = theme->Text.font;
	unsigned int color = theme->Text.color;

	// populate the list
	ComponentListRow row;

	if (!iconName.empty())
	{
		std::string iconPath = theme->getMenuIcon(iconName);
		if (!iconPath.empty())
		{
			// icon
			auto icon = std::make_shared<ImageComponent>(mWindow);
			icon->setImage(iconPath);
			icon->setColorShift(theme->Text.color);
			icon->setResize(0, theme->Text.font->getLetterHeight() * 1.25f);
			row.addElement(icon, false);

			// spacer between icon and text
			auto spacer = std::make_shared<GuiComponent>(mWindow);
			spacer->setSize(10, 0);
			row.addElement(spacer, false);
		}
	}

	row.addElement(std::make_shared<TextComponent>(mWindow, name, font, color), true);

	if (add_arrow)
	{
		std::shared_ptr<ImageComponent> bracket = makeArrow(mWindow);
		row.addElement(bracket, false);
	}

	row.makeAcceptInputHandler(func);
	mMenu.addRow(row);
}

bool GuiMenu::input(InputConfig* config, Input input)
{
	if(GuiComponent::input(config, input))
		return true;

	if((config->isMappedTo(BUTTON_BACK, input) || config->isMappedTo("start", input)) && input.value != 0)
	{
		delete this;
		return true;
	}

	return false;
}

std::vector<HelpPrompt> GuiMenu::getHelpPrompts()
{
	std::vector<HelpPrompt> prompts;
	prompts.push_back(HelpPrompt("up/down", _("CHOOSE"))); // batocera
	prompts.push_back(HelpPrompt(BUTTON_OK, _("SELECT"))); // batocera
	prompts.push_back(HelpPrompt("start", _("CLOSE"))); // batocera
	return prompts;
}

void GuiMenu::openKodiLauncher_batocera()
{
  Window *window = mWindow;
  if (!ApiSystem::getInstance()->launchKodi(window)) {
    LOG(LogWarning) << "Shutdown terminated with non-zero result!";
  }
}

void GuiMenu::openSystemInformations_batocera()
{
	auto theme = ThemeData::getMenuTheme();
	std::shared_ptr<Font> font = theme->Text.font;
	unsigned int color = theme->Text.color;


	Window *window = mWindow;
	bool isFullUI = UIModeController::getInstance()->isUIModeFull();
	GuiSettings *informationsGui = new GuiSettings(window, _("INFORMATION").c_str());

	auto version = std::make_shared<TextComponent>(window, ApiSystem::getInstance()->getVersion(), font, color);
	informationsGui->addWithLabel(_("VERSION"), version);

	bool warning = ApiSystem::getInstance()->isFreeSpaceLimit();
	auto space = std::make_shared<TextComponent>(window,
		ApiSystem::getInstance()->getFreeSpaceInfo(),
		font,
		warning ? 0xFF0000FF : color);
	informationsGui->addWithLabel(_("DISK USAGE"), space);

	// various informations
	std::vector<std::string> infos = ApiSystem::getInstance()->getSystemInformations();
	for (auto it = infos.begin(); it != infos.end(); it++) {
		std::vector<std::string> tokens = Utils::String::split(*it, ':');

		if (tokens.size() >= 2) {
			// concatenat the ending words
			std::string vname = "";
			for (unsigned int i = 1; i < tokens.size(); i++) {
				if (i > 1) vname += " ";
				vname += tokens.at(i);
			}

			auto space = std::make_shared<TextComponent>(window,
				vname,
				font,
				color);
			informationsGui->addWithLabel(tokens.at(0), space);
		}
	}
	
	window->pushGui(informationsGui);
}

void GuiMenu::openDeveloperSettings()
{
	Window *window = mWindow;

	auto s = new GuiSettings(mWindow, _("DEVELOPER").c_str());
	
	// maximum vram
	auto max_vram = std::make_shared<SliderComponent>(mWindow, 0.f, 1000.f, 10.f, "Mb");
	max_vram->setValue((float)(Settings::getInstance()->getInt("MaxVRAM")));
	s->addWithLabel(_("VRAM LIMIT"), max_vram);
	s->addSaveFunc([max_vram] { Settings::getInstance()->setInt("MaxVRAM", (int)round(max_vram->getValue())); });
	
	// framerate
	auto framerate = std::make_shared<SwitchComponent>(mWindow);
	framerate->setState(Settings::getInstance()->getBool("DrawFramerate"));
	s->addWithLabel(_("SHOW FRAMERATE"), framerate);
	s->addSaveFunc(
		[framerate] { Settings::getInstance()->setBool("DrawFramerate", framerate->getState()); });
	
	// vsync
	auto vsync = std::make_shared<SwitchComponent>(mWindow);
	vsync->setState(Settings::getInstance()->getBool("VSync"));
	s->addWithLabel(_("VSYNC"), vsync);
	s->addSaveFunc([vsync]
	{
		if (Settings::getInstance()->setBool("VSync", vsync->getState()))
			Renderer::setSwapInterval();
	});

	// preload UI
	auto preloadUI = std::make_shared<SwitchComponent>(mWindow);
	preloadUI->setState(Settings::getInstance()->getBool("PreloadUI"));
	s->addWithLabel(_("PRELOAD UI"), preloadUI);
	s->addSaveFunc([preloadUI] { Settings::getInstance()->setBool("PreloadUI", preloadUI->getState()); });

	// threaded loading
	auto threadedLoading = std::make_shared<SwitchComponent>(mWindow);
	threadedLoading->setState(Settings::getInstance()->getBool("ThreadedLoading"));
	s->addWithLabel(_("THREADED LOADING"), threadedLoading);
	s->addSaveFunc([threadedLoading] { Settings::getInstance()->setBool("ThreadedLoading", threadedLoading->getState()); });

	// threaded loading
	auto asyncImages = std::make_shared<SwitchComponent>(mWindow);
	asyncImages->setState(Settings::getInstance()->getBool("AsyncImages"));
	s->addWithLabel(_("ASYNC IMAGES LOADING"), asyncImages);
	s->addSaveFunc([asyncImages] { Settings::getInstance()->setBool("AsyncImages", asyncImages->getState()); });
	
	// optimizeVram
	auto optimizeVram = std::make_shared<SwitchComponent>(mWindow);
	optimizeVram->setState(Settings::getInstance()->getBool("OptimizeVRAM"));
	s->addWithLabel(_("OPTIMIZE IMAGES VRAM USE"), optimizeVram);
	s->addSaveFunc([optimizeVram] { Settings::getInstance()->setBool("OptimizeVRAM", optimizeVram->getState()); });
	
	// enableLogs
	auto enableLogs = std::make_shared<SwitchComponent>(mWindow);
	enableLogs->setState(Settings::getInstance()->getBool("EnableLogging"));
	s->addWithLabel(_("ENABLE LOG FILE"), enableLogs);
	s->addSaveFunc([enableLogs]
	{ 
		if (Settings::getInstance()->setBool("EnableLogging", enableLogs->getState()))
			Log::init();
	});


	// support
	s->addEntry(_("CREATE A SUPPORT FILE"), true, [window] {
		window->pushGui(new GuiMsgBox(window, _("CREATE A SUPPORT FILE ?"), _("YES"),
			[window] {
			if (ApiSystem::getInstance()->generateSupportFile()) {
				window->pushGui(new GuiMsgBox(window, _("FILE GENERATED SUCCESSFULLY"), _("OK")));
			}
			else {
				window->pushGui(new GuiMsgBox(window, _("FILE GENERATION FAILED"), _("OK")));
			}
		}, _("NO"), nullptr));
	});


#ifdef _RPI_
	// Video Player - VideoOmxPlayer
	auto omx_player = std::make_shared<SwitchComponent>(mWindow);
	omx_player->setState(Settings::getInstance()->getBool("VideoOmxPlayer"));
	s->addWithLabel("USE OMX PLAYER (HW ACCELERATED)", omx_player);
	s->addSaveFunc([omx_player, window]
	{
		// need to reload all views to re-create the right video components
		bool needReload = false;
		if (Settings::getInstance()->getBool("VideoOmxPlayer") != omx_player->getState())
			needReload = true;

		Settings::getInstance()->setBool("VideoOmxPlayer", omx_player->getState());

		if (needReload)
		{
			ViewController::get()->reloadAll(window);
			window->endRenderLoadingScreen();
		}
	});
#endif

	mWindow->pushGui(s);
}

void GuiMenu::openSystemSettings_batocera() 
{
	Window *window = mWindow;

	auto s = new GuiSettings(mWindow, _("SYSTEM SETTINGS").c_str());
	bool isFullUI = UIModeController::getInstance()->isUIModeFull();

	// system informations
	s->addEntry(_("INFORMATION"), true, [this] { openSystemInformations_batocera(); }, "iconSystem");

	std::vector<std::string> availableStorage = ApiSystem::getInstance()->getAvailableStorageDevices();
	std::string selectedStorage = ApiSystem::getInstance()->getCurrentStorage();

	// Storage device
	auto optionsStorage = std::make_shared<OptionListComponent<std::string> >(window, _("STORAGE DEVICE"), false);
	for (auto it = availableStorage.begin(); it != availableStorage.end(); it++) 
	{
		if ((*it) != "RAM")
		{
			if (Utils::String::startsWith(*it, "DEV"))
			{
				std::vector<std::string> tokens = Utils::String::split(*it, ' ');

				if (tokens.size() >= 3) {
					// concatenat the ending words
					std::string vname = "";
					for (unsigned int i = 2; i < tokens.size(); i++) {
						if (i > 2) vname += " ";
						vname += tokens.at(i);
					}
					optionsStorage->add(vname, (*it), selectedStorage == std::string("DEV " + tokens.at(1)));
				}
			}
			else {
				optionsStorage->add((*it), (*it), selectedStorage == (*it));
			}
		}
	}
	s->addWithLabel(_("STORAGE DEVICE"), optionsStorage);
	
	// language choice
	auto language_choice = std::make_shared<OptionListComponent<std::string> >(window, _("LANGUAGE"), false);

	std::string language = SystemConf::getInstance()->get("system.language");
	if (language.empty()) 
		language = "en_US";

	language_choice->add("BASQUE", "eu_ES", language == "eu_ES");
	language_choice->add("正體中文", "zh_TW", language == "zh_TW");
	language_choice->add("简体中文", "zh_CN", language == "zh_CN");
	language_choice->add("DEUTSCH", "de_DE", language == "de_DE");
	language_choice->add("ENGLISH", "en_US", language == "en_US");
	language_choice->add("ESPAÑOL", "es_ES", language == "es_ES");
	language_choice->add("FRANÇAIS", "fr_FR", language == "fr_FR");
	language_choice->add("ITALIANO", "it_IT", language == "it_IT");
	language_choice->add("PORTUGUES BRASILEIRO", "pt_BR", language == "pt_BR");
	language_choice->add("PORTUGUES PORTUGAL", "pt_PT", language == "pt_PT");
	language_choice->add("SVENSKA", "sv_SE", language == "sv_SE");
	language_choice->add("TÜRKÇE", "tr_TR", language == "tr_TR");
	language_choice->add("CATALÀ", "ca_ES", language == "ca_ES");
	language_choice->add("ARABIC", "ar_YE", language == "ar_YE");
	language_choice->add("DUTCH", "nl_NL", language == "nl_NL");
	language_choice->add("GREEK", "el_GR", language == "el_GR");
	language_choice->add("KOREAN", "ko_KR", language == "ko_KR");
	language_choice->add("NORWEGIAN", "nn_NO", language == "nn_NO");
	language_choice->add("NORWEGIAN BOKMAL", "nb_NO", language == "nb_NO");
	language_choice->add("POLISH", "pl_PL", language == "pl_PL");
	language_choice->add("JAPANESE", "ja_JP", language == "ja_JP");
	language_choice->add("RUSSIAN", "ru_RU", language == "ru_RU");
	language_choice->add("HUNGARIAN", "hu_HU", language == "hu_HU");

	s->addWithLabel(_("LANGUAGE"), language_choice);

	// Overclock choice
	auto overclock_choice = std::make_shared<OptionListComponent<std::string> >(window, _("OVERCLOCK"), false);

	std::string currentOverclock = Settings::getInstance()->getString("Overclock");
	if (currentOverclock == "") 
		currentOverclock = "none";

	std::vector<std::string> availableOverclocking = ApiSystem::getInstance()->getAvailableOverclocking();

	// Overclocking device
	bool isOneSet = false;
	for (auto it = availableOverclocking.begin(); it != availableOverclocking.end(); it++) 
	{
		std::vector<std::string> tokens = Utils::String::split(*it, ' ');		
		if (tokens.size() >= 2) 
		{
			// concatenat the ending words
			std::string vname;
			for (unsigned int i = 1; i < tokens.size(); i++) 
			{
				if (i > 1) vname += " ";
				vname += tokens.at(i);
			}
			bool isSet = currentOverclock == std::string(tokens.at(0));
			if (isSet) 
				isOneSet = true;
			
			overclock_choice->add(vname, tokens.at(0), isSet);
		}
	}

	if (isOneSet == false)
		overclock_choice->add(currentOverclock, currentOverclock, true);
	
#ifndef WIN32
	s->addWithLabel(_("OVERCLOCK"), overclock_choice);
#endif

	// Updates
	s->addEntry(_("UPDATES"), true, [this] 
	{
		GuiSettings *updateGui = new GuiSettings(mWindow, _("UPDATES").c_str());

		// Batocera themes installer/browser
		updateGui->addEntry(_("THEMES"), true, [this] { mWindow->pushGui(new GuiThemeInstallStart(mWindow)); });

		// Batocera integration with theBezelProject
		updateGui->addEntry(_("THE BEZEL PROJECT"), true, [this] { mWindow->pushGui(new GuiBezelInstallMenu(mWindow)); });

		#ifndef WIN32

		// Enable updates
		auto updates_enabled = std::make_shared<SwitchComponent>(mWindow);
		updates_enabled->setState(SystemConf::getInstance()->get("updates.enabled") == "1");
		updateGui->addWithLabel(_("AUTO UPDATES"), updates_enabled);

		// Start update
		updateGui->addEntry(_("START UPDATE"), true, [this] { mWindow->pushGui(new GuiUpdate(mWindow)); });

		updateGui->addSaveFunc([updates_enabled] 
		{
			SystemConf::getInstance()->set("updates.enabled", updates_enabled->getState() ? "1" : "0");
			SystemConf::getInstance()->saveSystemConf();
		});

		#endif

		mWindow->pushGui(updateGui);
	}, "iconUpdates");
	
	// backup
	s->addEntry(_("BACKUP USER DATA"), true, [this] { mWindow->pushGui(new GuiBackupStart(mWindow)); });

#ifdef _ENABLE_KODI_
	s->addEntry(_("KODI SETTINGS"), true, [this] {
		GuiSettings *kodiGui = new GuiSettings(mWindow, _("KODI SETTINGS").c_str());
		auto kodiEnabled = std::make_shared<SwitchComponent>(mWindow);
		kodiEnabled->setState(SystemConf::getInstance()->get("kodi.enabled") == "1");
		kodiGui->addWithLabel(_("ENABLE KODI"), kodiEnabled);
		auto kodiAtStart = std::make_shared<SwitchComponent>(mWindow);
		kodiAtStart->setState(
			SystemConf::getInstance()->get("kodi.atstartup") == "1");
		kodiGui->addWithLabel(_("KODI AT START"), kodiAtStart);
		auto kodiX = std::make_shared<SwitchComponent>(mWindow);
		kodiX->setState(SystemConf::getInstance()->get("kodi.xbutton") == "1");
		kodiGui->addWithLabel(_("START KODI WITH X"), kodiX);
		kodiGui->addSaveFunc([kodiEnabled, kodiAtStart, kodiX] {
			SystemConf::getInstance()->set("kodi.enabled",
				kodiEnabled->getState() ? "1" : "0");
			SystemConf::getInstance()->set("kodi.atstartup",
				kodiAtStart->getState() ? "1" : "0");
			SystemConf::getInstance()->set("kodi.xbutton",
				kodiX->getState() ? "1" : "0");
			SystemConf::getInstance()->saveSystemConf();
		});
		mWindow->pushGui(kodiGui);
	});
#endif

	// Install
	s->addEntry(_("INSTALL BATOCERA ON A NEW DISK"), true, [this] { mWindow->pushGui(new GuiInstallStart(mWindow)); });

	// Security
	s->addEntry(_("SECURITY"), true, [this] {
		GuiSettings *securityGui = new GuiSettings(mWindow, _("SECURITY").c_str());
		auto securityEnabled = std::make_shared<SwitchComponent>(mWindow);
		securityEnabled->setState(SystemConf::getInstance()->get("system.security.enabled") == "1");
		securityGui->addWithLabel(_("ENFORCE SECURITY"), securityEnabled);

		auto rootpassword = std::make_shared<TextComponent>(mWindow,
			ApiSystem::getInstance()->getRootPassword(),
			ThemeData::getMenuTheme()->Text.font, ThemeData::getMenuTheme()->Text.color);
		securityGui->addWithLabel(_("ROOT PASSWORD"), rootpassword);

		securityGui->addSaveFunc([this, securityEnabled] {
			Window* window = this->mWindow;
			bool reboot = false;

			if (securityEnabled->changed()) {
				SystemConf::getInstance()->set("system.security.enabled",
					securityEnabled->getState() ? "1" : "0");
				SystemConf::getInstance()->saveSystemConf();
				reboot = true;
			}

			if (reboot)
				window->displayMessage(_("A REBOOT OF THE SYSTEM IS REQUIRED TO APPLY THE NEW CONFIGURATION"));			
		});
		mWindow->pushGui(securityGui);
	});

	s->addSaveFunc([overclock_choice, window, language_choice, language, optionsStorage, selectedStorage] {
		bool reboot = false;
		if (optionsStorage->changed()) {
			ApiSystem::getInstance()->setStorage(optionsStorage->getSelected());
			reboot = true;
		}

		if (overclock_choice->changed()) {
			Settings::getInstance()->setString("Overclock", overclock_choice->getSelected());
			ApiSystem::getInstance()->setOverclock(overclock_choice->getSelected());
			reboot = true;
		}
		if (language_choice->changed()) {
			SystemConf::getInstance()->set("system.language",
				language_choice->getSelected());
			SystemConf::getInstance()->saveSystemConf();
			reboot = true;
		}
		if (reboot) {
			window->displayMessage(_("A REBOOT OF THE SYSTEM IS REQUIRED TO APPLY THE NEW CONFIGURATION"));
		}

	});

	// Developer options
	if (isFullUI)
		s->addEntry(_("DEVELOPER"), true, [this] { openDeveloperSettings(); });

	mWindow->pushGui(s);
}

void GuiMenu::openGamesSettings_batocera() 
{
	Window* window = mWindow;

	auto s = new GuiSettings(mWindow, _("GAMES SETTINGS").c_str());
	if (SystemConf::getInstance()->get("system.es.menu") != "bartop") {

		// Screen ratio choice
		auto ratio_choice = createRatioOptionList(mWindow, "global");
		s->addWithLabel(_("GAME RATIO"), ratio_choice);
		s->addSaveFunc([ratio_choice] {
			if (ratio_choice->changed()) {
				SystemConf::getInstance()->set("global.ratio", ratio_choice->getSelected());
				SystemConf::getInstance()->saveSystemConf();
			}
		});
	}
	// smoothing
	auto smoothing_enabled = std::make_shared<OptionListComponent<std::string>>(mWindow, _("SMOOTH GAMES"));
	smoothing_enabled->add(_("AUTO"), "auto", SystemConf::getInstance()->get("global.smooth") != "0" && SystemConf::getInstance()->get("global.smooth") != "1");
	smoothing_enabled->add(_("ON"), "1", SystemConf::getInstance()->get("global.smooth") == "1");
	smoothing_enabled->add(_("OFF"), "0", SystemConf::getInstance()->get("global.smooth") == "0");
	s->addWithLabel(_("SMOOTH GAMES"), smoothing_enabled);

	// rewind
	auto rewind_enabled = std::make_shared<OptionListComponent<std::string>>(mWindow, _("REWIND"));
	rewind_enabled->add(_("AUTO"), "auto", SystemConf::getInstance()->get("global.rewind") != "0" && SystemConf::getInstance()->get("global.rewind") != "1");
	rewind_enabled->add(_("ON"), "1", SystemConf::getInstance()->get("global.rewind") == "1");
	rewind_enabled->add(_("OFF"), "0", SystemConf::getInstance()->get("global.rewind") == "0");
	s->addWithLabel(_("REWIND"), rewind_enabled);

	// autosave/load
	auto autosave_enabled = std::make_shared<OptionListComponent<std::string>>(mWindow, _("AUTO SAVE/LOAD"));
	autosave_enabled->add(_("AUTO"), "auto", SystemConf::getInstance()->get("global.autosave") != "0" && SystemConf::getInstance()->get("global.autosave") != "1");
	autosave_enabled->add(_("ON"), "1", SystemConf::getInstance()->get("global.autosave") == "1");
	autosave_enabled->add(_("OFF"), "0", SystemConf::getInstance()->get("global.autosave") == "0");
	s->addWithLabel(_("AUTO SAVE/LOAD"), autosave_enabled);

	// Shaders preset
	auto shaders_choices = std::make_shared<OptionListComponent<std::string> >(mWindow, _("SHADERS SET"),
		false);
	std::string currentShader = SystemConf::getInstance()->get("global.shaderset");
	if (currentShader.empty()) {
		currentShader = std::string("auto");
	}

	shaders_choices->add(_("AUTO"), "auto", currentShader == "auto");
	shaders_choices->add(_("NONE"), "none", currentShader == "none");
	shaders_choices->add(_("SCANLINES"), "scanlines", currentShader == "scanlines");
	shaders_choices->add(_("RETRO"), "retro", currentShader == "retro");
	shaders_choices->add(_("ENHANCED"), "enhanced", currentShader == "enhanced"); // batocera 5.23
	shaders_choices->add(_("CURVATURE"), "curvature", currentShader == "curvature"); // batocera 5.24
	s->addWithLabel(_("SHADERS SET"), shaders_choices);

	// Integer scale
	auto integerscale_enabled = std::make_shared<OptionListComponent<std::string>>(mWindow, _("INTEGER SCALE (PIXEL PERFECT)"));
	integerscale_enabled->add(_("AUTO"), "auto", SystemConf::getInstance()->get("global.integerscale") != "0" && SystemConf::getInstance()->get("global.integerscale") != "1");
	integerscale_enabled->add(_("ON"), "1", SystemConf::getInstance()->get("global.integerscale") == "1");
	integerscale_enabled->add(_("OFF"), "0", SystemConf::getInstance()->get("global.integerscale") == "0");
	s->addWithLabel(_("INTEGER SCALE (PIXEL PERFECT)"), integerscale_enabled);
	s->addSaveFunc([integerscale_enabled] {
		if (integerscale_enabled->changed()) {
			SystemConf::getInstance()->set("global.integerscale", integerscale_enabled->getSelected());
			SystemConf::getInstance()->saveSystemConf();
		}
	});

	// decorations
	{
		auto decorations = std::make_shared<OptionListComponent<std::string> >(mWindow, _("DECORATION"), false);
		std::vector<std::string> decorations_item;
		decorations_item.push_back(_("AUTO"));
		decorations_item.push_back(_("NONE"));

		std::vector<std::string> sets = GuiMenu::getDecorationsSets();
		for (auto it = sets.begin(); it != sets.end(); it++) {
			decorations_item.push_back(*it);
		}

		for (auto it = decorations_item.begin(); it != decorations_item.end(); it++) {
			decorations->add(*it, *it,
				(SystemConf::getInstance()->get("global.bezel") == *it)
				||
				(SystemConf::getInstance()->get("global.bezel") == "none" && *it == _("NONE"))
				||
				(SystemConf::getInstance()->get("global.bezel") == "" && *it == _("AUTO"))
			);
		}
		s->addWithLabel(_("DECORATION"), decorations);
		s->addSaveFunc([decorations] {
			if (decorations->changed()) {
				SystemConf::getInstance()->set("global.bezel", decorations->getSelected() == _("NONE") ? "none" : decorations->getSelected() == _("AUTO") ? "" : decorations->getSelected());
				SystemConf::getInstance()->saveSystemConf();
			}
		});
	}

	if (SystemConf::getInstance()->get("system.es.menu") != "bartop") 
	{
		// Retroachievements
		s->addEntry(_("RETROACHIEVEMENTS SETTINGS"), true, [this] 
		{
			GuiSettings *retroachievements = new GuiSettings(mWindow, _("RETROACHIEVEMENTS SETTINGS").c_str());

			// retroachievements_enable
			auto retroachievements_enabled = std::make_shared<SwitchComponent>(mWindow);
			retroachievements_enabled->setState(
				SystemConf::getInstance()->get("global.retroachievements") == "1");
			retroachievements->addWithLabel(_("RETROACHIEVEMENTS"), retroachievements_enabled);

			// retroachievements_hardcore_mode
			auto retroachievements_hardcore_enabled = std::make_shared<SwitchComponent>(mWindow);
			retroachievements_hardcore_enabled->setState(
				SystemConf::getInstance()->get("global.retroachievements.hardcore") == "1");
			retroachievements->addWithLabel(_("HARDCORE MODE"), retroachievements_hardcore_enabled);

			// retroachievements_leaderboards
			auto retroachievements_leaderboards_enabled = std::make_shared<SwitchComponent>(mWindow);
			retroachievements_leaderboards_enabled->setState(
				SystemConf::getInstance()->get("global.retroachievements.leaderboards") == "1");
			retroachievements->addWithLabel(_("LEADERBOARDS"), retroachievements_leaderboards_enabled);

			// retroachievements_verbose_mode
			auto retroachievements_verbose_enabled = std::make_shared<SwitchComponent>(mWindow);
			retroachievements_verbose_enabled->setState(
				SystemConf::getInstance()->get("global.retroachievements.verbose") == "1");
			retroachievements->addWithLabel(_("VERBOSE MODE"), retroachievements_verbose_enabled);

			// retroachievements_automatic_screenshot
			auto retroachievements_screenshot_enabled = std::make_shared<SwitchComponent>(mWindow);
			retroachievements_screenshot_enabled->setState(
				SystemConf::getInstance()->get("global.retroachievements.screenshot") == "1");
			retroachievements->addWithLabel(_("AUTOMATIC SCREENSHOT"), retroachievements_screenshot_enabled);

			// retroachievements, username, password
			createInputTextRow(retroachievements, _("USERNAME"), "global.retroachievements.username",
				false);
			createInputTextRow(retroachievements, _("PASSWORD"), "global.retroachievements.password",
				true);

			retroachievements->addSaveFunc([retroachievements_enabled, retroachievements_hardcore_enabled, retroachievements_leaderboards_enabled,
				retroachievements_verbose_enabled, retroachievements_screenshot_enabled] {
				SystemConf::getInstance()->set("global.retroachievements",
					retroachievements_enabled->getState() ? "1" : "0");
				SystemConf::getInstance()->set("global.retroachievements.hardcore",
					retroachievements_hardcore_enabled->getState() ? "1" : "0");
				SystemConf::getInstance()->set("global.retroachievements.leaderboards",
					retroachievements_leaderboards_enabled->getState() ? "1" : "0");
				SystemConf::getInstance()->set("global.retroachievements.verbose",
					retroachievements_verbose_enabled->getState() ? "1" : "0");
				SystemConf::getInstance()->set("global.retroachievements.screenshot",
					retroachievements_screenshot_enabled->getState() ? "1" : "0");
				SystemConf::getInstance()->saveSystemConf();
			});
			mWindow->pushGui(retroachievements);
		});		

		// Bios
		s->addEntry(_("MISSING BIOS"), true, [this, s]
		{
			GuiSettings *configuration = new GuiSettings(mWindow, _("MISSING BIOS").c_str());
			std::vector<BiosSystem> biosInformations = ApiSystem::getInstance()->getBiosInformations();

			if (biosInformations.size() == 0) {
				configuration->addEntry(_("NO MISSING BIOS"));
			}
			else 
			{
				for (auto systemBios = biosInformations.begin(); systemBios != biosInformations.end(); systemBios++) 
				{
					BiosSystem systemBiosData = (*systemBios);
					configuration->addEntry((*systemBios).name.c_str(), true, [this, systemBiosData]
					{
						GuiSettings* configurationInfo = new GuiSettings(mWindow, systemBiosData.name.c_str());
						for (auto biosFile = systemBiosData.bios.begin(); biosFile != systemBiosData.bios.end(); biosFile++)
						{
							auto theme = ThemeData::getMenuTheme();

							auto biosPath = std::make_shared<TextComponent>(mWindow, biosFile->path.c_str(),
								theme->Text.font,
								theme->TextSmall.color); // 0x000000FF -> Avoid black on themes with black backgrounds
							auto biosMd5 = std::make_shared<TextComponent>(mWindow, biosFile->md5.c_str(),
								theme->TextSmall.font,
								theme->TextSmall.color);
							auto biosStatus = std::make_shared<TextComponent>(mWindow, biosFile->status.c_str(),
								theme->TextSmall.font,
								theme->TextSmall.color);

							ComponentListRow biosFileRow;
							biosFileRow.addElement(biosPath, true);
							configurationInfo->addRow(biosFileRow);

							configurationInfo->addWithLabel("   " + _("MD5"), biosMd5);
							configurationInfo->addWithLabel("   " + _("STATUS"), biosStatus);
						}
						mWindow->pushGui(configurationInfo);
					});
				}
			}
			mWindow->pushGui(configuration);
		});		

		// Custom config for systems
		s->addEntry(_("ADVANCED"), true, [this, s, window] 
		{
			s->save();
			GuiSettings* configuration = new GuiSettings(window, _("ADVANCED").c_str());

			// For each activated system
			std::vector<SystemData *> systems = SystemData::sSystemVector;
			for (auto system = systems.begin(); system != systems.end(); system++) 
			{
				if ((*system)->isCollection())
					continue;
				
				SystemData *systemData = (*system);
				configuration->addEntry((*system)->getFullName(), true, [this, systemData, window] {
					popSystemConfigurationGui(window, systemData, "");
				});				
			}

			window->pushGui(configuration);
		});
		
		// Game List Update
		s->addEntry(_("UPDATE GAMES LISTS"), false, [this, window] 
		{
			window->pushGui(new GuiMsgBox(window, _("REALLY UPDATE GAMES LISTS ?"), _("YES"),
				[this, window] {
				ViewController::get()->goToStart();
				delete ViewController::get();
				ViewController::init(window);
				CollectionSystemManager::deinit();
				CollectionSystemManager::init(window);
				SystemData::loadConfig(window);
				window->endRenderLoadingScreen();
				GuiComponent *gui;
				while ((gui = window->peekGui()) != NULL) {
					window->removeGui(gui);
					delete gui;
				}
				ViewController::get()->reloadAll();				
				window->pushGui(ViewController::get());
			}, _("NO"), nullptr));
		});
	}

	s->addSaveFunc([smoothing_enabled, rewind_enabled, shaders_choices, autosave_enabled] 
	{
		if (smoothing_enabled->changed())
			SystemConf::getInstance()->set("global.smooth", smoothing_enabled->getSelected());
		if (rewind_enabled->changed())
			SystemConf::getInstance()->set("global.rewind", rewind_enabled->getSelected());
		if (shaders_choices->changed())
			SystemConf::getInstance()->set("global.shaderset", shaders_choices->getSelected());
		if (autosave_enabled->changed())
			SystemConf::getInstance()->set("global.autosave", autosave_enabled->getSelected());

		SystemConf::getInstance()->saveSystemConf();
	});

	mWindow->pushGui(s);
}

void GuiMenu::openControllersSettings_batocera()
{

	GuiSettings *s = new GuiSettings(mWindow, _("CONTROLLERS SETTINGS").c_str());

	Window *window = mWindow;

	// CONFIGURE A CONTROLLER
	s->addEntry(_("CONFIGURE A CONTROLLER"), false, [window, this, s]
	{
		window->pushGui(new GuiMsgBox(window,
			_("YOU ARE GOING TO CONFIGURE A CONTROLLER. IF YOU HAVE ONLY ONE JOYSTICK, "
				"CONFIGURE THE DIRECTIONS KEYS AND SKIP JOYSTICK CONFIG BY HOLDING A BUTTON. "
				"IF YOU DO NOT HAVE A SPECIAL KEY FOR HOTKEY, CHOOSE THE SELECT BUTTON. SKIP "
				"ALL BUTTONS YOU DO NOT HAVE BY HOLDING A KEY. BUTTONS NAMES ARE BASED ON THE "
				"SNES CONTROLLER."), _("OK"),
			[window, this, s] {
			window->pushGui(new GuiDetectDevice(window, false, [this, s] {
				s->setSave(false);
				delete s;
				this->openControllersSettings_batocera();
			}));
		}));
	});

	// PAIR A BLUETOOTH CONTROLLER
	std::function<void(void *)> showControllerResult = [window, this, s](void *success)
	{
		bool result = (bool)success;

		if (result) {
			window->pushGui(new GuiMsgBox(window, _("CONTROLLER PAIRED"), _("OK")));
		}
		else {
			window->pushGui(new GuiMsgBox(window, _("UNABLE TO PAIR CONTROLLER"), _("OK")));
		}
	};

	s->addEntry(_("PAIR A BLUETOOTH CONTROLLER"), false, [window, this, s, showControllerResult] {
		window->pushGui(new GuiLoading(window, [] {
			bool success = ApiSystem::getInstance()->scanNewBluetooth();
			return (void *)success;
		}, showControllerResult));
	});

	// FORGET BLUETOOTH CONTROLLERS
	s->addEntry(_("FORGET BLUETOOTH CONTROLLERS"), false, [window, this, s] {
		ApiSystem::getInstance()->forgetBluetoothControllers();
		window->pushGui(new GuiMsgBox(window,
			_("CONTROLLERS LINKS HAVE BEEN DELETED."), _("OK")));
	});

	ComponentListRow row;

	// Here we go; for each player
	std::list<int> alreadyTaken = std::list<int>();

	// clear the current loaded inputs
	clearLoadedInput();

	std::vector<std::shared_ptr<OptionListComponent<StrInputConfig *>>> options;
	char strbuf[256];

	for (int player = 0; player < MAX_PLAYERS; player++) {
		std::stringstream sstm;
		sstm << "INPUT P" << player + 1;
		std::string confName = sstm.str() + "NAME";
		std::string confGuid = sstm.str() + "GUID";
		snprintf(strbuf, 256, _("INPUT P%i").c_str(), player + 1);

		LOG(LogInfo) << player + 1 << " " << confName << " " << confGuid;
		auto inputOptionList = std::make_shared<OptionListComponent<StrInputConfig *> >(mWindow, strbuf, false);
		options.push_back(inputOptionList);

		// Checking if a setting has been saved, else setting to default
		std::string configuratedName = Settings::getInstance()->getString(confName);
		std::string configuratedGuid = Settings::getInstance()->getString(confGuid);
		bool found = false;
		// For each available and configured input
		for (auto iter = InputManager::getInstance()->getJoysticks().begin(); iter != InputManager::getInstance()->getJoysticks().end(); iter++) {
			InputConfig* config = InputManager::getInstance()->getInputConfigByDevice(iter->first);
			if (config != NULL && config->isConfigured()) {
				// create name
				std::stringstream dispNameSS;
				dispNameSS << "#" << config->getDeviceId() << " ";
				std::string deviceName = config->getDeviceName();
				if (deviceName.size() > 25) {
					dispNameSS << deviceName.substr(0, 16) << "..." <<
						deviceName.substr(deviceName.size() - 5, deviceName.size() - 1);
				}
				else {
					dispNameSS << deviceName;
				}

				std::string displayName = dispNameSS.str();


				bool foundFromConfig = configuratedName == config->getDeviceName() &&
					configuratedGuid == config->getDeviceGUIDString();
				int deviceID = config->getDeviceId();
				// Si la manette est configurée, qu'elle correspond a la configuration, et qu'elle n'est pas
				// deja selectionnée on l'ajoute en séléctionnée
				StrInputConfig* newInputConfig = new StrInputConfig(config->getDeviceName(), config->getDeviceGUIDString());
				mLoadedInput.push_back(newInputConfig);

				if (foundFromConfig
					&& std::find(alreadyTaken.begin(), alreadyTaken.end(), deviceID) == alreadyTaken.end()
					&& !found) {
					found = true;
					alreadyTaken.push_back(deviceID);
					LOG(LogWarning) << "adding entry for player" << player << " (selected): " <<
						config->getDeviceName() << "  " << config->getDeviceGUIDString();
					inputOptionList->add(displayName, newInputConfig, true);
				}
				else {
					LOG(LogWarning) << "adding entry for player" << player << " (not selected): " <<
						config->getDeviceName() << "  " << config->getDeviceGUIDString();
					inputOptionList->add(displayName, newInputConfig, false);
				}
			}
		}
		if (configuratedName.compare("") == 0 || !found) {
			LOG(LogWarning) << "adding default entry for player " << player << "(selected : true)";
			inputOptionList->add("default", NULL, true);
		}
		else {
			LOG(LogWarning) << "adding default entry for player" << player << "(selected : false)";
			inputOptionList->add("default", NULL, false);
		}

		// ADD default config

		// Populate controllers list
		s->addWithLabel(strbuf, inputOptionList);
	}
	s->addSaveFunc([this, options, window] {
		for (int player = 0; player < MAX_PLAYERS; player++) {
			std::stringstream sstm;
			sstm << "INPUT P" << player + 1;
			std::string confName = sstm.str() + "NAME";
			std::string confGuid = sstm.str() + "GUID";

			auto input_p1 = options.at(player);
			std::string selectedName = input_p1->getSelectedName();

			if (selectedName.compare("default") == 0) {
				Settings::getInstance()->setString(confName, "DEFAULT");
				Settings::getInstance()->setString(confGuid, "");
			}
			else {
				if (input_p1->changed()) {
					LOG(LogWarning) << "Found the selected controller ! : name in list  = " << selectedName;
					LOG(LogWarning) << "Found the selected controller ! : guid  = " << input_p1->getSelected()->deviceGUIDString;

					Settings::getInstance()->setString(confName, input_p1->getSelected()->deviceName);
					Settings::getInstance()->setString(confGuid, input_p1->getSelected()->deviceGUIDString);
				}
			}
		}

		Settings::getInstance()->saveFile();
		// this is dependant of this configuration, thus update it
		InputManager::getInstance()->computeLastKnownPlayersDeviceIndexes();
	});

	row.elements.clear();
	window->pushGui(s);
}

void GuiMenu::openUISettings() 
{
	auto s = new GuiSettings(mWindow, _("UI SETTINGS").c_str());

	//UI mode
	auto UImodeSelection = std::make_shared< OptionListComponent<std::string> >(mWindow, _("UI MODE"), false);
	std::vector<std::string> UImodes = UIModeController::getInstance()->getUIModes();
	for (auto it = UImodes.cbegin(); it != UImodes.cend(); it++)
		UImodeSelection->add(*it, *it, Settings::getInstance()->getString("UIMode") == *it);
	s->addWithLabel(_("UI MODE"), UImodeSelection);
	Window* window = mWindow;
	s->addSaveFunc([UImodeSelection, window]
	{
		std::string selectedMode = UImodeSelection->getSelected();
		if (selectedMode != "Full")
		{
			std::string msg = _("You are changing the UI to a restricted mode:\nThis will hide most menu-options to prevent changes to the system.\nTo unlock and return to the full UI, enter this code:") + "\n";
			msg += "\"" + UIModeController::getInstance()->getFormattedPassKeyStr() + "\"\n\n";
			msg += _("Do you want to proceed ?");
			window->pushGui(new GuiMsgBox(window, msg,
				_("YES"), [selectedMode] {
				LOG(LogDebug) << "Setting UI mode to " << selectedMode;
				Settings::getInstance()->setString("UIMode", selectedMode);
				Settings::getInstance()->saveFile();
			}, _("NO"), nullptr));
		}
	});

#ifndef WIN32
	// video device
	auto optionsVideo = std::make_shared<OptionListComponent<std::string> >(mWindow, _("VIDEO OUTPUT"), false);
	std::string currentDevice = SystemConf::getInstance()->get("global.videooutput");
	if (currentDevice.empty()) currentDevice = "auto";

	std::vector<std::string> availableVideo = ApiSystem::getInstance()->getAvailableVideoOutputDevices();

	bool vfound = false;
	for (auto it = availableVideo.begin(); it != availableVideo.end(); it++) {
		optionsVideo->add((*it), (*it), currentDevice == (*it));
		if (currentDevice == (*it)) {
			vfound = true;
		}
	}
	if (vfound == false) {
		optionsVideo->add(currentDevice, currentDevice, true);
	}
	s->addWithLabel(_("VIDEO OUTPUT"), optionsVideo);

	s->addSaveFunc([this, optionsVideo, currentDevice] {
		if (optionsVideo->changed()) {
			SystemConf::getInstance()->set("global.videooutput", optionsVideo->getSelected());
			SystemConf::getInstance()->saveSystemConf();
			mWindow->displayMessage(_("A REBOOT OF THE SYSTEM IS REQUIRED TO APPLY THE NEW CONFIGURATION"));
		}
	});
#endif

	// theme set
	auto theme = ThemeData::getMenuTheme();
	auto themeSets = ThemeData::getThemeSets();
	auto system = ViewController::get()->getState().getSystem();

	if (!themeSets.empty()) 
	{
		auto selectedSet = themeSets.find(Settings::getInstance()->getString("ThemeSet"));
		if (selectedSet == themeSets.end())
			selectedSet = themeSets.begin();

		auto theme_set = std::make_shared<OptionListComponent<std::string> >(mWindow, _("THEME SET"), false);
		for (auto it = themeSets.begin(); it != themeSets.end(); it++)
			theme_set->add(it->first, it->first, it == selectedSet);
		s->addWithLabel(_("THEME SET"), theme_set);

		auto pthis = this;

		s->addSaveFunc([s, theme_set, pthis, window]
		{
			std::string oldTheme = Settings::getInstance()->getString("ThemeSet");
			if (oldTheme != theme_set->getSelected())
			{
				Settings::getInstance()->setString("ThemeSet", theme_set->getSelected());

				auto themeSubSets = ThemeData::getThemeSubSets(theme_set->getSelected());
				auto themeColorSets = ThemeData::getSubSet(themeSubSets, "colorset");
				auto themeIconSets = ThemeData::getSubSet(themeSubSets, "iconset");
				auto themeMenus = ThemeData::getSubSet(themeSubSets, "menu");
				auto themeSystemviewSets = ThemeData::getSubSet(themeSubSets, "systemview");
				auto themeGamelistViewSets = ThemeData::getSubSet(themeSubSets, "gamelistview");
				auto themeRegions = ThemeData::getSubSet(themeSubSets, "region");

				// theme changed without setting options, forcing options to avoid crash/blank theme
				Settings::getInstance()->setString("ThemeRegionName", themeRegions.empty() ? "" : themeRegions[0]);
				Settings::getInstance()->setString("ThemeColorSet", themeColorSets.empty() ? "" : themeColorSets[0]);
				Settings::getInstance()->setString("ThemeIconSet", themeIconSets.empty() ? "" : themeIconSets[0]);
				Settings::getInstance()->setString("ThemeMenu", themeMenus.empty() ? "" : themeMenus[0]);
				Settings::getInstance()->setString("ThemeSystemView", themeSystemviewSets.empty() ? "" : themeSystemviewSets[0]);
				Settings::getInstance()->setString("ThemeGamelistView", themeGamelistViewSets.empty() ? "" : themeGamelistViewSets[0]);

				window->renderLoadingScreen(_("Loading..."));

				Scripting::fireEvent("theme-changed", theme_set->getSelected(), oldTheme);

				ViewController::get()->reloadAll(window);

				window->endRenderLoadingScreen();

				delete pthis; 
				window->pushGui(new GuiMenu(window));
			}
		});

		// Theme subsets
		if (system != NULL && system->getTheme()->hasSubsets())
		{
			// theme config
			std::function<void()> openGui = [this, theme_set, s, window] {
				auto themeconfig = new GuiSettings(mWindow, _("THEME CONFIGURATION").c_str());

				auto SelectedTheme = theme_set->getSelected();

				auto themeSubSets = ThemeData::getThemeSubSets(SelectedTheme);
				auto themeColorSets = ThemeData::getSubSet(themeSubSets, "colorset");
				auto themeIconSets = ThemeData::getSubSet(themeSubSets, "iconset");
				auto themeMenus = ThemeData::getSubSet(themeSubSets, "menu");
				auto themeSystemviewSets = ThemeData::getSubSet(themeSubSets, "systemview");
				auto themeGamelistViewSets = ThemeData::getSubSet(themeSubSets, "gamelistview");
				auto themeRegions = ThemeData::getSubSet(themeSubSets, "region");

				// colorset
				std::shared_ptr<OptionListComponent<std::string>> theme_colorset = nullptr;
				if (themeColorSets.size() > 0)
				{
					auto selectedColorSet = std::find(themeColorSets.cbegin(), themeColorSets.cend(), Settings::getInstance()->getString("ThemeColorSet"));
					if (selectedColorSet == themeColorSets.end())
						selectedColorSet = themeColorSets.begin();

					theme_colorset = std::make_shared<OptionListComponent<std::string> >(mWindow, _("THEME COLORSET"), false);

					for (auto it = themeColorSets.begin(); it != themeColorSets.end(); it++)
						theme_colorset->add(*it, *it, it == selectedColorSet);

					if (!themeColorSets.empty())
						themeconfig->addWithLabel(_("THEME COLORSET"), theme_colorset);
				}

				// iconset
				std::shared_ptr<OptionListComponent<std::string>> theme_iconset = nullptr;
				if (themeIconSets.size() > 0)
				{
					auto selectedIconSet = std::find(themeIconSets.cbegin(), themeIconSets.cend(), Settings::getInstance()->getString("ThemeIconSet"));
					if (selectedIconSet == themeIconSets.end())
						selectedIconSet = themeIconSets.begin();

					theme_iconset = std::make_shared<OptionListComponent<std::string> >(mWindow, _("THEME ICONSET"), false);

					for (auto it = themeIconSets.begin(); it != themeIconSets.end(); it++)
						theme_iconset->add(*it, *it, it == selectedIconSet);

					if (!themeIconSets.empty())
						themeconfig->addWithLabel(_("THEME ICONSET"), theme_iconset);
				}

				// menu
				std::shared_ptr<OptionListComponent<std::string>> theme_menu = nullptr;
				if (themeMenus.size() > 0)
				{
					auto selectedMenu = std::find(themeMenus.cbegin(), themeMenus.cend(), Settings::getInstance()->getString("ThemeMenu"));					
					if (selectedMenu == themeMenus.end())
						selectedMenu = themeMenus.begin();

					theme_menu = std::make_shared<OptionListComponent<std::string> >(mWindow, _("THEME MENU"), false);

					for (auto it = themeMenus.begin(); it != themeMenus.end(); it++)
						theme_menu->add(*it, *it, it == selectedMenu);

					if (!themeMenus.empty())
						themeconfig->addWithLabel(_("THEME MENU"), theme_menu);
				}

				// systemview
				std::shared_ptr<OptionListComponent<std::string>> theme_systemview = nullptr;
				if (themeSystemviewSets.size() > 0)
				{
					auto selectedSystemviewSet = std::find(themeSystemviewSets.cbegin(), themeSystemviewSets.cend(), Settings::getInstance()->getString("ThemeSystemView"));
					if (selectedSystemviewSet == themeSystemviewSets.end())
						selectedSystemviewSet = themeSystemviewSets.begin();

					theme_systemview = std::make_shared<OptionListComponent<std::string> >(mWindow, _("THEME SYSTEMVIEW"), false);

					for (auto it = themeSystemviewSets.begin(); it != themeSystemviewSets.end(); it++)
						theme_systemview->add(*it, *it, it == selectedSystemviewSet);

					if (!themeSystemviewSets.empty())
						themeconfig->addWithLabel(_("THEME SYSTEMVIEW"), theme_systemview);
				}

				// gamelistview
				std::shared_ptr<OptionListComponent<std::string>> theme_gamelistview = nullptr;
				if (themeGamelistViewSets.size() > 0)
				{
					auto selectedGamelistViewSet = std::find(themeGamelistViewSets.cbegin(), themeGamelistViewSets.cend(), Settings::getInstance()->getString("ThemeGamelistView"));
					if (selectedGamelistViewSet == themeGamelistViewSets.end())
						selectedGamelistViewSet = themeGamelistViewSets.begin();

					theme_gamelistview = std::make_shared<OptionListComponent<std::string> >(mWindow, _("THEME GAMELISTVIEW"), false);

					for (auto it = themeGamelistViewSets.begin(); it != themeGamelistViewSets.end(); it++)
						theme_gamelistview->add(*it, *it, it == selectedGamelistViewSet);

					if (!themeGamelistViewSets.empty())
						themeconfig->addWithLabel(_("THEME GAMELISTVIEW"), theme_gamelistview);
				}

				// themeregion
				std::shared_ptr<OptionListComponent<std::string>> theme_region = nullptr;
				if (themeRegions.size() > 0)
				{
					auto selectedRegion = std::find(themeRegions.cbegin(), themeRegions.cend(), Settings::getInstance()->getString("ThemeRegionName"));
					if (selectedRegion == themeRegions.end())
						selectedRegion = themeRegions.begin();

					theme_region = std::make_shared<OptionListComponent<std::string> >(mWindow, _("THEME REGION"), false);

					for (auto it = themeRegions.begin(); it != themeRegions.end(); it++)
						theme_region->add(*it, *it, it == selectedRegion);

					if (!themeRegions.empty())
						themeconfig->addWithLabel(_("THEME REGION"), theme_region);
				}

				// gamelist_style
				std::shared_ptr<OptionListComponent<std::string>> gamelist_style = nullptr;
				//if (theme_gamelistview == nullptr)
				{
					gamelist_style = std::make_shared< OptionListComponent<std::string> >(mWindow, _("GAMELIST VIEW STYLE"), false);

					std::vector<std::string> styles;
					styles.push_back("automatic");

					auto system = ViewController::get()->getState().getSystem();
					if (system != NULL)
					{
						auto mViews = system->getTheme()->getViewsOfTheme();
						for (auto it = mViews.cbegin(); it != mViews.cend(); ++it)
							styles.push_back(*it);
					}
					else
					{
						styles.push_back("basic");
						styles.push_back("detailed");
					}

					auto viewPreference = Settings::getInstance()->getString("GamelistViewStyle");
					if (!system->getTheme()->hasView(viewPreference))
						viewPreference = "automatic";

					for (auto it = styles.cbegin(); it != styles.cend(); it++)
					  gamelist_style->add(_((*it).c_str()), *it, viewPreference == *it);

					themeconfig->addWithLabel(_("GAMELIST VIEW STYLE"), gamelist_style);
				}

				themeconfig->addSaveFunc([this, s, theme_set, theme_colorset, theme_iconset, theme_menu, theme_systemview, theme_gamelistview, theme_region, gamelist_style] {
					bool needReload = false;
					if (theme_colorset != nullptr && Settings::getInstance()->getString("ThemeColorSet") != theme_colorset->getSelected() && !theme_colorset->getSelected().empty())
						needReload = true;
					if (theme_iconset != nullptr && Settings::getInstance()->getString("ThemeIconSet") != theme_iconset->getSelected() && !theme_iconset->getSelected().empty())
						needReload = true;
					if (theme_menu != nullptr && Settings::getInstance()->getString("ThemeMenu") != theme_menu->getSelected() && !theme_menu->getSelected().empty())
						needReload = true;
					if (theme_systemview != nullptr && Settings::getInstance()->getString("ThemeSystemView") != theme_systemview->getSelected() && !theme_systemview->getSelected().empty())
						needReload = true;
					if (theme_gamelistview != nullptr && Settings::getInstance()->getString("ThemeGamelistView") != theme_gamelistview->getSelected() && !theme_gamelistview->getSelected().empty())
						needReload = true;
					if (theme_region != nullptr && Settings::getInstance()->getString("ThemeRegionName") != theme_region->getSelected() && !theme_region->getSelected().empty())
						needReload = true;
					if (gamelist_style != nullptr && Settings::getInstance()->getString("GamelistViewStyle") != gamelist_style->getSelected() && !gamelist_style->getSelected().empty())
						needReload = true;

					if (needReload) {

						Settings::getInstance()->setString("ThemeSet", theme_set == nullptr ? "" : theme_set->getSelected());
						Settings::getInstance()->setString("ThemeColorSet", theme_colorset == nullptr ? "" : theme_colorset->getSelected());
						Settings::getInstance()->setString("ThemeIconSet", theme_iconset == nullptr ? "" : theme_iconset->getSelected());
						Settings::getInstance()->setString("ThemeMenu", theme_menu == nullptr ? "" : theme_menu->getSelected());
						Settings::getInstance()->setString("ThemeSystemView", theme_systemview == nullptr ? "" : theme_systemview->getSelected());
						Settings::getInstance()->setString("ThemeGamelistView", theme_gamelistview == nullptr ? "" : theme_gamelistview->getSelected());
						Settings::getInstance()->setString("ThemeRegionName", theme_region == nullptr ? "" : theme_region->getSelected());
						Settings::getInstance()->setString("GamelistViewStyle", gamelist_style == nullptr ? "" : gamelist_style->getSelected());

						mWindow->renderLoadingScreen(_("Loading..."));

						//reload theme
						std::string oldTheme = Settings::getInstance()->getString("ThemeSet");
						Scripting::fireEvent("theme-changed", theme_set->getSelected(), oldTheme);
						CollectionSystemManager::get()->updateSystemsList();
						ViewController::get()->goToStart();
						ViewController::get()->reloadAll(mWindow);

						mWindow->endRenderLoadingScreen();
					}
				});
				if (!themeRegions.empty() || !themeGamelistViewSets.empty() || !themeSystemviewSets.empty() || !themeIconSets.empty() || !themeMenus.empty() || !themeColorSets.empty())
				{
					mWindow->pushGui(themeconfig);
				}
				else
					mWindow->pushGui(new GuiMsgBox(window, _("THIS THEME HAS NO OPTION"), _("OK")));
			};

			s->addSubMenu(_("THEME CONFIGURATION"), openGui);
		}
	}

	// GameList view style
	if (system != NULL && !system->getTheme()->hasSubsets())
	{
		auto gamelist_style = std::make_shared< OptionListComponent<std::string> >(mWindow, _("GAMELIST VIEW STYLE"), false);
		std::vector<std::string> styles;
		styles.push_back("automatic");
		
		auto system = ViewController::get()->getState().getSystem();
		if (system != NULL)
		{
			auto mViews = system->getTheme()->getViewsOfTheme();
			for (auto it = mViews.cbegin(); it != mViews.cend(); ++it)
				styles.push_back(*it);
		}
		else
		{
			styles.push_back("basic");
			styles.push_back("detailed");
			styles.push_back("video");
			styles.push_back("grid");
		}

		for (auto it = styles.cbegin(); it != styles.cend(); it++)
			gamelist_style->add(*it, *it, Settings::getInstance()->getString("GamelistViewStyle") == *it);
		s->addWithLabel(_("GAMELIST VIEW STYLE"), gamelist_style);
		s->addSaveFunc([gamelist_style, window] {
			bool needReload = false;
			if (Settings::getInstance()->getString("GamelistViewStyle") != gamelist_style->getSelected())
				needReload = true;
			Settings::getInstance()->setString("GamelistViewStyle", gamelist_style->getSelected());
			if (needReload)
			{
				window->renderLoadingScreen(_("Loading..."));
				ViewController::get()->reloadAll(window);
				window->endRenderLoadingScreen();
			}
		});
	}

	// Optionally start in selected system
	auto systemfocus_list = std::make_shared< OptionListComponent<std::string> >(mWindow, _("START ON SYSTEM"), false);
	systemfocus_list->add("NONE", "", Settings::getInstance()->getString("StartupSystem") == "");
	for (auto it = SystemData::sSystemVector.cbegin(); it != SystemData::sSystemVector.cend(); it++)
	{
		if ("retropie" != (*it)->getName())
		{
			systemfocus_list->add((*it)->getName(), (*it)->getName(), Settings::getInstance()->getString("StartupSystem") == (*it)->getName());
		}
	}
	s->addWithLabel(_("START ON SYSTEM"), systemfocus_list);
	s->addSaveFunc([systemfocus_list] {
		Settings::getInstance()->setString("StartupSystem", systemfocus_list->getSelected());
	});

	// Open gamelist at start
	auto startOnGamelist = std::make_shared<SwitchComponent>(mWindow);
	startOnGamelist->setState(Settings::getInstance()->getBool("StartupOnGameList"));
	s->addWithLabel(_("START ON GAMELIST"), startOnGamelist);
	s->addSaveFunc([startOnGamelist] { Settings::getInstance()->setBool("StartupOnGameList", startOnGamelist->getState()); });

	// Batocera: select systems to hide
	s->addEntry(_("DISPLAY / HIDE SYSTEMS"), true, [this] { mWindow->pushGui(new GuiSystemsHide(mWindow)); });

	// transition style
	auto transition_style = std::make_shared<OptionListComponent<std::string> >(mWindow,
		_("TRANSITION STYLE"),
		false);
	std::vector<std::string> transitions;
	transitions.push_back("fade");
	transitions.push_back("slide");
	transitions.push_back("instant");
	for (auto it = transitions.begin(); it != transitions.end(); it++)
		transition_style->add(*it, *it, Settings::getInstance()->getString("TransitionStyle") == *it);
	s->addWithLabel(_("TRANSITION STYLE"), transition_style);
	s->addSaveFunc([transition_style] {
		if (transition_style->changed()) {
			Settings::getInstance()->setString("TransitionStyle", transition_style->getSelected());
		}
	});

	// screensaver time
	auto screensaver_time = std::make_shared<SliderComponent>(mWindow, 0.f, 30.f, 1.f, "m");
	screensaver_time->setValue(
		(float)(Settings::getInstance()->getInt("ScreenSaverTime") / (1000 * 60)));
	s->addWithLabel(_("SCREENSAVER AFTER"), screensaver_time);
	s->addSaveFunc([screensaver_time] {
		Settings::getInstance()->setInt("ScreenSaverTime",
			(int)round(screensaver_time->getValue()) * (1000 * 60));
	});

	// Batocera screensavers: added "random video" (aka "demo mode") and slideshow at the same time,
	// for those who don't scrape videos and stick with pictures
	auto screensaver_behavior = std::make_shared<OptionListComponent<std::string> >(mWindow,
		_("TRANSITION STYLE"), false);
	std::vector<std::string> screensavers;
	screensavers.push_back("dim");
	screensavers.push_back("black");
	screensavers.push_back("random video");
	screensavers.push_back("slideshow");
	for (auto it = screensavers.cbegin(); it != screensavers.cend(); it++)
		screensaver_behavior->add(*it, *it, Settings::getInstance()->getString("ScreenSaverBehavior") == *it);
	s->addWithLabel(_("SCREENSAVER BEHAVIOR"), screensaver_behavior);
	s->addSaveFunc([this, screensaver_behavior] {
		if (Settings::getInstance()->getString("ScreenSaverBehavior") != "random video"
			&& screensaver_behavior->getSelected() == "random video") {
			// if before it wasn't risky but now there's a risk of problems, show warning
			mWindow->pushGui(new GuiMsgBox(mWindow,
				_("THE \"RANDOM VIDEO\" SCREENSAVER SHOWS VIDEOS FROM YOUR GAMELIST.\nIF YOU DON'T HAVE VIDEOS, OR IF NONE OF THEM CAN BE PLAYED AFTER A FEW ATTEMPTS, IT WILL DEFAULT TO \"BLACK\".\nMORE OPTIONS IN THE \"UI SETTINGS\" -> \"RANDOM VIDEO SCREENSAVER SETTINGS\" MENU."),
				_("OK"), [] { return; }));
		}
		Settings::getInstance()->setString("ScreenSaverBehavior", screensaver_behavior->getSelected());
		PowerSaver::updateTimeouts();
	});

	// RANDOM VIDEO SCREENSAVER SETTINGS
	s->addEntry(_("RANDOM VIDEO SCREENSAVER SETTINGS"), true, std::bind(&GuiMenu::openVideoScreensaverOptions, this));

	// SLIDESHOW SCREENSAVER SETTINGS
	s->addEntry(_("SLIDESHOW SCREENSAVER SETTINGS"), true, std::bind(&GuiMenu::openSlideshowScreensaverOptions, this));

	// clock
	auto clock = std::make_shared<SwitchComponent>(mWindow);
	clock->setState(Settings::getInstance()->getBool("DrawClock"));
	s->addWithLabel(_("SHOW CLOCK"), clock);
	s->addSaveFunc(
		[clock] { Settings::getInstance()->setBool("DrawClock", clock->getState()); });

	// show help
	auto show_help = std::make_shared<SwitchComponent>(mWindow);
	show_help->setState(Settings::getInstance()->getBool("ShowHelpPrompts"));
	s->addWithLabel(_("ON-SCREEN HELP"), show_help);
	s->addSaveFunc(
		[show_help] {
		Settings::getInstance()->setBool("ShowHelpPrompts", show_help->getState());
	});

	// quick system select (left/right in game list view)
	auto quick_sys_select = std::make_shared<SwitchComponent>(mWindow);
	quick_sys_select->setState(Settings::getInstance()->getBool("QuickSystemSelect"));
	s->addWithLabel(_("QUICK SYSTEM SELECT"), quick_sys_select);
	s->addSaveFunc([quick_sys_select] {
		Settings::getInstance()->setBool("QuickSystemSelect", quick_sys_select->getState());
	});

	// Enable OSK (On-Screen-Keyboard)
	auto osk_enable = std::make_shared<SwitchComponent>(mWindow);
	osk_enable->setState(Settings::getInstance()->getBool("UseOSK"));
	s->addWithLabel(_("ON SCREEN KEYBOARD"), osk_enable);
	s->addSaveFunc([osk_enable] {
		Settings::getInstance()->setBool("UseOSK", osk_enable->getState()); });

	// carousel transition option
	auto move_carousel = std::make_shared<SwitchComponent>(mWindow);
	move_carousel->setState(Settings::getInstance()->getBool("MoveCarousel"));
	s->addWithLabel(_("CAROUSEL TRANSITIONS"), move_carousel);
	s->addSaveFunc([move_carousel] {
		if (move_carousel->getState()
			&& !Settings::getInstance()->getBool("MoveCarousel")
			&& PowerSaver::getMode() == PowerSaver::INSTANT)
		{
			Settings::getInstance()->setString("PowerSaverMode", "default");
			PowerSaver::init();
		}
		Settings::getInstance()->setBool("MoveCarousel", move_carousel->getState());
	});

	// enable filters (ForceDisableFilters)
	auto enable_filter = std::make_shared<SwitchComponent>(mWindow);
	enable_filter->setState(!Settings::getInstance()->getBool("ForceDisableFilters"));
	s->addWithLabel(_("ENABLE FILTERS"), enable_filter);
	s->addSaveFunc([enable_filter] {
		bool filter_is_enabled = !Settings::getInstance()->getBool("ForceDisableFilters");
		Settings::getInstance()->setBool("ForceDisableFilters", !enable_filter->getState());
		if (enable_filter->getState() != filter_is_enabled) ViewController::get()->ReloadAndGoToStart();
	});
	
	// overscan
	auto overscan_enabled = std::make_shared<SwitchComponent>(mWindow);
	overscan_enabled->setState(Settings::getInstance()->getBool("Overscan"));
	s->addWithLabel(_("OVERSCAN"), overscan_enabled);
	s->addSaveFunc([overscan_enabled] {
		if (Settings::getInstance()->getBool("Overscan") != overscan_enabled->getState()) {
			Settings::getInstance()->setBool("Overscan", overscan_enabled->getState());
			ApiSystem::getInstance()->setOverscan(overscan_enabled->getState());
		}
	});

	// gamelists
	auto save_gamelists = std::make_shared<SwitchComponent>(mWindow);
	save_gamelists->setState(Settings::getInstance()->getBool("SaveGamelistsOnExit"));
	s->addWithLabel(_("SAVE METADATA ON EXIT"), save_gamelists);
	s->addSaveFunc([save_gamelists] { Settings::getInstance()->setBool("SaveGamelistsOnExit", save_gamelists->getState()); });

	auto parse_gamelists = std::make_shared<SwitchComponent>(mWindow);
	parse_gamelists->setState(Settings::getInstance()->getBool("ParseGamelistOnly"));
	s->addWithLabel(_("PARSE GAMESLISTS ONLY"), parse_gamelists);
	s->addSaveFunc([parse_gamelists] { Settings::getInstance()->setBool("ParseGamelistOnly", parse_gamelists->getState()); });

	auto local_art = std::make_shared<SwitchComponent>(mWindow);
	local_art->setState(Settings::getInstance()->getBool("LocalArt"));
	s->addWithLabel(_("SEARCH FOR LOCAL ART"), local_art);
	s->addSaveFunc([local_art] { Settings::getInstance()->setBool("LocalArt", local_art->getState()); });

	// hidden files
	auto hidden_files = std::make_shared<SwitchComponent>(mWindow);
	hidden_files->setState(Settings::getInstance()->getBool("ShowHiddenFiles"));
	s->addWithLabel(_("SHOW HIDDEN FILES"), hidden_files);
	s->addSaveFunc([hidden_files] { Settings::getInstance()->setBool("ShowHiddenFiles", hidden_files->getState()); });
	
	// power saver
	auto power_saver = std::make_shared< OptionListComponent<std::string> >(mWindow, _("POWER SAVER MODES"), false);
	std::vector<std::string> modes;
	modes.push_back("disabled");
	modes.push_back("default");
	modes.push_back("enhanced");
	modes.push_back("instant");
	for (auto it = modes.cbegin(); it != modes.cend(); it++)
		power_saver->add(*it, *it, Settings::getInstance()->getString("PowerSaverMode") == *it);
	s->addWithLabel(_("POWER SAVER MODES"), power_saver);
	s->addSaveFunc([this, power_saver] {
		if (Settings::getInstance()->getString("PowerSaverMode") != "instant" && power_saver->getSelected() == "instant") {
			Settings::getInstance()->setString("TransitionStyle", "instant");
			Settings::getInstance()->setBool("MoveCarousel", false);
			Settings::getInstance()->setBool("EnableSounds", false);
		}
		Settings::getInstance()->setString("PowerSaverMode", power_saver->getSelected());
		PowerSaver::init();
	});

	mWindow->pushGui(s);
}

void GuiMenu::openSoundSettings() 
{
	auto s = new GuiSettings(mWindow, _("SOUND SETTINGS").c_str());

	// volume
	auto volume = std::make_shared<SliderComponent>(mWindow, 0.f, 100.f, 1.f, "%");
	volume->setValue((float)VolumeControl::getInstance()->getVolume());
	s->addWithLabel(_("SYSTEM VOLUME"), volume);
	s->addSaveFunc([volume] { VolumeControl::getInstance()->setVolume((int)Math::round(volume->getValue())); });

	// disable sounds
	auto music_enabled = std::make_shared<SwitchComponent>(mWindow);
	music_enabled->setState(!(SystemConf::getInstance()->get("audio.bgmusic") == "0"));
	s->addWithLabel(_("FRONTEND MUSIC"), music_enabled);
	s->addSaveFunc([music_enabled] {
		SystemConf::getInstance()->set("audio.bgmusic", music_enabled->getState() ? "1" : "0");
		if (music_enabled->getState())
			AudioManager::getInstance()->playRandomMusic();
		else
			AudioManager::getInstance()->stopMusic();
	});

	// batocera - display music titles
	auto display_titles = std::make_shared<SwitchComponent>(mWindow);
	display_titles->setState((SystemConf::getInstance()->get("audio.display_titles") == "1"));
	s->addWithLabel(_("DISPLAY SONG TITLES"), display_titles);
	s->addSaveFunc([display_titles] {
		SystemConf::getInstance()->set("audio.display_titles", display_titles->getState() ? "1" : "0");
	});

	// batocera - how long to display the song titles?
	auto titles_time = std::make_shared<SliderComponent>(mWindow, 2.f, 300.f, 2.f, "s");
	std::string currentTitlesTime = SystemConf::getInstance()->get("audio.display_titles_time");
	if (currentTitlesTime.empty())
		currentTitlesTime = std::string("10");
	// Check if the string we got has only digits, otherwise throw a default value
	bool has_only_digits = (currentTitlesTime.find_first_not_of("0123456789") == std::string::npos);
	if (!has_only_digits)
		currentTitlesTime = std::string("10");
	titles_time->setValue((float)std::stoi(currentTitlesTime));
	s->addWithLabel(_("HOW MANY SECONDS FOR SONG TITLES"), titles_time);
	s->addSaveFunc([titles_time] {
		SystemConf::getInstance()->set("audio.display_titles_time", std::to_string((int)round(titles_time->getValue())));
	});

	// batocera - music per system
	auto music_per_system = std::make_shared<SwitchComponent>(mWindow);
	music_per_system->setState(!(SystemConf::getInstance()->get("audio.persystem") == "0"));
	s->addWithLabel(_("ONLY PLAY SYSTEM-SPECIFIC MUSIC FOLDER"), music_per_system);
	s->addSaveFunc([music_per_system] {
		SystemConf::getInstance()->set("audio.persystem", music_per_system->getState() ? "1" : "0");
	});

	// disable sounds
	auto sounds_enabled = std::make_shared<SwitchComponent>(mWindow);
	sounds_enabled->setState(Settings::getInstance()->getBool("EnableSounds"));
	s->addWithLabel(_("ENABLE NAVIGATION SOUNDS"), sounds_enabled);
	s->addSaveFunc([sounds_enabled] {
	    if (sounds_enabled->getState()
		  && !Settings::getInstance()->getBool("EnableSounds")
		  && PowerSaver::getMode() == PowerSaver::INSTANT)
		{
		  Settings::getInstance()->setString("PowerSaverMode", "default");
		  PowerSaver::init();
		}
	    Settings::getInstance()->setBool("EnableSounds", sounds_enabled->getState());
	  });

	auto video_audio = std::make_shared<SwitchComponent>(mWindow);
	video_audio->setState(Settings::getInstance()->getBool("VideoAudio"));
	s->addWithLabel(_("ENABLE VIDEO AUDIO"), video_audio);
	s->addSaveFunc([video_audio] { Settings::getInstance()->setBool("VideoAudio", video_audio->getState()); });

#ifndef WIN32
	// audio device
	auto optionsAudio = std::make_shared<OptionListComponent<std::string> >(mWindow, _("OUTPUT DEVICE"), false);

	std::vector<std::string> availableAudio = ApiSystem::getInstance()->getAvailableAudioOutputDevices();
	std::string selectedAudio = ApiSystem::getInstance()->getCurrentAudioOutputDevice();
	if (selectedAudio.empty()) 
		selectedAudio = "auto";

	if (SystemConf::getInstance()->get("system.es.menu") != "bartop") 
	{
		bool vfound = false;
		for (auto it = availableAudio.begin(); it != availableAudio.end(); it++)
		{
			std::vector<std::string> tokens = Utils::String::split(*it, ' ');

			if (selectedAudio == (*it))
				vfound = true;

			if (tokens.size() >= 2) 
			{
				// concatenat the ending words
				std::string vname = "";
				for (unsigned int i = 1; i < tokens.size(); i++) 
				{
					if (i > 2) vname += " ";
					vname += tokens.at(i);
				}
				optionsAudio->add(vname, (*it), selectedAudio == (*it));
			}
			else
				optionsAudio->add((*it), (*it), selectedAudio == (*it));			
		}

		if (vfound == false)
			optionsAudio->add(selectedAudio, selectedAudio, true);

		s->addWithLabel(_("OUTPUT DEVICE"), optionsAudio);
	}

	s->addSaveFunc([this, optionsAudio, selectedAudio, volume] {
		bool v_need_reboot = false;

		VolumeControl::getInstance()->setVolume((int)round(volume->getValue()));
		SystemConf::getInstance()->set("audio.volume",
			std::to_string((int)round(volume->getValue())));

		if (optionsAudio->changed()) {
			SystemConf::getInstance()->set("audio.device", optionsAudio->getSelected());
			ApiSystem::getInstance()->setAudioOutputDevice(optionsAudio->getSelected());
			v_need_reboot = true;
		}
		SystemConf::getInstance()->saveSystemConf();
		if (v_need_reboot)
			mWindow->displayMessage(_("A REBOOT OF THE SYSTEM IS REQUIRED TO APPLY THE NEW CONFIGURATION"));
	});
#endif

	mWindow->pushGui(s);
}

void GuiMenu::openNetworkSettings_batocera()
{
	auto theme = ThemeData::getMenuTheme();
	std::shared_ptr<Font> font = theme->Text.font;
	unsigned int color = theme->Text.color;

	Window *window = mWindow;

	auto s = new GuiSettings(mWindow, _("NETWORK SETTINGS").c_str());
	auto status = std::make_shared<TextComponent>(mWindow,
		ApiSystem::getInstance()->ping() ? _("CONNECTED")
		: _("NOT CONNECTED"),
		font, color);
	s->addWithLabel(_("STATUS"), status);
	auto ip = std::make_shared<TextComponent>(mWindow, ApiSystem::getInstance()->getIpAdress(),
		font, color);
	s->addWithLabel(_("IP ADDRESS"), ip);
	// Hostname
	createInputTextRow(s, _("HOSTNAME"), "system.hostname", false);

	// Wifi enable
	auto enable_wifi = std::make_shared<SwitchComponent>(mWindow);
	bool baseEnabled = SystemConf::getInstance()->get("wifi.enabled") == "1";
	enable_wifi->setState(baseEnabled);
	s->addWithLabel(_("ENABLE WIFI"), enable_wifi);

	// window, title, settingstring,
	const std::string baseSSID = SystemConf::getInstance()->get("wifi.ssid");
	createInputTextRow(s, _("WIFI SSID"), "wifi.ssid", false);
	const std::string baseKEY = SystemConf::getInstance()->get("wifi.key");
	createInputTextRow(s, _("WIFI KEY"), "wifi.key", true);

	s->addSaveFunc([baseEnabled, baseSSID, baseKEY, enable_wifi, window] {
		bool wifienabled = enable_wifi->getState();
		SystemConf::getInstance()->set("wifi.enabled", wifienabled ? "1" : "0");
		std::string newSSID = SystemConf::getInstance()->get("wifi.ssid");
		std::string newKey = SystemConf::getInstance()->get("wifi.key");
		SystemConf::getInstance()->saveSystemConf();
		if (wifienabled) {
			if (baseSSID != newSSID
				|| baseKEY != newKey
				|| !baseEnabled) {
				if (ApiSystem::getInstance()->enableWifi(newSSID, newKey)) {
					window->pushGui(
						new GuiMsgBox(window, _("WIFI ENABLED"))
					);
				}
				else {
					window->pushGui(
						new GuiMsgBox(window, _("WIFI CONFIGURATION ERROR"))
					);
				}
			}
		}
		else if (baseEnabled) {
			ApiSystem::getInstance()->disableWifi();
		}
	});

	mWindow->pushGui(s);
}

void GuiMenu::openRetroAchievements_batocera()
{
	Window *window = mWindow;
	auto s = new GuiSettings(mWindow, _("RETROACHIEVEMENTS").c_str());
	ComponentListRow row;
	std::vector<std::string> retroRes = ApiSystem::getInstance()->getRetroAchievements();
	for (auto it = retroRes.begin(); it != retroRes.end(); it++) 
	{
		int itsize = (*it).length() + 1;
		char *tmp = new char[itsize];
		char *lcmsg = new char[itsize];
		char *longmsg = new char[itsize];
		std::strcpy(tmp, (*it).c_str());
		// Format of the string returned by batocera-retroachievements-info:
		// "Bust-A-Move (SNES)@0 of 27 achievements@0/400 points@Last played 2019-05-20 03:06:16"
		char *gamename = strtok(tmp, "@"); // or whatever is given before the first @
		char *achiev = strtok(NULL, "@");
		char *points = strtok(NULL, "@");
		char *lastplayed = strtok(NULL, "@");
		if (achiev)
			sprintf(lcmsg, "%s ~ %s", gamename, achiev); // beautiful 1-liner
		else
			sprintf(lcmsg, "%s", gamename);
		auto itstring = std::make_shared<TextComponent>(mWindow,
			lcmsg, ThemeData::getMenuTheme()->TextSmall.font, ThemeData::getMenuTheme()->TextSmall.color);
		if (points && lastplayed) {
			sprintf(longmsg, "%s\n%s\n%s\n%s\n", gamename, achiev, points, lastplayed);
			row.makeAcceptInputHandler([this, longmsg] {
				mWindow->pushGui(new GuiMsgBox(mWindow, longmsg, _("OK")));
			});
		}
		row.addElement(itstring, true);
		s->addRow(row);
		row.elements.clear();
	}
	mWindow->pushGui(s);
}

void GuiMenu::openScraperSettings_batocera() 
{
	auto s = new GuiSettings(mWindow, _("SCRAPE").c_str());

	s->addEntry(_("AUTOMATIC SCRAPER"), true, [this] {
		Window* window = mWindow;
		window->pushGui(new GuiMsgBox(window, _("REALLY SCRAPE?"), _("YES"),
			[window] {
			window->pushGui(new GuiAutoScrape(window));
		}, _("NO"), nullptr));
	});

	s->addEntry(_("MANUAL SCRAPER"), true, [this] { openScraperSettings(); });

	mWindow->pushGui(s);
}

void GuiMenu::openQuitMenu_batocera()
{
  GuiMenu::openQuitMenu_batocera_static(mWindow);
}

void GuiMenu::openQuitMenu_batocera_static(Window *window, bool forceWin32Menu)
{
#ifdef WIN32
	if (!forceWin32Menu)
	{
		Scripting::fireEvent("quit");
		quitES("");
		return;
	}
#endif

	auto s = new GuiSettings(window, _("QUIT").c_str());

	s->addEntry(_("RESTART SYSTEM"), false, [window] {
		window->pushGui(new GuiMsgBox(window, _("REALLY RESTART?"), _("YES"),
			[] {
			if (ApiSystem::getInstance()->reboot() != 0) {
				LOG(LogWarning) <<
					"Restart terminated with non-zero result!";
			}
		}, _("NO"), nullptr));
	}, "iconRestart");

	s->addEntry(_("SHUTDOWN SYSTEM"), false, [window] {
		window->pushGui(new GuiMsgBox(window, _("REALLY SHUTDOWN?"), _("YES"),
			[] {
			if (ApiSystem::getInstance()->shutdown() != 0) {
				LOG(LogWarning) <<
					"Shutdown terminated with non-zero result!";
			}
		}, _("NO"), nullptr));
	}, "iconShutdown");

	s->addEntry(_("FAST SHUTDOWN SYSTEM"), false, [window] {
		window->pushGui(new GuiMsgBox(window, _("REALLY SHUTDOWN WITHOUT SAVING METADATAS?"), _("YES"),
			[] {
			if (ApiSystem::getInstance()->fastShutdown() != 0) {
				LOG(LogWarning) <<
					"Shutdown terminated with non-zero result!";
			}
		}, _("NO"), nullptr));
	}, "iconFastShutdown");

#ifdef WIN32
	if (Settings::getInstance()->getBool("ShowExit"))
	{
		s->addEntry(_("QUIT EMULATIONSTATION"), false, [window] {
			window->pushGui(new GuiMsgBox(window, _("REALLY QUIT?"), _("YES"),
				[] {
				SDL_Event ev;
				ev.type = SDL_QUIT;
				SDL_PushEvent(&ev);
			}, _("NO"), nullptr));
		}, "iconQuit");
	}
#endif

	if (forceWin32Menu)
		s->getMenu().animateTo(Vector2f((Renderer::getScreenWidth() - s->getMenu().getSize().x()) / 2, (Renderer::getScreenHeight() - s->getMenu().getSize().y()) / 2));

	//Vector2f((Renderer::getScreenWidth() - s->getMenu().getSize().x()) / 2, Renderer::getScreenHeight() * 0.9),
	//Vector2f((Renderer::getScreenWidth() - s->getMenu().getSize().x()) / 2, Renderer::getScreenHeight() * 0.15f));

	window->pushGui(s);
}

void GuiMenu::createInputTextRow(GuiSettings *gui, std::string title, const char *settingsID, bool password)
{
	auto theme = ThemeData::getMenuTheme();
	std::shared_ptr<Font> font = theme->Text.font;
	unsigned int color = theme->Text.color;

	// LABEL
	Window *window = mWindow;
	ComponentListRow row;

	auto lbl = std::make_shared<TextComponent>(window, title, font, color);
	row.addElement(lbl, true); // label

	std::shared_ptr<GuiComponent> ed;

	ed = std::make_shared<TextComponent>(window, ((password &&
		SystemConf::getInstance()->get(settingsID) != "")
		? "*********" : SystemConf::getInstance()->get(
			settingsID)), font, color, ALIGN_RIGHT); // Font::get(FONT_SIZE_MEDIUM, FONT_PATH_LIGHT)
	row.addElement(ed, true);

	auto spacer = std::make_shared<GuiComponent>(mWindow);
	spacer->setSize(Renderer::getScreenWidth() * 0.005f, 0);
	row.addElement(spacer, false);

	auto bracket = std::make_shared<ImageComponent>(mWindow);
	bracket->setImage(theme->Icons.arrow);
	bracket->setResize(Vector2f(0, lbl->getFont()->getLetterHeight()));
	row.addElement(bracket, false);

	auto updateVal = [ed, settingsID, password](const std::string &newVal) {
		if (!password)
			ed->setValue(newVal);
		else {
			ed->setValue("*********");
		}
		SystemConf::getInstance()->set(settingsID, newVal);
	}; // ok callback (apply new value to ed)

	row.makeAcceptInputHandler([this, title, updateVal, settingsID] {
		if (Settings::getInstance()->getBool("UseOSK")) {
			mWindow->pushGui(
				new GuiTextEditPopupKeyboard(mWindow, title, SystemConf::getInstance()->get(settingsID),
					updateVal, false));
		}
		else {
			mWindow->pushGui(
				new GuiTextEditPopup(mWindow, title, SystemConf::getInstance()->get(settingsID),
					updateVal, false));
		}
	});
	gui->addRow(row);
}

void GuiMenu::popSystemConfigurationGui(Window* mWindow, SystemData *systemData, std::string previouslySelectedEmulator) {
  popSpecificConfigurationGui(mWindow, systemData->getFullName(), systemData->getName(), systemData, previouslySelectedEmulator);
}

void GuiMenu::popGameConfigurationGui(Window* mWindow, std::string romFilename, SystemData *systemData, std::string previouslySelectedEmulator) {
  popSpecificConfigurationGui(mWindow, romFilename, romFilename, systemData, previouslySelectedEmulator);
}

void GuiMenu::popSpecificConfigurationGui(Window* mWindow, std::string title, std::string configName, SystemData *systemData, std::string previouslySelectedEmulator) {
  // The system configuration
  GuiSettings *systemConfiguration = new GuiSettings(mWindow, title.c_str());
  //Emulator choice
  auto emu_choice = std::make_shared<OptionListComponent<std::string>>(mWindow, "emulator", false);
  bool selected = false;
  std::string selectedEmulator = "";

  for (auto it = systemData->getEmulators()->begin(); it != systemData->getEmulators()->end(); it++) {
    bool found;
    std::string curEmulatorName = it->first;
    if (previouslySelectedEmulator != "") {
      // We just changed the emulator
      found = previouslySelectedEmulator == curEmulatorName;
    } else {
      found = (SystemConf::getInstance()->get(configName + ".emulator") == curEmulatorName);
    }
    if (found) {
      selectedEmulator = curEmulatorName;
    }
    selected = selected || found;
    emu_choice->add(curEmulatorName, curEmulatorName, found);
  }
  emu_choice->add(_("AUTO"), "auto", !selected);
  emu_choice->setSelectedChangedCallback([mWindow, title, configName, systemConfiguration, systemData](std::string s) {
      popSpecificConfigurationGui(mWindow, title, configName, systemData, s);
      delete systemConfiguration;
    });
  systemConfiguration->addWithLabel(_("Emulator"), emu_choice);

  // Core choice
  auto core_choice = std::make_shared<OptionListComponent<std::string> >(mWindow, _("Core"), false);

  // search if one will be selected
  bool onefound = false;
  for (auto emulator = systemData->getEmulators()->begin();
       emulator != systemData->getEmulators()->end(); emulator++) {
    if (selectedEmulator == emulator->first) {
      for (auto core = emulator->second->begin(); core != emulator->second->end(); core++) {
	if((SystemConf::getInstance()->get(configName + ".core") == *core)) {
	  onefound = true;
	}
      }
    }
  }

  // add auto if emu_choice is auto
  if(emu_choice->getSelected() == "auto") { // allow auto only if emulator is auto
    core_choice->add(_("AUTO"), "auto", !onefound);
    onefound = true;
  }
    
  // list
  for (auto emulator = systemData->getEmulators()->begin();
       emulator != systemData->getEmulators()->end(); emulator++) {
    if (selectedEmulator == emulator->first) {
      for (auto core = emulator->second->begin(); core != emulator->second->end(); core++) {
	bool found = (SystemConf::getInstance()->get(configName + ".core") == *core);
	core_choice->add(*core, *core, found || !onefound); // select the first one if none is selected
	onefound = true;
      }
    }
  }
  systemConfiguration->addWithLabel(_("Core"), core_choice);


  // Screen ratio choice
  auto ratio_choice = createRatioOptionList(mWindow, title);
  systemConfiguration->addWithLabel(_("GAME RATIO"), ratio_choice);
  // video resolution mode
  auto videoResolutionMode_choice = createVideoResolutionModeOptionList(mWindow, title);
  systemConfiguration->addWithLabel(_("VIDEO MODE"), videoResolutionMode_choice);
  // smoothing
  auto smoothing_enabled = std::make_shared<OptionListComponent<std::string>>(mWindow, _("SMOOTH GAMES"));
  smoothing_enabled->add(_("AUTO"), "auto", SystemConf::getInstance()->get(configName + ".smooth") != "0" && SystemConf::getInstance()->get(configName + ".smooth") != "1");
  smoothing_enabled->add(_("ON"),   "1",    SystemConf::getInstance()->get(configName + ".smooth") == "1");
  smoothing_enabled->add(_("OFF"),  "0",    SystemConf::getInstance()->get(configName + ".smooth") == "0");
  systemConfiguration->addWithLabel(_("SMOOTH GAMES"), smoothing_enabled);
  // rewind
  auto rewind_enabled = std::make_shared<OptionListComponent<std::string>>(mWindow, _("REWIND"));
  rewind_enabled->add(_("AUTO"), "auto", SystemConf::getInstance()->get(configName + ".rewind") != "0" && SystemConf::getInstance()->get(configName + ".rewind") != "1");
  rewind_enabled->add(_("ON"),   "1",    SystemConf::getInstance()->get(configName + ".rewind") == "1");
  rewind_enabled->add(_("OFF"),  "0",    SystemConf::getInstance()->get(configName + ".rewind") == "0");
  systemConfiguration->addWithLabel(_("REWIND"), rewind_enabled);
  // autosave
  auto autosave_enabled = std::make_shared<OptionListComponent<std::string>>(mWindow, _("AUTO SAVE/LOAD"));
  autosave_enabled->add(_("AUTO"), "auto", SystemConf::getInstance()->get(configName + ".autosave") != "0" && SystemConf::getInstance()->get(configName + ".autosave") != "1");
  autosave_enabled->add(_("ON"),   "1",    SystemConf::getInstance()->get(configName + ".autosave") == "1");
  autosave_enabled->add(_("OFF"),  "0",    SystemConf::getInstance()->get(configName + ".autosave") == "0");
  systemConfiguration->addWithLabel(_("AUTO SAVE/LOAD"), autosave_enabled);

  // Shaders preset
  auto shaders_choices = std::make_shared<OptionListComponent<std::string> >(mWindow, _("SHADERS SET"),
									     false);
  std::string currentShader = SystemConf::getInstance()->get(configName + ".shaderset");
  if (currentShader.empty()) {
    currentShader = std::string("auto");
  }

  shaders_choices->add(_("AUTO"), "auto", currentShader == "auto");
  shaders_choices->add(_("NONE"), "none", currentShader == "none");
  shaders_choices->add(_("SCANLINES"), "scanlines", currentShader == "scanlines");
  shaders_choices->add(_("RETRO"), "retro", currentShader == "retro");
  shaders_choices->add(_("ENHANCED"), "enhanced", currentShader == "enhanced"); // batocera 5.23
  shaders_choices->add(_("CURVATURE"), "curvature", currentShader == "curvature"); // batocera 5.24
  systemConfiguration->addWithLabel(_("SHADERS SET"), shaders_choices);

  // Integer scale
  auto integerscale_enabled = std::make_shared<OptionListComponent<std::string>>(mWindow, _("INTEGER SCALE (PIXEL PERFECT)"));
  integerscale_enabled->add(_("AUTO"), "auto", SystemConf::getInstance()->get(configName + ".integerscale") != "0" && SystemConf::getInstance()->get(configName + ".integerscale") != "1");
  integerscale_enabled->add(_("ON"),   "1",    SystemConf::getInstance()->get(configName + ".integerscale") == "1");
  integerscale_enabled->add(_("OFF"),  "0",    SystemConf::getInstance()->get(configName + ".integerscale") == "0");
  systemConfiguration->addWithLabel(_("INTEGER SCALE (PIXEL PERFECT)"), integerscale_enabled);
  systemConfiguration->addSaveFunc([integerscale_enabled, configName] {
      if(integerscale_enabled->changed()) {
	SystemConf::getInstance()->set(configName + ".integerscale", integerscale_enabled->getSelected());
	SystemConf::getInstance()->saveSystemConf();
      }
    });

  // decorations
  {
    auto decorations = std::make_shared<OptionListComponent<std::string> >(mWindow, _("DECORATION"), false);
    std::vector<std::string> decorations_item;
    decorations_item.push_back(_("AUTO"));
    decorations_item.push_back(_("NONE"));

    std::vector<std::string> sets = GuiMenu::getDecorationsSets();
    for(auto it = sets.begin(); it != sets.end(); it++) {
      decorations_item.push_back(*it);
    }

    for (auto it = decorations_item.begin(); it != decorations_item.end(); it++) {
      decorations->add(*it, *it,
		       (SystemConf::getInstance()->get(configName + ".bezel") == *it)
		       ||
		       (SystemConf::getInstance()->get(configName + ".bezel") == "none" && *it == _("NONE"))
		       ||
		       (SystemConf::getInstance()->get(configName + ".bezel") == "" && *it == _("AUTO"))
		       );
    }
    systemConfiguration->addWithLabel(_("DECORATION"), decorations);
    systemConfiguration->addSaveFunc([decorations, configName] {
	if(decorations->changed()) {
	  SystemConf::getInstance()->set(configName + ".bezel", decorations->getSelected() == _("NONE") ? "none" : decorations->getSelected() == _("AUTO") ? "" : decorations->getSelected());
	  SystemConf::getInstance()->saveSystemConf();
	}
      });
  }

  // gameboy colorize
  auto colorizations_choices = std::make_shared<OptionListComponent<std::string> >(mWindow, _("COLORIZATION"), false);
  std::string currentColorization = SystemConf::getInstance()->get(configName + "-renderer.colorization");
  if (currentColorization.empty()) {
    currentColorization = std::string("auto");
  }
  colorizations_choices->add(_("AUTO"), "auto", currentColorization == "auto");
  colorizations_choices->add(_("NONE"), "none", currentColorization == "none");

  const char* all_gambate_gc_colors_modes[] = {"GB - DMG",
					       "GB - Light",
					       "GB - Pocket",
					       "GBC - Blue",
					       "GBC - Brown",
					       "GBC - Dark Blue",
					       "GBC - Dark Brown",
					       "GBC - Dark Green",
					       "GBC - Grayscale",
					       "GBC - Green",
					       "GBC - Inverted",
					       "GBC - Orange",
					       "GBC - Pastel Mix",
					       "GBC - Red",
					       "GBC - Yellow",
					       "SGB - 1A",
					       "SGB - 1B",
					       "SGB - 1C",
					       "SGB - 1D",
					       "SGB - 1E",
					       "SGB - 1F",
					       "SGB - 1G",
					       "SGB - 1H",
					       "SGB - 2A",
					       "SGB - 2B",
					       "SGB - 2C",
					       "SGB - 2D",
					       "SGB - 2E",
					       "SGB - 2F",
					       "SGB - 2G",
					       "SGB - 2H",
					       "SGB - 3A",
					       "SGB - 3B",
					       "SGB - 3C",
					       "SGB - 3D",
					       "SGB - 3E",
					       "SGB - 3F",
					       "SGB - 3G",
					       "SGB - 3H",
					       "SGB - 4A",
					       "SGB - 4B",
					       "SGB - 4C",
					       "SGB - 4D",
					       "SGB - 4E",
					       "SGB - 4F",
					       "SGB - 4G",
					       "SGB - 4H",
					       "Special 1",
					       "Special 2",
					       "Special 3" };
  int n_all_gambate_gc_colors_modes = 50;
  for(int i=0; i<n_all_gambate_gc_colors_modes; i++) {
    colorizations_choices->add(all_gambate_gc_colors_modes[i], all_gambate_gc_colors_modes[i], currentColorization == std::string(all_gambate_gc_colors_modes[i]));
  }
  if(systemData->getName() == "gb" || systemData->getName() == "gbc" || systemData->getName() == "gb2players" || systemData->getName() == "gbc2players") // only for gb, gbc and gb2players
    systemConfiguration->addWithLabel(_("COLORIZATION"), colorizations_choices);
  
  // ps2 full boot
  auto fullboot_enabled = std::make_shared<OptionListComponent<std::string>>(mWindow, _("FULL BOOT"));
  fullboot_enabled->add(_("AUTO"), "auto", SystemConf::getInstance()->get(configName + ".fullboot") != "0" && SystemConf::getInstance()->get(configName + ".fullboot") != "1");
  fullboot_enabled->add(_("ON"),   "1",    SystemConf::getInstance()->get(configName + ".fullboot") == "1");
  fullboot_enabled->add(_("OFF"),  "0",    SystemConf::getInstance()->get(configName + ".fullboot") == "0");
  if(systemData->getName() == "ps2") // only for ps2
    systemConfiguration->addWithLabel(_("FULL BOOT"), fullboot_enabled);

  // wii emulated wiimotes
  auto emulatedwiimotes_enabled = std::make_shared<OptionListComponent<std::string>>(mWindow, _("EMULATED WIIMOTES"));
  emulatedwiimotes_enabled->add(_("AUTO"), "auto", SystemConf::getInstance()->get(configName + ".emulatedwiimotes") != "0" && SystemConf::getInstance()->get(configName + ".emulatedwiimotes") != "1");
  emulatedwiimotes_enabled->add(_("ON"),   "1",    SystemConf::getInstance()->get(configName + ".emulatedwiimotes") == "1");
  emulatedwiimotes_enabled->add(_("OFF"),  "0",    SystemConf::getInstance()->get(configName + ".emulatedwiimotes") == "0");
  if(systemData->getName() == "wii") // only for wii
    systemConfiguration->addWithLabel(_("EMULATED WIIMOTES"), emulatedwiimotes_enabled);

  // psp internal resolution
  auto internalresolution = std::make_shared<OptionListComponent<std::string>>(mWindow, _("INTERNAL RESOLUTION"));
  internalresolution->add(_("AUTO"), "auto",
			  SystemConf::getInstance()->get(configName + ".internalresolution") != "1" &&
			  SystemConf::getInstance()->get(configName + ".internalresolution") != "2" &&
			  SystemConf::getInstance()->get(configName + ".internalresolution") != "4" &&
			  SystemConf::getInstance()->get(configName + ".internalresolution") != "8" &&
			  SystemConf::getInstance()->get(configName + ".internalresolution") != "10");
  internalresolution->add("1", "1",    SystemConf::getInstance()->get(configName + ".internalresolution") == "1");
  internalresolution->add("2", "2",    SystemConf::getInstance()->get(configName + ".internalresolution") == "2");
  internalresolution->add("4", "4",    SystemConf::getInstance()->get(configName + ".internalresolution") == "4");
  internalresolution->add("8", "8",    SystemConf::getInstance()->get(configName + ".internalresolution") == "8");
  internalresolution->add("10", "10",  SystemConf::getInstance()->get(configName + ".internalresolution") == "10");
  if(systemData->getName() == "psp") // only for psp
    systemConfiguration->addWithLabel(_("INTERNAL RESOLUTION"), internalresolution);

  systemConfiguration->addSaveFunc(
				   [configName, systemData, smoothing_enabled, rewind_enabled, ratio_choice, videoResolutionMode_choice, emu_choice, core_choice, autosave_enabled, shaders_choices, colorizations_choices, fullboot_enabled, emulatedwiimotes_enabled, internalresolution] {
				     if(ratio_choice->changed()){
				       SystemConf::getInstance()->set(configName + ".ratio", ratio_choice->getSelected());
				     }
				     if(videoResolutionMode_choice->changed()){
				       SystemConf::getInstance()->set(configName + ".videomode", videoResolutionMode_choice->getSelected());
				     }
				     if(rewind_enabled->changed()) {
				       SystemConf::getInstance()->set(configName + ".rewind", rewind_enabled->getSelected());
				     }
				     if(smoothing_enabled->changed()){
				       SystemConf::getInstance()->set(configName + ".smooth", smoothing_enabled->getSelected());
				     }
				     if(autosave_enabled->changed()) {
				       SystemConf::getInstance()->set(configName + ".autosave", autosave_enabled->getSelected());
				     }
				     if(shaders_choices->changed()) {
				       SystemConf::getInstance()->set(configName + ".shaderset", shaders_choices->getSelected());
				     }
				     if(colorizations_choices->changed()){
				       SystemConf::getInstance()->set(configName + "-renderer.colorization", colorizations_choices->getSelected());
				     }

				     if(fullboot_enabled->changed()){
				       SystemConf::getInstance()->set(configName + ".fullboot", fullboot_enabled->getSelected());
				     }
				     if(emulatedwiimotes_enabled->changed()){
				       SystemConf::getInstance()->set(configName + ".emulatedwiimotes", emulatedwiimotes_enabled->getSelected());
				     }
				     if(internalresolution->changed()){
				       SystemConf::getInstance()->set(configName + ".internalresolution", internalresolution->getSelected());
				     }

				     // the menu GuiMenu::popSystemConfigurationGui is a hack
				     // if you change any option except emulator and change the emulator, the value is lost
				     // this is especially bad for core while the changed() value is lost too.
				     // to avoid this issue, instead of reprogramming this part, i will force a save of the emulator and core
				     // at each GuiMenu::popSystemConfigurationGui call, including the ending saving one (when changed() values are bad)
				     SystemConf::getInstance()->set(configName + ".emulator", emu_choice->getSelected());
				     SystemConf::getInstance()->set(configName + ".core", core_choice->getSelected());

				     SystemConf::getInstance()->saveSystemConf();
				   });
  mWindow->pushGui(systemConfiguration);
}

std::shared_ptr<OptionListComponent<std::string>> GuiMenu::createRatioOptionList(Window *window,
                                                                                 std::string configname) {
  auto ratio_choice = std::make_shared<OptionListComponent<std::string> >(window, _("GAME RATIO"), false);
  std::string currentRatio = SystemConf::getInstance()->get(configname + ".ratio");
  if (currentRatio.empty()) {
    currentRatio = std::string("auto");
  }

  std::map<std::string, std::string> *ratioMap = LibretroRatio::getInstance()->getRatio();
  for (auto ratio = ratioMap->begin(); ratio != ratioMap->end(); ratio++) {
    ratio_choice->add(_(ratio->first.c_str()), ratio->second, currentRatio == ratio->second);
  }

  return ratio_choice;
}

std::shared_ptr<OptionListComponent<std::string>> GuiMenu::createVideoResolutionModeOptionList(Window *window, std::string configname) 
{
	auto videoResolutionMode_choice = std::make_shared<OptionListComponent<std::string> >(window, _("VIDEO MODE"), false);

	std::string currentVideoMode = SystemConf::getInstance()->get(configname + ".videomode");
	if (currentVideoMode.empty())
		currentVideoMode = std::string("auto");
	
	std::vector<std::string> videoResolutionModeMap = ApiSystem::getInstance()->getVideoModes();
	videoResolutionMode_choice->add(_("AUTO"), "auto", currentVideoMode == "auto");
	for (auto videoMode = videoResolutionModeMap.begin(); videoMode != videoResolutionModeMap.end(); videoMode++)
	{
		std::vector<std::string> tokens = Utils::String::split(*videoMode, ':');

		// concatenat the ending words
		std::string vname;
		for (unsigned int i = 1; i < tokens.size(); i++) 
		{
			if (i > 1) 
				vname += ":";

			vname += tokens.at(i);
		}

		videoResolutionMode_choice->add(vname, tokens.at(0), currentVideoMode == tokens.at(0));
	}

	return videoResolutionMode_choice;
}

void GuiMenu::clearLoadedInput() {
  for(int i=0; i<mLoadedInput.size(); i++) {
    delete mLoadedInput[i];
  }
  mLoadedInput.clear();
}

std::vector<std::string> GuiMenu::getDecorationsSets()
{
  std::vector<std::string> sets;

  static const size_t pathCount = 2;
  std::string paths[pathCount] = {
    "/storage/.config/emuelec/decorations",
    "/userdata/decorations"
  };
  Utils::FileSystem::stringList dirContent;
  std::string folder;

  for(size_t i = 0; i < pathCount; i++) {
    if(!Utils::FileSystem::isDirectory(paths[i])) continue;
    dirContent = Utils::FileSystem::getDirContent(paths[i]);
    for (Utils::FileSystem::stringList::const_iterator it = dirContent.cbegin(); it != dirContent.cend(); ++it) {
      if(Utils::FileSystem::isDirectory(*it)) {
	folder = *it;
	folder = folder.substr(paths[i].size()+1);
	sets.push_back(folder);
      }
    }
  }

  // sort and remove duplicates
  sort(sets.begin(), sets.end());
  sets.erase(unique(sets.begin(), sets.end()), sets.end());
  return sets;
}

GuiMenu::~GuiMenu() {
  clearLoadedInput();
}
