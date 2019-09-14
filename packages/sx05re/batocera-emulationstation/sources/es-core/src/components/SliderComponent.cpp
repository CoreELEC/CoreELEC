#include "components/SliderComponent.h"

#include "resources/Font.h"
#include "LocaleES.h"

#define MOVE_REPEAT_DELAY 500
#define MOVE_REPEAT_RATE 40

SliderComponent::SliderComponent(Window* window, float min, float max, float increment, const std::string& suffix) : GuiComponent(window),
	mMin(min), mMax(max), mSingleIncrement(increment), mMoveRate(0), mKnob(window), mSuffix(suffix)
{
	assert((min - max) != 0);

	auto menuTheme = ThemeData::getMenuTheme();
	mColor = menuTheme->Text.color;

	// some sane default value
	mValue = (max + min) / 2;

	mKnob.setOrigin(0.5f, 0.5f);
	mKnob.setImage(ThemeData::getMenuTheme()->Icons.knob);
	mKnob.setColorShift(mColor);
	
	setSize(Renderer::getScreenWidth() * 0.15f, menuTheme->Text.font->getLetterHeight());
}

void SliderComponent::setColor(unsigned int color) 
{
	mColor = color;
	mKnob.setColorShift(mColor);
	onValueChanged();
}

bool SliderComponent::input(InputConfig* config, Input input)
{
	if(config->isMappedLike("left", input))
	{
		if(input.value)
			setValue(mValue - mSingleIncrement);

		mMoveRate = input.value ? -mSingleIncrement : 0;
		mMoveAccumulator = -MOVE_REPEAT_DELAY;
		return true;
	}
	if(config->isMappedLike("right", input))
	{
		if(input.value)
			setValue(mValue + mSingleIncrement);

		mMoveRate = input.value ? mSingleIncrement : 0;
		mMoveAccumulator = -MOVE_REPEAT_DELAY;
		return true;
	}

	return GuiComponent::input(config, input);
}

void SliderComponent::update(int deltaTime)
{
	if(mMoveRate != 0)
	{
		mMoveAccumulator += deltaTime;
		while(mMoveAccumulator >= MOVE_REPEAT_RATE)
		{
			setValue(mValue + mMoveRate);
			mMoveAccumulator -= MOVE_REPEAT_RATE;
		}
	}
	
	GuiComponent::update(deltaTime);
}

void SliderComponent::render(const Transform4x4f& parentTrans)
{
	Transform4x4f trans = parentTrans * getTransform();

	if (!Renderer::isVisibleOnScreen(trans.translation().x(), trans.translation().y(), mSize.x(), mSize.y()))
		return;

	Renderer::setMatrix(trans);

	// render suffix
	if(mValueCache)
		mFont->renderTextCache(mValueCache.get());

	float width = mSize.x() - mKnob.getSize().x() - (mValueCache ? mValueCache->metrics.size.x() + 4 : 0);

	//render line
	const float lineWidth = 2;
	Renderer::drawRect(mKnob.getSize().x() / 2, mSize.y() / 2 - lineWidth / 2, width, lineWidth, mColor);

	//render knob
	mKnob.render(trans);
	
	GuiComponent::renderChildren(trans);
}

void SliderComponent::setValue(float value)
{
	mValue = value;
	if(mValue < mMin)
		mValue = mMin;
	else if(mValue > mMax)
		mValue = mMax;

	onValueChanged();
}

float SliderComponent::getValue()
{
	return mValue;
}

void SliderComponent::onSizeChanged()
{
	if(!mSuffix.empty())
		mFont = Font::get((int)(mSize.y()), FONT_PATH_LIGHT);
	
	onValueChanged();
}

void SliderComponent::onValueChanged()
{
	// update suffix textcache
	if(mFont)
	{
		std::stringstream ss;
		ss << std::fixed;
		ss.precision(0);
		ss << mValue;
		ss << mSuffix;
		const std::string val = ss.str();

		ss.str("");
		ss.clear();
		ss << std::fixed;
		ss.precision(0);
		ss << mMax;
		ss << mSuffix;
		const std::string max = ss.str();

		Vector2f textSize = mFont->sizeText(max);
		mValueCache = std::shared_ptr<TextCache>(mFont->buildTextCache(val, mSize.x() - textSize.x(), (mSize.y() - textSize.y()) / 2, mColor));
		mValueCache->metrics.size[0] = textSize.x(); // fudge the width
	}

	// update knob position/size
	mKnob.setResize(0, mSize.y() * 0.7f);
	float lineLength = mSize.x() - mKnob.getSize().x() - (mValueCache ? mValueCache->metrics.size.x() + 4 : 0);
	mKnob.setPosition(((mValue + mMin) / mMax) * lineLength + mKnob.getSize().x()/2, mSize.y() / 2);
}

std::vector<HelpPrompt> SliderComponent::getHelpPrompts()
{
	std::vector<HelpPrompt> prompts;
	prompts.push_back(HelpPrompt("left/right", _("CHANGE"))); // batocera
	return prompts;
}
