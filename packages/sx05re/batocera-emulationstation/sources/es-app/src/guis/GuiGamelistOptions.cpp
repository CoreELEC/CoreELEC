#include "GuiGamelistOptions.h"

#include "guis/GuiGamelistFilter.h"
#include "scrapers/Scraper.h"
#include "views/gamelist/IGameListView.h"
#include "views/UIModeController.h"
#include "views/ViewController.h"
#include "components/SwitchComponent.h"
#include "CollectionSystemManager.h"
#include "FileFilterIndex.h"
#include "FileSorts.h"
#include "GuiMetaDataEd.h"
#include "SystemData.h"
#include "LocaleES.h"
#include "guis/GuiMenu.h"
#include "guis/GuiMsgBox.h"

GuiGamelistOptions::GuiGamelistOptions(Window* window, SystemData* system) : GuiComponent(window),
	mSystem(system), mMenu(window, "OPTIONS"), fromPlaceholder(false), mFiltersChanged(false), mReloadAll(false)
{
	auto theme = ThemeData::getMenuTheme();

	addChild(&mMenu);

	// check it's not a placeholder folder - if it is, only show "Filter Options"
	FileData* file = getGamelist()->getCursor();
	fromPlaceholder = file->isPlaceHolder();
	ComponentListRow row;

	if (!fromPlaceholder) {
		// jump to letter
		row.elements.clear();

		// define supported character range
		// this range includes all numbers, capital letters, and most reasonable symbols
		char startChar = '!';
		char endChar = '_';

		char curChar = (char)toupper(getGamelist()->getCursor()->getName()[0]);
		if(curChar < startChar || curChar > endChar)
			curChar = startChar;

		mJumpToLetterList = std::make_shared<LetterList>(mWindow, _("JUMP TO..."), false); // batocera
		for (char c = startChar; c <= endChar; c++)
		{
			// check if c is a valid first letter in current list
			const std::vector<FileData*>& files = getGamelist()->getCursor()->getParent()->getChildrenListToDisplay();
			for (auto file : files)
			{
				char candidate = (char)toupper(file->getName()[0]);
				if (c == candidate)
				{
					mJumpToLetterList->add(std::string(1, c), c, c == curChar);
					break;
				}
			}
		}

		row.addElement(std::make_shared<TextComponent>(mWindow, _("JUMP TO..."), theme->Text.font, theme->Text.color), true); // batocera
		row.addElement(mJumpToLetterList, false);
		row.input_handler = [&](InputConfig* config, Input input) 
		{
		  if(config->isMappedTo(BUTTON_OK, input) && input.value)
			{
				jumpToLetter();
				return true;
			}
			else if(mJumpToLetterList->input(config, input))
			{
				return true;
			}
			return false;
		};
		mMenu.addRow(row);

		// sort list by
		unsigned int currentSortId = mSystem->getSortId();
		if (currentSortId > FileSorts::SortTypes.size())
			currentSortId = 0;

		mListSort = std::make_shared<SortList>(mWindow, _("SORT GAMES BY"), false);
		for(unsigned int i = 0; i < FileSorts::SortTypes.size(); i++)
		{
			const FolderData::SortType& sort = FileSorts::SortTypes.at(i);
			mListSort->add(sort.icon + _(Utils::String::toUpper(sort.description).c_str()), i, i == currentSortId); // TODO - actually make the sort type persistent
		}

		mMenu.addWithLabel(_("SORT GAMES BY"), mListSort); // batocera
	}

	// Show filtered menu
	if (!Settings::getInstance()->getBool("ForceDisableFilters"))
		mMenu.addEntry(_("FILTER GAMELIST"), true, std::bind(&GuiGamelistOptions::openGamelistFilter, this));

	// Show favorites first in gamelists
	auto favoritesFirstSwitch = std::make_shared<SwitchComponent>(mWindow);
	favoritesFirstSwitch->setState(Settings::getInstance()->getBool("FavoritesFirst"));
	mMenu.addWithLabel(_("SHOW FAVORITES ON TOP"), favoritesFirstSwitch);
	addSaveFunc([favoritesFirstSwitch, this]
	{
		if (Settings::getInstance()->setBool("FavoritesFirst", favoritesFirstSwitch->getState()))
			mReloadAll = true;
	});

	// hidden files
	auto hidden_files = std::make_shared<SwitchComponent>(mWindow);
	hidden_files->setState(Settings::getInstance()->getBool("ShowHiddenFiles"));
	mMenu.addWithLabel(_("SHOW HIDDEN FILES"), hidden_files);
	addSaveFunc([hidden_files, this]
	{
		if (Settings::getInstance()->setBool("ShowHiddenFiles", hidden_files->getState()))
			mReloadAll = true;
	});

	// Flat folders
	auto flatFolders = std::make_shared<SwitchComponent>(mWindow);
	flatFolders->setState(!Settings::getInstance()->getBool("FlatFolders"));
	mMenu.addWithLabel(_("SHOW FOLDERS"), flatFolders); // batocera
	addSaveFunc([flatFolders, this] 
	{ 
		if (Settings::getInstance()->setBool("FlatFolders", !flatFolders->getState()))
			mReloadAll = true;
	});



	std::map<std::string, CollectionSystemData> customCollections = CollectionSystemManager::get()->getCustomCollectionSystems();

	if(UIModeController::getInstance()->isUIModeFull() &&
		((customCollections.find(system->getName()) != customCollections.cend() && CollectionSystemManager::get()->getEditingCollection() != system->getName()) ||
		CollectionSystemManager::get()->getCustomCollectionsBundle()->getName() == system->getName()))
	{
		mMenu.addEntry(_("ADD/REMOVE GAMES TO THIS GAME COLLECTION"), false, std::bind(&GuiGamelistOptions::startEditMode, this));
	}

	if(UIModeController::getInstance()->isUIModeFull() && CollectionSystemManager::get()->isEditing())
		mMenu.addEntry(_("FINISH EDITING COLLECTION") + " : " + Utils::String::toUpper(CollectionSystemManager::get()->getEditingCollection()), true, std::bind(&GuiGamelistOptions::exitEditMode, this));
	
	if (UIModeController::getInstance()->isUIModeFull() && !fromPlaceholder && !(mSystem->isCollection() && file->getType() == FOLDER))
		mMenu.addEntry(_("EDIT THIS GAME'S METADATA"), true, std::bind(&GuiGamelistOptions::openMetaDataEd, this));

	// batocera
	if (UIModeController::getInstance()->isUIModeFull() && !(mSystem->isCollection() && file->getType() == FOLDER))
		mMenu.addEntry(_("ADVANCED GAME OPTIONS"), true, [this, file, system] { GuiMenu::popGameConfigurationGui(mWindow, Utils::FileSystem::getFileName(file->getPath()), file->getSourceFileData()->getSystem(), ""); });

	// Game List Update
	mMenu.addEntry(_("UPDATE GAMES LISTS"), false, [this, window]
	{
		window->pushGui(new GuiMsgBox(window, _("REALLY UPDATE GAMES LISTS ?"), _("YES"), [this, window]
		{
			std::string systemName = mSystem->getName();
			
			mSystem = nullptr;

			ViewController::get()->goToStart(true);

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

			if (!ViewController::get()->goToGameList(systemName, true))
				ViewController::get()->goToStart(true);			
			
		}, _("NO"), nullptr));
	});

	// center the menu
	setSize((float)Renderer::getScreenWidth(), (float)Renderer::getScreenHeight());
	mMenu.animateTo(Vector2f((Renderer::getScreenWidth() - mMenu.getSize().x()) / 2, (Renderer::getScreenHeight() - mMenu.getSize().y()) / 2));
}

GuiGamelistOptions::~GuiGamelistOptions()
{
	if (mSystem == nullptr)
		return;

	for (auto it = mSaveFuncs.cbegin(); it != mSaveFuncs.cend(); it++)
		(*it)();

	// apply sort
	if (!fromPlaceholder && mListSort->getSelected() != mSystem->getSortId())
	{
		mSystem->setSortId(mListSort->getSelected());

		FolderData* root = mSystem->getRootFolder();

		const FolderData::SortType& sort = FileSorts::SortTypes.at(mListSort->getSelected());
		root->sort(sort);

		// notify that the root folder was sorted
		getGamelist()->onFileChanged(root, FILE_SORTED);
	}

	if (mReloadAll)
	{
		mWindow->renderLoadingScreen(_("Loading..."));
		ViewController::get()->reloadAll(mWindow);
		mWindow->endRenderLoadingScreen();
	}
	else if (mFiltersChanged)
	{
		// only reload full view if we came from a placeholder
		// as we need to re-display the remaining elements for whatever new
		// game is selected
		ViewController::get()->reloadGameListView(mSystem);
	}

	Settings::getInstance()->saveFile();
}

void GuiGamelistOptions::openGamelistFilter()
{
	mReloadAll = false;
	mFiltersChanged = true;
	GuiGamelistFilter* ggf = new GuiGamelistFilter(mWindow, mSystem);
	mWindow->pushGui(ggf);
}

void GuiGamelistOptions::startEditMode()
{
	std::string editingSystem = mSystem->getName();
	// need to check if we're editing the collections bundle, as we will want to edit the selected collection within
	if(editingSystem == CollectionSystemManager::get()->getCustomCollectionsBundle()->getName())
	{
		FileData* file = getGamelist()->getCursor();
		// do we have the cursor on a specific collection?
		if (file->getType() == FOLDER)
		{
			editingSystem = file->getName();
		}
		else
		{
			// we are inside a specific collection. We want to edit that one.
			editingSystem = file->getSystem()->getName();
		}
	}
	CollectionSystemManager::get()->setEditMode(editingSystem);
	delete this;
}

void GuiGamelistOptions::exitEditMode()
{
	CollectionSystemManager::get()->exitEditMode();
	delete this;
}

void GuiGamelistOptions::openMetaDataEd()
{
	// open metadata editor
	// get the FileData that hosts the original metadata
	FileData* file = getGamelist()->getCursor()->getSourceFileData();
	ScraperSearchParams p;
	p.game = file;
	p.system = file->getSystem();

	std::function<void()> deleteBtnFunc;

	if (file->getType() == FOLDER)
	{
		deleteBtnFunc = NULL;
	}
	else
	{
		deleteBtnFunc = [this, file] {
			CollectionSystemManager::get()->deleteCollectionFiles(file);
			ViewController::get()->getGameListView(file->getSystem()).get()->remove(file, true);
		};
	}

	mWindow->pushGui(new GuiMetaDataEd(mWindow, &file->metadata, file->metadata.getMDD(), p, Utils::FileSystem::getFileName(file->getPath()),
		std::bind(&IGameListView::onFileChanged, ViewController::get()->getGameListView(file->getSystem()).get(), file, FILE_METADATA_CHANGED), deleteBtnFunc));
}

void GuiGamelistOptions::jumpToLetter()
{
	char letter = mJumpToLetterList->getSelected();
	IGameListView* gamelist = getGamelist();

	if (mListSort->getSelected() != 0)
	{
		mListSort->selectFirstItem();
		mSystem->setSortId(0);

		FolderData* root = mSystem->getRootFolder();
		const FolderData::SortType& sort = FileSorts::SortTypes.at(0);
		root->sort(sort);

		getGamelist()->onFileChanged(root, FILE_SORTED);
	}

	// this is a really shitty way to get a list of files
	const std::vector<FileData*>& files = gamelist->getCursor()->getParent()->getChildrenListToDisplay();

	long min = 0;
	long max = (long)files.size() - 1;
	long mid = 0;

	while(max >= min)
	{
		mid = ((max - min) / 2) + min;

		// game somehow has no first character to check
		if(files.at(mid)->getName().empty())
			continue;

		char checkLetter = (char)toupper(files.at(mid)->getName()[0]);

		if(checkLetter < letter)
			min = mid + 1;
		else if(checkLetter > letter || (mid > 0 && (letter == toupper(files.at(mid - 1)->getName()[0]))))
			max = mid - 1;
		else
			break; //exact match found
	}

	gamelist->setCursor(files.at(mid));

	delete this;
}

bool GuiGamelistOptions::input(InputConfig* config, Input input)
{
	if ((config->isMappedTo(BUTTON_BACK, input) || config->isMappedTo("select", input)) && input.value)
	{
		delete this;
		return true;
	}

	return mMenu.input(config, input);
}

HelpStyle GuiGamelistOptions::getHelpStyle()
{
	HelpStyle style = HelpStyle();
	style.applyTheme(mSystem->getTheme(), "system");
	return style;
}

std::vector<HelpPrompt> GuiGamelistOptions::getHelpPrompts()
{
	auto prompts = mMenu.getHelpPrompts();
	prompts.push_back(HelpPrompt(BUTTON_BACK, _("CLOSE")));
	return prompts;
}

IGameListView* GuiGamelistOptions::getGamelist()
{
	return ViewController::get()->getGameListView(mSystem).get();
}
