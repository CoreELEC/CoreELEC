#pragma once
#ifndef ES_CORE_COMPONENTS_GRID_TILE_COMPONENT_H
#define ES_CORE_COMPONENTS_GRID_TILE_COMPONENT_H

#include "NinePatchComponent.h"
#include "ImageComponent.h"
#include "TextComponent.h"


class VideoComponent;
struct GridTileProperties
{
	Vector2f mSize;
	Vector2f mPadding;
	unsigned int mImageColor;
	std::string mBackgroundImage;
	Vector2f mBackgroundCornerSize;
	unsigned int mBackgroundCenterColor;
	unsigned int mBackgroundEdgeColor;

	std::string mImageSizeMode;
	std::string mSelectionMode;

	Vector2f mLabelSize;
	unsigned int mLabelColor;
	unsigned int mLabelBackColor;

	unsigned int mLabelGlowColor;
	unsigned int mLabelGlowSize;

	std::string		mFontPath;
	unsigned int	mFontSize;

	Vector2f		mMirror;

	/*
	<fontPath>. / main / fonts / Dosis - Bold.ttf< / fontPath>
		<fontSize>0.025< / fontSize>*/
};

class GridTileComponent : public GuiComponent
{
public:
	GridTileComponent(Window* window);
	~GridTileComponent();

	void render(const Transform4x4f& parentTrans) override;

	virtual void applyTheme(const std::shared_ptr<ThemeData>& theme, const std::string& view, const std::string& element, unsigned int properties);

	// Made this a static function because the ImageGridComponent need to know the default tile max size
	// to calculate the grid dimension before it instantiate the GridTileComponents
	static Vector2f getDefaultTileSize();
	Vector2f getSelectedTileSize() const;
	bool isSelected() const;

	void reset();

	void setLabel(std::string name);
	void setVideo(const std::string& path, float defaultDelay = -1.0);

	void setImage(const std::string& path);
	// void setImage(const std::shared_ptr<TextureResource>& texture, std::string name);
	void setSelected(bool selected, bool allowAnimation = true, Vector3f* pPosition = NULL, bool force=false);
	void setVisible(bool visible);

	void forceSize(Vector2f size, float selectedZoom = 1.0);

	void renderBackground(const Transform4x4f& parentTrans);
	void renderContent(const Transform4x4f& parentTrans);

	bool shouldSplitRendering() { return isAnimationPlaying(3); };

	Vector3f getBackgroundPosition();

	virtual void onShow();
	virtual void onHide();
	virtual void update(int deltaTime);

	std::shared_ptr<TextureResource> getTexture();

private:
	void resize();
	const GridTileProperties& getCurrentProperties();

	std::shared_ptr<ImageComponent> mImage;

	TextComponent mLabel;	

	bool mLabelVisible;
	bool mLabelMerged;

	NinePatchComponent mBackground;

	GridTileProperties mDefaultProperties;
	GridTileProperties mSelectedProperties;
	GridTileProperties mMixedProperties;

	std::string mCurrentPath;
	std::string mVideoPath;

	void setSelectedZoom(float percent);

	float mSelectedZoomPercent;
	bool mSelected;
	bool mVisible;

	Vector3f mAnimPosition;

	VideoComponent* mVideo;
	bool mVideoPlaying;
	bool mShown;
};

#endif // ES_CORE_COMPONENTS_GRID_TILE_COMPONENT_H
