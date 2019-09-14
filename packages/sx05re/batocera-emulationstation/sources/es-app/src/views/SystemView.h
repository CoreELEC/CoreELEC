#pragma once
#ifndef ES_APP_VIEWS_SYSTEM_VIEW_H
#define ES_APP_VIEWS_SYSTEM_VIEW_H

#include "components/IList.h"
#include "components/TextComponent.h"
#include "resources/Font.h"
#include "GuiComponent.h"
#include <memory>
#include <functional>

class AnimatedImageComponent;
class SystemData;

enum CarouselType : unsigned int
{
	HORIZONTAL = 0,
	VERTICAL = 1,
	VERTICAL_WHEEL = 2,
	HORIZONTAL_WHEEL = 3
};

struct SystemViewData
{
	std::shared_ptr<GuiComponent> logo;
	std::vector<GuiComponent*> backgroundExtras;
};

struct SystemViewCarousel
{
	CarouselType type;
	Vector2f pos;
	Vector2f size;
	Vector2f origin;
	float logoScale;
	float logoRotation;
	Vector2f logoRotationOrigin;
	Alignment logoAlignment;
	unsigned int color;
	unsigned int colorEnd;
	bool colorGradientHorizontal;
	int maxLogoCount; // number of logos shown on the carousel
	Vector2f logoSize;
	Vector2f logoPos;
	float zIndex;
};

class SystemView : public IList<SystemViewData, SystemData*>
{
public:
	SystemView(Window* window);
	~SystemView();

	virtual void onShow() override;
	virtual void onHide() override;

	void goToSystem(SystemData* system, bool animate);

	bool input(InputConfig* config, Input input) override;
	void update(int deltaTime) override;
	void render(const Transform4x4f& parentTrans) override;

	void onThemeChanged(const std::shared_ptr<ThemeData>& theme);

	std::vector<HelpPrompt> getHelpPrompts() override;
	virtual HelpStyle getHelpStyle() override;

protected:
	void onCursorChanged(const CursorState& state) override;

private:
	void	 activateExtras(int cursor, bool activate = true);
	void	 updateExtras(const std::function<void(GuiComponent*)>& func);
	void	 clearEntries();

	virtual void onScreenSaverActivate() override;
	virtual void onScreenSaverDeactivate() override;
	virtual void topWindow(bool isTop) override;

	void populate();
	void getViewElements(const std::shared_ptr<ThemeData>& theme);
	void getDefaultElements(void);
	void getCarouselFromTheme(const ThemeData::ThemeElement* elem);

	void renderCarousel(const Transform4x4f& parentTrans);
	void renderExtras(const Transform4x4f& parentTrans, float lower, float upper);
	void renderInfoBar(const Transform4x4f& trans);
	void renderFade(const Transform4x4f& trans);


	SystemViewCarousel mCarousel;
	TextComponent mSystemInfo;
	ImageComponent*		mStaticBackground;

	// unit is list index
	float mCamOffset;
	float mExtrasCamOffset;
	float mExtrasFadeOpacity;

	bool mViewNeedsReload;
	bool mShowing;
	bool launchKodi;

	bool mDisable;
	bool mScreensaverActive;

	int mLastCursor;
};

#endif // ES_APP_VIEWS_SYSTEM_VIEW_H
