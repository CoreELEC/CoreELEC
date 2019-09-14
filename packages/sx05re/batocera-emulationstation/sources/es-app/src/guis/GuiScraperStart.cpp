#include "guis/GuiScraperStart.h"

#include "components/OptionListComponent.h"
#include "components/SwitchComponent.h"
#include "guis/GuiMsgBox.h"
#include "guis/GuiScraperMulti.h"
#include "views/ViewController.h"
#include "FileData.h"
#include "SystemData.h"
#include "LocaleES.h"

GuiScraperStart::GuiScraperStart(Window* window) : GuiComponent(window),
  mMenu(window, _("SCRAPE NOW").c_str()) // batocera
{
	addChild(&mMenu);

	// add filters (with first one selected)
	mFilters = std::make_shared< OptionListComponent<GameFilterFunc> >(mWindow, _("SCRAPE THESE GAMES"), false); // batocera
	mFilters->add(_("All Games"), // batocera
		[](SystemData*, FileData*) -> bool { return true; }, false);
	mFilters->add(_("Only missing image"), 
		[](SystemData*, FileData* g) -> bool { return g->metadata.get("image").empty(); }, true);
	mMenu.addWithLabel(_("FILTER"), mFilters); // batocera

	//add systems (all with a platformid specified selected)
	mSystems = std::make_shared< OptionListComponent<SystemData*> >(mWindow, _("SCRAPE THESE SYSTEMS"), true); // batocera
	for(auto it = SystemData::sSystemVector.cbegin(); it != SystemData::sSystemVector.cend(); it++)
	{
		if(!(*it)->hasPlatformId(PlatformIds::PLATFORM_IGNORE))
			mSystems->add((*it)->getFullName(), *it, !(*it)->getPlatformIds().empty());
	}
	mMenu.addWithLabel(_("SYSTEMS"), mSystems); // batocera

	mApproveResults = std::make_shared<SwitchComponent>(mWindow);
	mApproveResults->setState(true);
	mMenu.addWithLabel(_("USER DECIDES ON CONFLICTS"), mApproveResults); // batocera

	mMenu.addButton(_("START"), "start", std::bind(&GuiScraperStart::pressedStart, this)); // batocera
	mMenu.addButton(_("BACK"), "back", [&] { delete this; }); // batocera

	if (Renderer::isSmallScreen())
		mMenu.setPosition((Renderer::getScreenWidth() - mMenu.getSize().x()) / 2, (Renderer::getScreenHeight() - mMenu.getSize().y()) / 2);
	else
		mMenu.setPosition((Renderer::getScreenWidth() - mMenu.getSize().x()) / 2, Renderer::getScreenHeight() * 0.15f);
}

void GuiScraperStart::pressedStart()
{
	std::vector<SystemData*> sys = mSystems->getSelectedObjects();
	for(auto it = sys.cbegin(); it != sys.cend(); it++)
	{
		if((*it)->getPlatformIds().empty())
		{
			mWindow->pushGui(new GuiMsgBox(mWindow, 
				_("WARNING: SOME OF YOUR SELECTED SYSTEMS DO NOT HAVE A PLATFORM SET. RESULTS MAY BE EVEN MORE INACCURATE THAN USUAL!\nCONTINUE ANYWAY?"),  // batocera
						       _("YES"), std::bind(&GuiScraperStart::start, this),  // batocera
						       _("NO"), nullptr)); // batocera
			return;
		}
	}

	start();
}

void GuiScraperStart::start()
{
	std::queue<ScraperSearchParams> searches = getSearches(mSystems->getSelectedObjects(), mFilters->getSelected());

	if(searches.empty())
	{
		mWindow->pushGui(new GuiMsgBox(mWindow,
					       _("NO GAMES FIT THAT CRITERIA."))); // batocera
	}else{
		GuiScraperMulti* gsm = new GuiScraperMulti(mWindow, searches, mApproveResults->getState());
		mWindow->pushGui(gsm);
		delete this;
	}
}

std::queue<ScraperSearchParams> GuiScraperStart::getSearches(std::vector<SystemData*> systems, GameFilterFunc selector)
{
	std::queue<ScraperSearchParams> queue;
	for(auto sys = systems.cbegin(); sys != systems.cend(); sys++)
	{
		std::vector<FileData*> games = (*sys)->getRootFolder()->getFilesRecursive(GAME);
		for(auto game = games.cbegin(); game != games.cend(); game++)
		{
			if(selector((*sys), (*game)))
			{
				ScraperSearchParams search;
				search.game = *game;
				search.system = *sys;
				
				queue.push(search);
			}
		}
	}

	return queue;
}

bool GuiScraperStart::input(InputConfig* config, Input input)
{
	bool consumed = GuiComponent::input(config, input);
	if(consumed)
		return true;
	
	if(input.value != 0 && config->isMappedTo(BUTTON_BACK, input))
	{
		delete this;
		return true;
	}

	if(config->isMappedTo("start", input) && input.value != 0)
	{
		// close everything
		Window* window = mWindow;
		while(window->peekGui() && window->peekGui() != ViewController::get())
			delete window->peekGui();
	}


	return false;
}

std::vector<HelpPrompt> GuiScraperStart::getHelpPrompts()
{
	std::vector<HelpPrompt> prompts = mMenu.getHelpPrompts();
	prompts.push_back(HelpPrompt(BUTTON_BACK, _("BACK")));
	prompts.push_back(HelpPrompt("start", _("CLOSE"))); // batocera
	return prompts;
}
