#include "guis/GuiThemeInstallStart.h"

#include "ApiSystem.h"
#include "components/OptionListComponent.h"
#include "guis/GuiThemeInstall.h"
#include "guis/GuiSettings.h"
#include "views/ViewController.h"
#include "utils/StringUtil.h"
#include "components/ComponentGrid.h"
#include "LocaleES.h"

GuiThemeInstallStart::GuiThemeInstallStart(Window* window)
	: GuiComponent(window), mMenu(window, _("SELECT THEME").c_str())
{
	auto theme = ThemeData::getMenuTheme();

	addChild(&mMenu);
	
	std::vector<std::string> availableThemes = ApiSystem::getInstance()->getBatoceraThemesList();

	for(auto it = availableThemes.begin(); it != availableThemes.end(); it++)
	{
		auto parts = Utils::String::split(*it, '\t');
		if (parts.size() != 3)
			continue;

		bool isInstalled = (Utils::String::startsWith(parts[0],"[I]"));
		std::string themeName = parts[1];
		std::string themeUrl = parts[2];

		ComponentListRow row;

		// icon
		auto icon = std::make_shared<ImageComponent>(mWindow);
		icon->setImage(isInstalled ? ":/star_filled.svg" : ":/star_unfilled.svg");
		icon->setColorShift(theme->Text.color);
		icon->setResize(0, theme->Text.font->getLetterHeight() * 1.25f);
		row.addElement(icon, false);

		// spacer between icon and text
		auto spacer = std::make_shared<GuiComponent>(mWindow);
		spacer->setSize(10, 0);
		row.addElement(spacer, false);

		auto grid = std::make_shared<MultiLineMenuEntry>(window, themeName, themeUrl);
		row.addElement(grid, true);
		row.makeAcceptInputHandler([this, themeName] { start(themeName); });

		mMenu.addRow(row);	
	}

	mMenu.addButton(_("BACK"), "back", [&] { delete this; });

	if (Renderer::isSmallScreen())
		mMenu.setPosition((Renderer::getScreenWidth() - mMenu.getSize().x()) / 2, (Renderer::getScreenHeight() - mMenu.getSize().y()) / 2);
	else
		mMenu.setPosition((Renderer::getScreenWidth() - mMenu.getSize().x()) / 2, Renderer::getScreenHeight() * 0.15f);
}

void GuiThemeInstallStart::start(std::string themeName)
{
	mWindow->pushGui(new GuiThemeInstall(mWindow, themeName.c_str()));
    delete this;
}

bool GuiThemeInstallStart::input(InputConfig* config, Input input)
{
	if(GuiComponent::input(config, input))
		return true;
	
	if(input.value != 0 && config->isMappedTo(BUTTON_BACK, input))
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
	}
	return false;
}

std::vector<HelpPrompt> GuiThemeInstallStart::getHelpPrompts()
{
	std::vector<HelpPrompt> prompts = mMenu.getHelpPrompts();
	prompts.push_back(HelpPrompt(BUTTON_BACK, _("BACK")));
	prompts.push_back(HelpPrompt("start", _("CLOSE")));
	return prompts;
}
