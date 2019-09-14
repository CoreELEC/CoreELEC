#include "components/MenuComponent.h"

#include "components/ButtonComponent.h"

#define BUTTON_GRID_VERT_PADDING  (Renderer::getScreenHeight()*0.0296296)
#define BUTTON_GRID_HORIZ_PADDING (Renderer::getScreenWidth()*0.0052083333)

#define TITLE_HEIGHT (mTitle->getFont()->getLetterHeight() + TITLE_VERT_PADDING)

MenuComponent::MenuComponent(Window* window, const char* title, const std::shared_ptr<Font>& titleFont) : GuiComponent(window),
	mBackground(window), mGrid(window, Vector2i(1, 3))
{
	auto theme = ThemeData::getMenuTheme();

	addChild(&mBackground);
	addChild(&mGrid);

	mBackground.setImagePath(theme->Background.path);
	mBackground.setCenterColor(theme->Background.color);
	mBackground.setEdgeColor(theme->Background.color);

	// set up title
	mTitle = std::make_shared<TextComponent>(mWindow);
	mTitle->setHorizontalAlignment(ALIGN_CENTER);
	mTitle->setColor(theme->Title.color); // 0x555555FF
	setTitle(title, theme->Title.font); //  titleFont
	mGrid.setEntry(mTitle, Vector2i(0, 0), false);

	mGrid.setUnhandledInputCallback([this](InputConfig* config, Input input) -> bool {
		if (config->isMappedLike("down", input)) {
			mGrid.setCursorTo(mList);
			mList->setCursorIndex(0);
			return true;
		}
		if (config->isMappedLike("up", input)) {
			mList->setCursorIndex(mList->size() - 1);
			if (mButtons.size()) {
				mGrid.moveCursor(Vector2i(0, 1));
			}
			else {
				mGrid.setCursorTo(mList);
			}
			return true;
		}
		return false;
	});

	// set up list which will never change (externally, anyway)
	mList = std::make_shared<ComponentList>(mWindow);
	mGrid.setEntry(mList, Vector2i(0, 1), true);

	updateGrid();
	updateSize();

	mGrid.resetCursor();
}

void MenuComponent::addWithLabel(const std::string& label, const std::shared_ptr<GuiComponent>& comp, const std::function<void()>& func, const std::string iconName, bool setCursorHere, bool invert_when_selected)
{
	auto theme = ThemeData::getMenuTheme();

	ComponentListRow row;

	if (!iconName.empty())
	{
		std::string iconPath = theme->getMenuIcon(iconName);
		if (!iconPath.empty())
		{
			// icon
			auto icon = std::make_shared<ImageComponent>(mWindow);
			icon->setImage(iconPath);
			icon->setColorShift(theme->Text.color);
			icon->setResize(0, theme->Text.font->getLetterHeight() * 1.25f);
			row.addElement(icon, false);

			// spacer between icon and text
			auto spacer = std::make_shared<GuiComponent>(mWindow);
			spacer->setSize(10, 0);
			row.addElement(spacer, false);
		}
	}

	row.addElement(std::make_shared<TextComponent>(mWindow, Utils::String::toUpper(label), theme->Text.font, theme->Text.color), true);
	row.addElement(comp, false, invert_when_selected);

	if (func != nullptr)
		row.makeAcceptInputHandler(func);

	addRow(row, setCursorHere);
}

void MenuComponent::addEntry(const std::string name, bool add_arrow, const std::function<void()>& func, const std::string iconName, bool setCursorHere, bool invert_when_selected)
{
	auto theme = ThemeData::getMenuTheme();
	std::shared_ptr<Font> font = theme->Text.font;
	unsigned int color = theme->Text.color;

	// populate the list
	ComponentListRow row;

	if (!iconName.empty())
	{
		std::string iconPath = theme->getMenuIcon(iconName);
		if (!iconPath.empty())
		{
			// icon
			auto icon = std::make_shared<ImageComponent>(mWindow);
			icon->setImage(iconPath);
			icon->setColorShift(theme->Text.color);
			icon->setResize(0, theme->Text.font->getLetterHeight() * 1.25f);
			row.addElement(icon, false);

			// spacer between icon and text
			auto spacer = std::make_shared<GuiComponent>(mWindow);
			spacer->setSize(10, 0);
			row.addElement(spacer, false);
		}
	}

	row.addElement(std::make_shared<TextComponent>(mWindow, name, font, color), true, invert_when_selected);

	if (add_arrow)
		row.addElement(makeArrow(mWindow), false);

	if (func != nullptr)
		row.makeAcceptInputHandler(func);

	addRow(row, setCursorHere);
}

void MenuComponent::setTitle(const char* title, const std::shared_ptr<Font>& font)
{
	mTitle->setText(Utils::String::toUpper(title));
	mTitle->setFont(font);
}

float MenuComponent::getButtonGridHeight() const
{
	auto menuTheme = ThemeData::getMenuTheme();
	return (mButtonGrid ? mButtonGrid->getSize().y() : menuTheme->Text.font->getHeight() + BUTTON_GRID_VERT_PADDING);	
}

void MenuComponent::updateSize()
{
	// GPI
	if (Renderer::isSmallScreen())
	{
		setSize(Renderer::getScreenWidth(), Renderer::getScreenHeight());
		return;
	}

	const float maxHeight = Renderer::getScreenHeight() * 0.75f;
	float height = TITLE_HEIGHT + mList->getTotalRowHeight() + getButtonGridHeight() + 2;
	if(height > maxHeight)
	{
		height = TITLE_HEIGHT + getButtonGridHeight();
		int i = 0;
		while(i < mList->size())
		{
			float rowHeight = mList->getRowHeight(i);
			if(height + rowHeight < maxHeight)
				height += rowHeight;
			else
				break;
			i++;
		}
	}

	float width = (float)Math::min((int)Renderer::getScreenHeight(), (int)(Renderer::getScreenWidth() * 0.90f));
	setSize(width, height);
}

void MenuComponent::onSizeChanged()
{
	mBackground.fitTo(mSize, Vector3f::Zero(), Vector2f(-32, -32));

	// update grid row/col sizes
	mGrid.setRowHeightPerc(0, TITLE_HEIGHT / mSize.y());
	mGrid.setRowHeightPerc(2, getButtonGridHeight() / mSize.y());

	mGrid.setSize(mSize);
}

void MenuComponent::addButton(const std::string& name, const std::string& helpText, const std::function<void()>& callback)
{
	mButtons.push_back(std::make_shared<ButtonComponent>(mWindow, Utils::String::toUpper(name), helpText, callback));
	updateGrid();
	updateSize();
}

void MenuComponent::updateGrid()
{
	if(mButtonGrid)
		mGrid.removeEntry(mButtonGrid);

	mButtonGrid.reset();

	if(mButtons.size())
	{
		mButtonGrid = makeButtonGrid(mWindow, mButtons);
		mGrid.setEntry(mButtonGrid, Vector2i(0, 2), true, false);
	}
}

std::vector<HelpPrompt> MenuComponent::getHelpPrompts()
{
	return mGrid.getHelpPrompts();
}

std::shared_ptr<ComponentGrid> makeButtonGrid(Window* window, const std::vector< std::shared_ptr<ButtonComponent> >& buttons)
{
	std::shared_ptr<ComponentGrid> buttonGrid = std::make_shared<ComponentGrid>(window, Vector2i((int)buttons.size(), 2));

	float buttonGridWidth = (float)BUTTON_GRID_HORIZ_PADDING * buttons.size(); // initialize to padding
	for(int i = 0; i < (int)buttons.size(); i++)
	{
		buttonGrid->setEntry(buttons.at(i), Vector2i(i, 0), true, false);
		buttonGridWidth += buttons.at(i)->getSize().x();
	}
	for(unsigned int i = 0; i < buttons.size(); i++)
	{
		buttonGrid->setColWidthPerc(i, (buttons.at(i)->getSize().x() + BUTTON_GRID_HORIZ_PADDING) / buttonGridWidth);
	}

	buttonGrid->setSize(buttonGridWidth, buttons.at(0)->getSize().y() + BUTTON_GRID_VERT_PADDING + 2);
	buttonGrid->setRowHeightPerc(1, 2 / buttonGrid->getSize().y()); // spacer row to deal with dropshadow to make buttons look centered

	return buttonGrid;
}

std::shared_ptr<ImageComponent> makeArrow(Window* window)
{
	auto menuTheme = ThemeData::getMenuTheme();

	auto bracket = std::make_shared<ImageComponent>(window);
	bracket->setImage(ThemeData::getMenuTheme()->Icons.arrow); // ":/arrow.svg");
	bracket->setColorShift(menuTheme->Text.color);
	bracket->setResize(0, round(menuTheme->Text.font->getLetterHeight()));

	return bracket;
}
