#include "guis/GuiScraperMulti.h"

#include "components/ButtonComponent.h"
#include "components/MenuComponent.h"
#include "components/ScraperSearchComponent.h"
#include "components/TextComponent.h"
#include "guis/GuiMsgBox.h"
#include "views/ViewController.h"
#include "Gamelist.h"
#include "PowerSaver.h"
#include "SystemData.h"
#include "Window.h"
#include "LocaleES.h"

GuiScraperMulti::GuiScraperMulti(Window* window, const std::queue<ScraperSearchParams>& searches, bool approveResults) :
	GuiComponent(window), mBackground(window, ":/frame.png"), mGrid(window, Vector2i(1, 5)),
	mSearchQueue(searches)
{
	auto theme = ThemeData::getMenuTheme();
	mBackground.setImagePath(theme->Background.path); // ":/frame.png"
	mBackground.setCenterColor(theme->Background.color);
	mBackground.setEdgeColor(theme->Background.color);

	assert(mSearchQueue.size());

	addChild(&mBackground);
	addChild(&mGrid);

	PowerSaver::pause();
	mIsProcessing = true;

	mTotalGames = (int)mSearchQueue.size();
	mCurrentGame = 0;
	mTotalSuccessful = 0;
	mTotalSkipped = 0;

	// set up grid
	mTitle = std::make_shared<TextComponent>(mWindow, _("SCRAPING IN PROGRESS"), theme->Title.font, theme->Title.color, ALIGN_CENTER); // batocera
	mGrid.setEntry(mTitle, Vector2i(0, 0), false, true);

	mSystem = std::make_shared<TextComponent>(mWindow, _("SYSTEM"), theme->Text.font, theme->Text.color, ALIGN_CENTER); // batocera
	mGrid.setEntry(mSystem, Vector2i(0, 1), false, true);

	mSubtitle = std::make_shared<TextComponent>(mWindow, _("subtitle text"), theme->TextSmall.font, theme->TextSmall.color, ALIGN_CENTER); // batocera
	mGrid.setEntry(mSubtitle, Vector2i(0, 2), false, true);

	mSearchComp = std::make_shared<ScraperSearchComponent>(mWindow,
		approveResults ? ScraperSearchComponent::ALWAYS_ACCEPT_MATCHING_CRC : ScraperSearchComponent::ALWAYS_ACCEPT_FIRST_RESULT);
	mSearchComp->setAcceptCallback(std::bind(&GuiScraperMulti::acceptResult, this, std::placeholders::_1));
	mSearchComp->setSkipCallback(std::bind(&GuiScraperMulti::skip, this));
	mSearchComp->setCancelCallback(std::bind(&GuiScraperMulti::finish, this));
	mGrid.setEntry(mSearchComp, Vector2i(0, 3), mSearchComp->getSearchType() != ScraperSearchComponent::ALWAYS_ACCEPT_FIRST_RESULT, true);

	std::vector< std::shared_ptr<ButtonComponent> > buttons;

	if(approveResults)
	{
	  buttons.push_back(std::make_shared<ButtonComponent>(mWindow, _("INPUT"), _("search"), [&] { // batocera
			mSearchComp->openInputScreen(mSearchQueue.front());
			mGrid.resetCursor();
		}));

	  buttons.push_back(std::make_shared<ButtonComponent>(mWindow, _("SKIP"), _("SKIP"), [&] { // batocera
			skip();
			mGrid.resetCursor();
		}));
	}

	buttons.push_back(std::make_shared<ButtonComponent>(mWindow, _("STOP"), _("stop (progress saved)"), std::bind(&GuiScraperMulti::finish, this))); // batocera

	mButtonGrid = makeButtonGrid(mWindow, buttons);
	mGrid.setEntry(mButtonGrid, Vector2i(0, 4), true, false);

	setSize(Renderer::getScreenWidth() * 0.95f, Renderer::getScreenHeight() * 0.849f);
	setPosition((Renderer::getScreenWidth() - mSize.x()) / 2, (Renderer::getScreenHeight() - mSize.y()) / 2);

	doNextSearch();
}

GuiScraperMulti::~GuiScraperMulti()
{
	// view type probably changed (basic -> detailed)
	for(auto it = SystemData::sSystemVector.cbegin(); it != SystemData::sSystemVector.cend(); it++)
		ViewController::get()->reloadGameListView(*it, false);
}

void GuiScraperMulti::onSizeChanged()
{
	mBackground.fitTo(mSize, Vector3f::Zero(), Vector2f(-32, -32));

	mGrid.setRowHeightPerc(0, mTitle->getFont()->getLetterHeight() * 1.9725f / mSize.y(), false);
	mGrid.setRowHeightPerc(1, (mSystem->getFont()->getLetterHeight() + 2) / mSize.y(), false);
	mGrid.setRowHeightPerc(2, mSubtitle->getFont()->getHeight() * 1.75f / mSize.y(), false);
	mGrid.setRowHeightPerc(4, mButtonGrid->getSize().y() / mSize.y(), false);
	mGrid.setSize(mSize);
}

void GuiScraperMulti::doNextSearch()
{
	if(mSearchQueue.empty())
	{
		finish();
		return;
	}

	// update title
	std::stringstream ss;
	mSystem->setText(Utils::String::toUpper(mSearchQueue.front().system->getFullName()));

	// update subtitle
	ss.str(""); // clear
	char strbuf[256];
	snprintf(strbuf, 256, _("GAME %i OF %i").c_str(), mCurrentGame + 1, mTotalGames); // batocera
	ss << strbuf << " - " << Utils::String::toUpper(Utils::FileSystem::getFileName(mSearchQueue.front().game->getPath()));
	mSubtitle->setText(ss.str());

	mSearchComp->search(mSearchQueue.front());
}

void GuiScraperMulti::acceptResult(const ScraperSearchResult& result)
{
	ScraperSearchParams& search = mSearchQueue.front();

	search.game->metadata = result.mdl;
	updateGamelist(search.system);

	mSearchQueue.pop();
	mCurrentGame++;
	mTotalSuccessful++;
	doNextSearch();
}

void GuiScraperMulti::skip()
{
	mSearchQueue.pop();
	mCurrentGame++;
	mTotalSkipped++;
	doNextSearch();
}

void GuiScraperMulti::finish()
{
	std::stringstream ss;
	if(mTotalSuccessful == 0)
	{
		ss << _("WE CAN'T FIND ANY SYSTEMS!\n"
			"CHECK THAT YOUR PATHS ARE CORRECT IN THE SYSTEMS CONFIGURATION FILE, AND "
			"YOUR GAME DIRECTORY HAS AT LEAST ONE GAME WITH THE CORRECT EXTENSION."
			);
	} else {
	  char strbuf[256];
	  snprintf(strbuf, 256, ngettext("%i GAME SUCCESSFULLY SCRAPED!", "%i GAMES SUCCESSFULLY SCRAPED!", mTotalSuccessful).c_str(), mTotalSuccessful); // batocera
	  ss << strbuf;

	  if(mTotalSkipped > 0) {
	    snprintf(strbuf, 256, ngettext("%i GAME SKIPPED.", "%i GAMES SKIPPED.", mTotalSkipped).c_str(), mTotalSkipped); // batocera
	    ss << "\n" << strbuf;
	  }
	}

	mWindow->pushGui(new GuiMsgBox(mWindow, ss.str(),
		"OK", [&] { delete this; }));

	mIsProcessing = false;
	PowerSaver::resume();
}

std::vector<HelpPrompt> GuiScraperMulti::getHelpPrompts()
{
	return mGrid.getHelpPrompts();
}
