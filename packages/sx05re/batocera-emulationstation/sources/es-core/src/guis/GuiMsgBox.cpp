#include "guis/GuiMsgBox.h"

#include "components/ButtonComponent.h"
#include "components/MenuComponent.h"
#include "components/ImageComponent.h"
#include "resources/ResourceManager.h"
#include "LocaleES.h"

#define HORIZONTAL_PADDING_PX  (Renderer::getScreenWidth()*0.01)

GuiMsgBox::GuiMsgBox(Window* window, const std::string& text, const std::string& name1, const std::function<void()>& func1, GuiMsgBoxIcon icon) 
	: GuiMsgBox(window, text, name1, func1, "", nullptr, "", nullptr, icon) { }

GuiMsgBox::GuiMsgBox(Window* window, const std::string& text,
	const std::string& name1, const std::function<void()>& func1,
	const std::string& name2, const std::function<void()>& func2,
	GuiMsgBoxIcon icon)
	: GuiMsgBox(window, text, name1, func1, name2, func2, "", nullptr, icon) { }

GuiMsgBox::GuiMsgBox(Window* window, const std::string& text, 
	const std::string& name1, const std::function<void()>& func1,
	const std::string& name2, const std::function<void()>& func2, 
	const std::string& name3, const std::function<void()>& func3,
	GuiMsgBoxIcon icon) : GuiComponent(window),
	mBackground(window, ":/frame.png"), mGrid(window, Vector2i(2, 2))
	
{
	auto theme = ThemeData::getMenuTheme();
	mBackground.setImagePath(theme->Background.path);
	mBackground.setCenterColor(theme->Background.color);
	mBackground.setEdgeColor(theme->Background.color);

	float width = Renderer::getScreenWidth() * 0.6f; // max width
	float minWidth = Renderer::getScreenWidth() * 0.3f; // minimum width
	
	mImage = nullptr;

	std::string imageFile;

	switch (icon)
	{
	case ICON_INFORMATION:
		imageFile = ":/info.svg";
		break;
	case ICON_QUESTION:
		imageFile = ":/question.svg";
		break;
	case ICON_WARNING:
		imageFile = ":/warning.svg";
		break;
	case ICON_ERROR:
		imageFile = ":/alert.svg";
		break;
	case ICON_AUTOMATIC:

		if (text.rfind("?") != std::string::npos || name1 == _("YES"))
			imageFile = ":/question.svg";
		else if (name1 == _("OK"))
		{
			if (name2.empty())
				imageFile = ":/info.svg";
			else
				imageFile = ":/question.svg";
		}

		break;
	}

	if (!imageFile.empty() && ResourceManager::getInstance()->fileExists(imageFile))
	{
		mImage = std::make_shared<ImageComponent>(window);
		mImage->setImage(imageFile);
		mImage->setColorShift(theme->Text.color);
		mImage->setMaxSize(theme->Text.font->getLetterHeight() * 2.0f, theme->Text.font->getLetterHeight() * 2.0f);

		mGrid.setEntry(mImage, Vector2i(0, 0), false, false);
	}

	mMsg = std::make_shared<TextComponent>(mWindow, text, ThemeData::getMenuTheme()->Text.font, ThemeData::getMenuTheme()->Text.color, mImage == nullptr ? ALIGN_CENTER : ALIGN_LEFT); // CENTER
	mMsg->setPadding(Vector4f(Renderer::getScreenWidth()*0.015f, 0, Renderer::getScreenWidth()*0.015f, 0));
	
	mGrid.setEntry(mMsg, Vector2i(mImage == nullptr ? 0 : 1, 0), false, false, Vector2i(mImage == nullptr ? 2 : 1, 1));

	// create the buttons
	mButtons.push_back(std::make_shared<ButtonComponent>(mWindow, name1, name1, std::bind(&GuiMsgBox::deleteMeAndCall, this, func1)));
	if(!name2.empty())
		mButtons.push_back(std::make_shared<ButtonComponent>(mWindow, name2, name3, std::bind(&GuiMsgBox::deleteMeAndCall, this, func2)));
	if(!name3.empty())
		mButtons.push_back(std::make_shared<ButtonComponent>(mWindow, name3, name3, std::bind(&GuiMsgBox::deleteMeAndCall, this, func3)));

	// set accelerator automatically (button to press when BUTTON_BACK is pressed)
	if(mButtons.size() == 1)
	{
		mAcceleratorFunc = mButtons.front()->getPressedFunc();
	}else{
		for(auto it = mButtons.cbegin(); it != mButtons.cend(); it++)
		{
			if(Utils::String::toUpper((*it)->getText()) == _("OK") || Utils::String::toUpper((*it)->getText()) == _("NO"))
			{
				mAcceleratorFunc = (*it)->getPressedFunc();
				break;
			}
		}
	}

	// put the buttons into a ComponentGrid
	mButtonGrid = makeButtonGrid(mWindow, mButtons);
	mGrid.setEntry(mButtonGrid, Vector2i(0, 1), true, false, Vector2i(2, 1), GridFlags::BORDER_TOP);

	// decide final width
	if(mMsg->getSize().x() < width && mButtonGrid->getSize().x() < width)
	{
		// mMsg and buttons are narrower than width
		width = Math::max(mButtonGrid->getSize().x(), mMsg->getSize().x());
		width = Math::max(width, minWidth);
	}
	
	// now that we know width, we can find height
	mMsg->setSize(width, 0); // mMsg->getSize.y() now returns the proper length
	const float msgHeight = Math::max(Font::get(FONT_SIZE_LARGE)->getHeight(), mMsg->getSize().y()*1.225f);
	setSize(width + HORIZONTAL_PADDING_PX*2, msgHeight + mButtonGrid->getSize().y());

	// center for good measure
	setPosition((Renderer::getScreenWidth() - mSize.x()) / 2.0f, (Renderer::getScreenHeight() - mSize.y()) / 2.0f);

	addChild(&mBackground);
	addChild(&mGrid);
}

bool GuiMsgBox::input(InputConfig* config, Input input)
{
	// special case for when GuiMsgBox comes up to report errors before anything has been configured
	if(config->getDeviceId() == DEVICE_KEYBOARD && !config->isConfigured() && input.value && 
		(input.id == SDLK_RETURN || input.id == SDLK_ESCAPE || input.id == SDLK_SPACE))
	{
		mAcceleratorFunc();
		return true;
	}

	/* when it's not configured, allow to remove the message box too to allow the configdevice window a chance */
	if(mAcceleratorFunc && ((config->isMappedTo(BUTTON_BACK, input) && input.value != 0) || (config->isConfigured() == false && input.type == TYPE_BUTTON))) // batocera
	//{
		mAcceleratorFunc();
	//	return true;
	//}

	return GuiComponent::input(config, input);
}

void GuiMsgBox::onSizeChanged()
{
	mGrid.setSize(mSize);

	if (mImage != nullptr)
		mGrid.setColWidthPerc(0, (ThemeData::getMenuTheme()->Text.font->getLetterHeight() * 4.5f) / mSize.x());

	mGrid.setRowHeightPerc(1, mButtonGrid->getSize().y() / mSize.y());
			
	mMsg->setSize(mSize.x() - HORIZONTAL_PADDING_PX*2, mGrid.getRowHeight(0));
	mGrid.onSizeChanged();

	mBackground.fitTo(mSize, Vector3f::Zero(), Vector2f(-32, -32));
}

void GuiMsgBox::deleteMeAndCall(const std::function<void()>& func)
{
	auto funcCopy = func;
	delete this;

	if(funcCopy)
		funcCopy();

}

std::vector<HelpPrompt> GuiMsgBox::getHelpPrompts()
{
	return mGrid.getHelpPrompts();
}
