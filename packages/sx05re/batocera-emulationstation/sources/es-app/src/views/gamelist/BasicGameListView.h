#pragma once
#ifndef ES_APP_VIEWS_GAME_LIST_BASIC_GAME_LIST_VIEW_H
#define ES_APP_VIEWS_GAME_LIST_BASIC_GAME_LIST_VIEW_H

#include "components/TextListComponent.h"
#include "views/gamelist/ISimpleGameListView.h"

class BasicGameListView : public ISimpleGameListView
{
public:
	BasicGameListView(Window* window, FolderData* root);

	// Called when a FileData* is added, has its metadata changed, or is removed
	virtual void onFileChanged(FileData* file, FileChangeType change);

	virtual void onThemeChanged(const std::shared_ptr<ThemeData>& theme);

	virtual FileData* getCursor() override;
	virtual void setCursor(FileData* file) override;
	virtual int getCursorIndex() override; // batocera
	virtual void setCursorIndex(int index) override; // batocera

	virtual const char* getName() const override
	{
		if (!mCustomThemeName.empty())
			return mCustomThemeName.c_str();

		return "basic";
	}

	virtual std::vector<HelpPrompt> getHelpPrompts() override;
	virtual void launch(FileData* game) override;

protected:
	virtual std::string getQuickSystemSelectRightButton() override;
	virtual std::string getQuickSystemSelectLeftButton() override;
	virtual void populateList(const std::vector<FileData*>& files) override;
	virtual void remove(FileData* game, bool deleteFile) override;
	virtual void addPlaceholder();

	TextListComponent<FileData*> mList;
};

#endif // ES_APP_VIEWS_GAME_LIST_BASIC_GAME_LIST_VIEW_H
