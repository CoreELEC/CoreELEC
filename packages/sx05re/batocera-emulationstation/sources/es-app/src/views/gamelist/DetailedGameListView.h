#pragma once
#ifndef ES_APP_VIEWS_GAME_LIST_DETAILED_GAME_LIST_VIEW_H
#define ES_APP_VIEWS_GAME_LIST_DETAILED_GAME_LIST_VIEW_H

#include "components/DateTimeComponent.h"
#include "components/RatingComponent.h"
#include "components/ScrollableContainer.h"
#include "views/gamelist/BasicGameListView.h"

class VideoComponent;

class DetailedGameListView : public BasicGameListView
{
public:
	DetailedGameListView(Window* window, FolderData* root);
	~DetailedGameListView();

	virtual void onThemeChanged(const std::shared_ptr<ThemeData>& theme) override;
	virtual void onShow() override;

	virtual const char* getName() const override
	{
		if (!mCustomThemeName.empty())
			return mCustomThemeName.c_str();

		return "detailed";
	}

	virtual void launch(FileData* game) override;

private:
	void updateInfoPanel();

	void createVideo();
	void createMarquee();
	void createImage();

	void initMDLabels();
	void initMDValues();
	
	ImageComponent* mImage;
	ImageComponent* mMarquee;
	VideoComponent* mVideo;

	TextComponent mLblRating, mLblReleaseDate, mLblDeveloper, mLblPublisher, mLblGenre, mLblPlayers, mLblLastPlayed, mLblPlayCount;

	RatingComponent mRating;
	DateTimeComponent mReleaseDate;
	TextComponent mDeveloper;
	TextComponent mPublisher;
	TextComponent mGenre;
	TextComponent mPlayers;
	DateTimeComponent mLastPlayed;
	TextComponent mPlayCount;
	TextComponent mName;

	std::vector<TextComponent*> getMDLabels();
	std::vector<GuiComponent*> getMDValues();

	ScrollableContainer mDescContainer;
	TextComponent mDescription;
};

#endif // ES_APP_VIEWS_GAME_LIST_DETAILED_GAME_LIST_VIEW_H
