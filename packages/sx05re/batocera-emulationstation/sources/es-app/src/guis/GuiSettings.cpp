#include "guis/GuiSettings.h"

#include "views/ViewController.h"
#include "Settings.h"
#include "SystemData.h"
#include "Window.h"
#include "LocaleES.h"
#include "SystemConf.h"

GuiSettings::GuiSettings(Window* window, const char* title) : GuiComponent(window), mMenu(window, title)
{
	addChild(&mMenu);

	mMenu.addButton(_("BACK"), "go back", [this] { delete this; }); // batocera

	setSize((float)Renderer::getScreenWidth(), (float)Renderer::getScreenHeight());

	if (Renderer::isSmallScreen())
		mMenu.setPosition((Renderer::getScreenWidth() - mMenu.getSize().x()) / 2, (Renderer::getScreenHeight() - mMenu.getSize().y()) / 2);
	else
		mMenu.setPosition((mSize.x() - mMenu.getSize().x()) / 2, Renderer::getScreenHeight() * 0.15f);
}

GuiSettings::~GuiSettings()
{
	if(doSave) save(); // batocera
}

void GuiSettings::save()
{
	if(!mSaveFuncs.size())
		return;

	for(auto it = mSaveFuncs.cbegin(); it != mSaveFuncs.cend(); it++)
		(*it)();

	Settings::getInstance()->saveFile();
	SystemConf::getInstance()->saveSystemConf();
}

bool GuiSettings::input(InputConfig* config, Input input)
{
	if(config->isMappedTo(BUTTON_BACK, input) && input.value != 0)
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
		return true;
	}
	
	return GuiComponent::input(config, input);
}

HelpStyle GuiSettings::getHelpStyle()
{
	HelpStyle style = HelpStyle();
	style.applyTheme(ViewController::get()->getState().getSystem()->getTheme(), "system");
	return style;
}

std::vector<HelpPrompt> GuiSettings::getHelpPrompts()
{
	std::vector<HelpPrompt> prompts = mMenu.getHelpPrompts();

	prompts.push_back(HelpPrompt(BUTTON_BACK, _("BACK")));
	prompts.push_back(HelpPrompt("start", _("CLOSE"))); // batocera

	return prompts;
}

void GuiSettings::addSubMenu(const std::string& label, const std::function<void()>& func) 
{
	ComponentListRow row;
	row.makeAcceptInputHandler(func);

	auto theme = ThemeData::getMenuTheme();

	auto entryMenu = std::make_shared<TextComponent>(mWindow, label, theme->Text.font, theme->Text.color);
	row.addElement(entryMenu, true);
	row.addElement(makeArrow(mWindow), false);
	mMenu.addRow(row);
};