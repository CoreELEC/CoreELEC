#include "views/gamelist/BasicGameListView.h"

#include "utils/FileSystemUtil.h"
#include "views/UIModeController.h"
#include "views/ViewController.h"
#include "CollectionSystemManager.h"
#include "Settings.h"
#include "SystemData.h"
#include "LocaleES.h"

BasicGameListView::BasicGameListView(Window* window, FolderData* root)
	: ISimpleGameListView(window, root), mList(window)
{
	mList.setSize(mSize.x(), mSize.y() * 0.8f);
	mList.setPosition(0, mSize.y() * 0.2f);
	mList.setDefaultZIndex(20);
	addChild(&mList);

	populateList(root->getChildrenListToDisplay());
}

void BasicGameListView::onThemeChanged(const std::shared_ptr<ThemeData>& theme)
{
	ISimpleGameListView::onThemeChanged(theme);
	using namespace ThemeFlags;
	mList.applyTheme(theme, getName(), "gamelist", ALL);

	sortChildren();
}

void BasicGameListView::onFileChanged(FileData* file, FileChangeType change)
{
	if(change == FILE_METADATA_CHANGED)
	{
		// might switch to a detailed view
		ViewController::get()->reloadGameListView(this);
		return;
	}

	ISimpleGameListView::onFileChanged(file, change);
}

void BasicGameListView::populateList(const std::vector<FileData*>& files)
{
	mList.clear();

	std::string systemName = mRoot->getSystem()->getFullName();
	mHeaderText.setText(systemName);

	if (files.size() > 0)
	{
		bool favoritesFirst = Settings::getInstance()->getBool("FavoritesFirst");
		bool showFavoriteIcon = (systemName != "favorites");
		if (!showFavoriteIcon)
			favoritesFirst = false;

		if (favoritesFirst)
		{
			for (auto it = files.cbegin(); it != files.cend(); it++)
			{
				if (!(*it)->getFavorite())
					continue;

				if (showFavoriteIcon)
					mList.add(_U("\uF006 ") + (*it)->getName(), *it, ((*it)->getType() == FOLDER));
				else
					mList.add((*it)->getName(), *it, ((*it)->getType() == FOLDER));
			}
		}

		for (auto it = files.cbegin(); it != files.cend(); it++)
		{
			if ((*it)->getFavorite())
			{
				if (favoritesFirst)
					continue;

				if (showFavoriteIcon)
				{
					mList.add(_U("\uF006 ") + (*it)->getName(), *it, ((*it)->getType() == FOLDER));
					continue;
				}
			}

			mList.add((*it)->getName(), *it, ((*it)->getType() == FOLDER));
		}
	}
	else
	{
		addPlaceholder();
	}
}

FileData* BasicGameListView::getCursor()
{
	return mList.getSelected();
}

void BasicGameListView::setCursor(FileData* cursor)
{
	if(!mList.setCursor(cursor) && (!cursor->isPlaceHolder()))
	{
		populateList(cursor->getParent()->getChildrenListToDisplay());
		mList.setCursor(cursor);

		// update our cursor stack in case our cursor just got set to some folder we weren't in before
		if(mCursorStack.empty() || mCursorStack.top() != cursor->getParent())
		{
			std::stack<FileData*> tmp;
			FileData* ptr = cursor->getParent();
			while(ptr && ptr != mRoot)
			{
				tmp.push(ptr);
				ptr = ptr->getParent();
			}

			// flip the stack and put it in mCursorStack
			mCursorStack = std::stack<FileData*>();
			while(!tmp.empty())
			{
				mCursorStack.push(tmp.top());
				tmp.pop();
			}
		}
	}
}

void BasicGameListView::addPlaceholder()
{
	// empty list - add a placeholder
	FileData* placeholder = new FileData(PLACEHOLDER, _("<No Entries Found>"), this->mRoot->getSystem());
	mList.add(placeholder->getName(), placeholder, (placeholder->getType() == PLACEHOLDER));
}

std::string BasicGameListView::getQuickSystemSelectRightButton()
{
	return "right";
}

std::string BasicGameListView::getQuickSystemSelectLeftButton()
{
	return "left";
}

void BasicGameListView::launch(FileData* game)
{
	ViewController::get()->launch(game);
}

void BasicGameListView::remove(FileData *game, bool deleteFile)
{
	if (deleteFile)
		Utils::FileSystem::removeFile(game->getPath());  // actually delete the file on the filesystem
	FolderData* parent = game->getParent();
	if (getCursor() == game)                     // Select next element in list, or prev if none
	{
		std::vector<FileData*> siblings = parent->getChildrenListToDisplay();
		auto gameIter = std::find(siblings.cbegin(), siblings.cend(), game);
		unsigned int gamePos = (int)std::distance(siblings.cbegin(), gameIter);
		if (gameIter != siblings.cend())
		{
			if ((gamePos + 1) < siblings.size())
			{
				setCursor(siblings.at(gamePos + 1));
			} else if (gamePos > 1) {
				setCursor(siblings.at(gamePos - 1));
			}
		}
	}
	mList.remove(game);
	if(mList.size() == 0)
	{
		addPlaceholder();
	}
	delete game;                                 // remove before repopulating (removes from parent)
	onFileChanged(parent, FILE_REMOVED);           // update the view, with game removed
}

std::vector<HelpPrompt> BasicGameListView::getHelpPrompts()
{
	std::vector<HelpPrompt> prompts;

	if(Settings::getInstance()->getBool("QuickSystemSelect"))
	  prompts.push_back(HelpPrompt("left/right", _("SYSTEM"))); // batocera
	prompts.push_back(HelpPrompt("up/down", _("CHOOSE"))); // batocera
	prompts.push_back(HelpPrompt(BUTTON_OK, _("LAUNCH")));
	prompts.push_back(HelpPrompt(BUTTON_BACK, _("BACK")));
	if(!UIModeController::getInstance()->isUIModeKid())
	  prompts.push_back(HelpPrompt("select", _("OPTIONS"))); // batocera
	if(mRoot->getSystem()->isGameSystem())
	  prompts.push_back(HelpPrompt("x", _("RANDOM"))); // batocera
	if(mRoot->getSystem()->isGameSystem() && !UIModeController::getInstance()->isUIModeKid())
	{
		std::string prompt = CollectionSystemManager::get()->getEditingCollection();
		prompts.push_back(HelpPrompt("y", prompt));
	}
	return prompts;
}

// batocera
void BasicGameListView::setCursorIndex(int cursor){
	mList.setCursorIndex(cursor);
}

// batocera
int BasicGameListView::getCursorIndex(){
	return mList.getCursorIndex();
}
