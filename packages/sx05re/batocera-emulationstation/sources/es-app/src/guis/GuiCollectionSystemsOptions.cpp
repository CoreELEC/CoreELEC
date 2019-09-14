#include "guis/GuiCollectionSystemsOptions.h"

#include "components/OptionListComponent.h"
#include "components/SwitchComponent.h"
#include "guis/GuiSettings.h"
#include "guis/GuiTextEditPopupKeyboard.h"
#include "guis/GuiTextEditPopup.h"
#include "utils/StringUtil.h"
#include "views/ViewController.h"
#include "CollectionSystemManager.h"
#include "Window.h"

GuiCollectionSystemsOptions::GuiCollectionSystemsOptions(Window* window) : GuiComponent(window), mMenu(window, _("GAME COLLECTION SETTINGS").c_str())
{
	initializeMenu();
}

void GuiCollectionSystemsOptions::initializeMenu()
{
	addChild(&mMenu);

	// get collections

	addSystemsToMenu();

	// add "Create New Custom Collection from Theme"

	std::vector<std::string> unusedFolders = CollectionSystemManager::get()->getUnusedSystemsFromTheme();
	if (unusedFolders.size() > 0)
	{
	  addEntry(_("CREATE NEW CUSTOM COLLECTION FROM THEME").c_str(), true,
		[this, unusedFolders] {
		     auto s = new GuiSettings(mWindow, _("SELECT THEME FOLDER").c_str());
		     std::shared_ptr< OptionListComponent<std::string> > folderThemes = std::make_shared< OptionListComponent<std::string> >(mWindow, _("SELECT THEME FOLDER"), true);

			// add Custom Systems
			for(auto it = unusedFolders.cbegin() ; it != unusedFolders.cend() ; it++ )
			{
				ComponentListRow row;
				std::string name = *it;

				std::function<void()> createCollectionCall = [name, this, s] {
					createCollection(name);
				};
				row.makeAcceptInputHandler(createCollectionCall);

				auto themeFolder = std::make_shared<TextComponent>(mWindow, Utils::String::toUpper(name), ThemeData::getMenuTheme()->TextSmall.font, ThemeData::getMenuTheme()->Text.color);
				row.addElement(themeFolder, true);
				s->addRow(row);				
			}
			mWindow->pushGui(s);
		});
	}

	auto createCustomCollection = [this](const std::string& newVal) {
		std::string name = newVal;
		// we need to store the first Gui and remove it, as it'll be deleted by the actual Gui
		Window* window = mWindow;
		GuiComponent* topGui = window->peekGui();
		window->removeGui(topGui);
		createCollection(name);
	};
	addEntry(_("CREATE NEW CUSTOM COLLECTION").c_str(), true, [this, createCustomCollection] {
		if (Settings::getInstance()->getBool("UseOSK")) {
			mWindow->pushGui(new GuiTextEditPopupKeyboard(mWindow, _("New Collection Name"), "", createCustomCollection, false));
		}
		else {
			mWindow->pushGui(new GuiTextEditPopup(mWindow, _("New Collection Name"), "", createCustomCollection, false));
		}
	});

	bundleCustomCollections = std::make_shared<SwitchComponent>(mWindow);
	bundleCustomCollections->setState(Settings::getInstance()->getBool("UseCustomCollectionsSystem"));
	mMenu.addWithLabel(_("GROUP UNTHEMED CUSTOM COLLECTIONS"), bundleCustomCollections);

	sortAllSystemsSwitch = std::make_shared<SwitchComponent>(mWindow);
	sortAllSystemsSwitch->setState(Settings::getInstance()->getBool("SortAllSystems"));
	mMenu.addWithLabel(_("SORT CUSTOM COLLECTIONS AND SYSTEMS"), sortAllSystemsSwitch);

	toggleSystemNameInCollections = std::make_shared<SwitchComponent>(mWindow);
	toggleSystemNameInCollections->setState(Settings::getInstance()->getBool("CollectionShowSystemInfo"));
	mMenu.addWithLabel(_("SHOW SYSTEM NAME IN COLLECTIONS"), toggleSystemNameInCollections);

	if(CollectionSystemManager::get()->isEditing())
		addEntry((_("FINISH EDITING COLLECTION") + " : " + Utils::String::toUpper(CollectionSystemManager::get()->getEditingCollection())).c_str(), false, std::bind(&GuiCollectionSystemsOptions::exitEditMode, this));

	mMenu.addButton(_("BACK"), "back", std::bind(&GuiCollectionSystemsOptions::applySettings, this));

	if (Renderer::isSmallScreen())
		mMenu.setPosition((Renderer::getScreenWidth() - mMenu.getSize().x()) / 2, (Renderer::getScreenHeight() - mMenu.getSize().y()) / 2);
	else
		mMenu.setPosition((Renderer::getScreenWidth() - mMenu.getSize().x()) / 2, Renderer::getScreenHeight() * 0.15f);
}

void GuiCollectionSystemsOptions::addEntry(const char* name, bool add_arrow, const std::function<void()>& func)
{
	auto theme = ThemeData::getMenuTheme();
	std::shared_ptr<Font> font = theme->Text.font;
	unsigned int color = theme->Text.color;

	// populate the list
	ComponentListRow row;
	row.addElement(std::make_shared<TextComponent>(mWindow, name, font, color), true);

	if(add_arrow)
	{
		std::shared_ptr<ImageComponent> bracket = makeArrow(mWindow);
		row.addElement(bracket, false);
	}

	row.makeAcceptInputHandler(func);

	mMenu.addRow(row);
}

void GuiCollectionSystemsOptions::createCollection(std::string inName) {
	std::string name = CollectionSystemManager::get()->getValidNewCollectionName(inName);
	SystemData* newSys = CollectionSystemManager::get()->addNewCustomCollection(name);
	customOptionList->add(name, name, true);
	std::string outAuto = Utils::String::vectorToCommaString(autoOptionList->getSelectedObjects());
	std::string outCustom = Utils::String::vectorToCommaString(customOptionList->getSelectedObjects());
	updateSettings(outAuto, outCustom);
	ViewController::get()->goToSystemView(newSys);

	Window* window = mWindow;
	CollectionSystemManager::get()->setEditMode(name);
	while(window->peekGui() && window->peekGui() != ViewController::get())
		delete window->peekGui();
	return;
}

void GuiCollectionSystemsOptions::exitEditMode()
{
	CollectionSystemManager::get()->exitEditMode();
	applySettings();
}

GuiCollectionSystemsOptions::~GuiCollectionSystemsOptions()
{

}

void GuiCollectionSystemsOptions::addSystemsToMenu()
{

	std::map<std::string, CollectionSystemData> autoSystems = CollectionSystemManager::get()->getAutoCollectionSystems();

	autoOptionList = std::make_shared< OptionListComponent<std::string> >(mWindow, _("SELECT COLLECTIONS"), true);

	// add Auto Systems
	for(std::map<std::string, CollectionSystemData>::const_iterator it = autoSystems.cbegin() ; it != autoSystems.cend() ; it++ )
	{
		autoOptionList->add(it->second.decl.longName, it->second.decl.name, it->second.isEnabled);
	}
	mMenu.addWithLabel(_("AUTOMATIC GAME COLLECTIONS"), autoOptionList);

	std::map<std::string, CollectionSystemData> customSystems = CollectionSystemManager::get()->getCustomCollectionSystems();

	customOptionList = std::make_shared< OptionListComponent<std::string> >(mWindow, _("SELECT COLLECTIONS"), true);

	// add Custom Systems
	for(std::map<std::string, CollectionSystemData>::const_iterator it = customSystems.cbegin() ; it != customSystems.cend() ; it++ )
	{
		customOptionList->add(it->second.decl.longName, it->second.decl.name, it->second.isEnabled);
	}
	mMenu.addWithLabel(_("CUSTOM GAME COLLECTIONS"), customOptionList);
}

void GuiCollectionSystemsOptions::applySettings()
{
	std::string outAuto = Utils::String::vectorToCommaString(autoOptionList->getSelectedObjects());
	std::string prevAuto = Settings::getInstance()->getString("CollectionSystemsAuto");
	std::string outCustom = Utils::String::vectorToCommaString(customOptionList->getSelectedObjects());
	std::string prevCustom = Settings::getInstance()->getString("CollectionSystemsCustom");
	bool outSort = sortAllSystemsSwitch->getState();
	bool prevSort = Settings::getInstance()->getBool("SortAllSystems");
	bool outBundle = bundleCustomCollections->getState();
	bool prevBundle = Settings::getInstance()->getBool("UseCustomCollectionsSystem");

	bool needUpdateSettings = prevAuto != outAuto || prevCustom != outCustom || outSort != prevSort || outBundle != prevBundle;
	if (needUpdateSettings)
	{
		updateSettings(outAuto, outCustom);
	}
	delete this;
}

void GuiCollectionSystemsOptions::updateSettings(std::string newAutoSettings, std::string newCustomSettings)
{
	Settings::getInstance()->setString("CollectionSystemsAuto", newAutoSettings);
	Settings::getInstance()->setString("CollectionSystemsCustom", newCustomSettings);
	Settings::getInstance()->setBool("SortAllSystems", sortAllSystemsSwitch->getState());
	Settings::getInstance()->setBool("UseCustomCollectionsSystem", bundleCustomCollections->getState());
	Settings::getInstance()->setBool("CollectionShowSystemInfo", toggleSystemNameInCollections->getState());	
	Settings::getInstance()->saveFile();
	CollectionSystemManager::get()->loadEnabledListFromSettings();
	CollectionSystemManager::get()->updateSystemsList();
	ViewController::get()->goToStart();
	ViewController::get()->reloadAll();
}

bool GuiCollectionSystemsOptions::input(InputConfig* config, Input input)
{
	bool consumed = GuiComponent::input(config, input);
	if(consumed)
		return true;

	if(config->isMappedTo(BUTTON_BACK, input) && input.value != 0)
		applySettings();

	return false;
}

std::vector<HelpPrompt> GuiCollectionSystemsOptions::getHelpPrompts()
{
	std::vector<HelpPrompt> prompts = mMenu.getHelpPrompts();
	prompts.push_back(HelpPrompt(BUTTON_BACK, _("BACK")));
	return prompts;
}
