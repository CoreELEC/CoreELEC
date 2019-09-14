#include "guis/GuiBezelInstallStart.h"

#include "ApiSystem.h"
#include "components/OptionListComponent.h"
#include "guis/GuiBezelInstall.h"
#include "guis/GuiSettings.h"
#include "views/ViewController.h"
#include "SystemData.h"

#include "LocaleES.h"

// Batocera integration with theBezelProject
GuiBezelInstallMenu::GuiBezelInstallMenu(Window* window)
        :GuiComponent(window), mMenu(window, _("THE BEZEL PROJECT").c_str())
{
	auto theme = ThemeData::getMenuTheme();

	addChild(&mMenu);
	ComponentListRow row;
      {
        auto openBezelInstallNow = [this] { mWindow->pushGui(new GuiBezelInstallStart(mWindow)); };
        row.makeAcceptInputHandler(openBezelInstallNow);
        auto BezelInstallSettings = std::make_shared<TextComponent>(mWindow, _("INSTALL BEZELS"),
			theme->Text.font, theme->Text.color);
        auto bracket = makeArrow(mWindow);
        row.addElement(BezelInstallSettings, true);
        row.addElement(bracket, false);
	mMenu.addRow(row);
	row.elements.clear();
      }
      { // Also add the ability to remove bezels installed previously 
        auto openBezelUninstallNow = [this] { mWindow->pushGui(new GuiBezelUninstallStart(mWindow)); };
        row.makeAcceptInputHandler(openBezelUninstallNow);
        auto BezelUninstallSettings = std::make_shared<TextComponent>(mWindow, _("UNINSTALL BEZELS"),
			theme->Text.font, theme->Text.color);
        auto bracket = makeArrow(mWindow);
        row.addElement(BezelUninstallSettings, true);
        row.addElement(bracket, false);
	mMenu.addRow(row);
	row.elements.clear();
      }

        mMenu.addButton(_("BACK"), "back", [&] { delete this; });

	if (Renderer::isSmallScreen())
		mMenu.setPosition((Renderer::getScreenWidth() - mMenu.getSize().x()) / 2, (Renderer::getScreenHeight() - mMenu.getSize().y()) / 2);
	else
		mMenu.setPosition((Renderer::getScreenWidth() - mMenu.getSize().x()) / 2, Renderer::getScreenHeight() * 0.15f);
}

bool GuiBezelInstallMenu::input(InputConfig* config, Input input)
{
	bool consumed = GuiComponent::input(config, input);
	if(consumed)
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

std::vector<HelpPrompt> GuiBezelInstallMenu::getHelpPrompts()
{
        std::vector<HelpPrompt> prompts = mMenu.getHelpPrompts();
        prompts.push_back(HelpPrompt(BUTTON_BACK, _("BACK")));
        prompts.push_back(HelpPrompt("start", _("CLOSE")));
        return prompts;
}

// Batocera install theBezelProject
GuiBezelInstallStart::GuiBezelInstallStart(Window* window)
	:GuiComponent(window), mMenu(window, _("INSTALL THE BEZEL PROJECT").c_str())
{
	auto theme = ThemeData::getMenuTheme();

	addChild(&mMenu);
	ComponentListRow row;
	
	row.addElement(std::make_shared<TextComponent>(window, _("SELECT SYSTEM WHERE BEZELS WILL BE INSTALLED"), theme->Text.font, theme->Text.color), true);
	mMenu.addRow(row);
	row.elements.clear();

	//list all bezels available from TheBezelProject
	std::vector<std::string> availableBezels = ApiSystem::getInstance()->getBatoceraBezelsList();

        for(auto it = availableBezels.begin(); it != availableBezels.end(); it++){

                auto itstring = std::make_shared<TextComponent>(mWindow,
                                (*it).c_str(), theme->TextSmall.font, theme->Text.color);

                char *tmp=new char [(*it).length()+1];
                mSelectedBezel=new char [(*it).length()+1];
                std::strcpy (tmp, (*it).c_str());
                // Get Bezel_System name (from string '[A] Bezel_name http://url_of_this_Bezel')
		// as bezel_cli
                char *bezel_cli = strtok (tmp, " \t");
                bezel_cli = strtok (NULL, " \t");
		
                // Names longer than this will crash GuiMsgBox downstream
                // "48" found by trials and errors. Ideally should be fixed
                // in es-core MsgBox -- FIXME
                if (strlen(bezel_cli) > 48)
                        bezel_cli[47]='\0';
                row.makeAcceptInputHandler([this, bezel_cli] {
                                strcpy (mSelectedBezel, bezel_cli);
                                this->start();
                                });
                row.addElement(itstring, true);
                mMenu.addRow(row);
                row.elements.clear();
        }

        mMenu.addButton(_("BACK"), "back", [&] { delete this; });

		if (Renderer::isSmallScreen())
			mMenu.setPosition((Renderer::getScreenWidth() - mMenu.getSize().x()) / 2, (Renderer::getScreenHeight() - mMenu.getSize().y()) / 2);
		else
			mMenu.setPosition((Renderer::getScreenWidth() - mMenu.getSize().x()) / 2, Renderer::getScreenHeight() * 0.15f);
}

void GuiBezelInstallStart::start()
{
  if(strcmp(mSelectedBezel,"")) {
    mWindow->pushGui(new GuiBezelInstall(mWindow, mSelectedBezel));
    delete this;
  }
}

bool GuiBezelInstallStart::input(InputConfig* config, Input input)
{
	bool consumed = GuiComponent::input(config, input);
	if(consumed)
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

std::vector<HelpPrompt> GuiBezelInstallStart::getHelpPrompts()
{
        std::vector<HelpPrompt> prompts = mMenu.getHelpPrompts();
        prompts.push_back(HelpPrompt(BUTTON_BACK, _("BACK")));
        prompts.push_back(HelpPrompt("start", _("CLOSE")));
	return prompts;
}

// And now uninstall them
GuiBezelUninstallStart::GuiBezelUninstallStart(Window* window)
	:GuiComponent(window), mMenu(window, _("UNINSTALL THE BEZEL PROJECT").c_str())
{
	auto theme = ThemeData::getMenuTheme();

	addChild(&mMenu);
	ComponentListRow row;
	
	row.addElement(std::make_shared<TextComponent>(window, _("SELECT SYSTEM WHERE BEZELS WILL BE REMOVED"), theme->Text.font, theme->Text.color), true);
	mMenu.addRow(row);
	row.elements.clear();

	//list all bezels available from TheBezelProject
	std::vector<std::string> availableBezels = ApiSystem::getInstance()->getBatoceraBezelsList();

        for(auto it = availableBezels.begin(); it != availableBezels.end(); it++){

                auto itstring = std::make_shared<TextComponent>(mWindow,
                                (*it).c_str(), theme->TextSmall.font, theme->Text.color);
                char *tmp=new char [(*it).length()+1];
                mSelectedBezel=new char [(*it).length()+1];
                std::strcpy (tmp, (*it).c_str());
                // Get Bezel_System name (from string '[I] Bezel_name http://url_of_this_Bezel')
		// as bezel_cli (short system name, long names will be fetched below)
                char *bezel_cli = strtok (tmp, " \t");
                bezel_cli = strtok (NULL, " \t");

		// Only show [I]nstalled bezels not the other
		if (strcmp (tmp, "[I]"))
			continue;

		std::vector<SystemData*>::const_iterator itSystem;
		for (itSystem = SystemData::sSystemVector.cbegin(); itSystem != SystemData::sSystemVector.cend(); itSystem++)
		{
			// Let's put the pretty name of the system
			if (! strcmp ((*itSystem)->getName().c_str(), bezel_cli))
				itstring = std::make_shared<TextComponent>(mWindow,
						(*itSystem)->getFullName(), theme->TextSmall.font, theme->Text.color);
		}

                // Names longer than this will crash GuiMsgBox downstream
                // "48" found by trials and errors. Ideally should be fixed
                // in es-core MsgBox -- FIXME
                if (strlen(bezel_cli) > 48)
                        bezel_cli[47]='\0';
                row.makeAcceptInputHandler([this, bezel_cli] {
                                strcpy (mSelectedBezel, bezel_cli);
                                this->start();
                                });
                row.addElement(itstring, true);
                mMenu.addRow(row);
                row.elements.clear();
        }

        mMenu.addButton(_("BACK"), "back", [&] { delete this; });

	if (Renderer::isSmallScreen())
		mMenu.setPosition((Renderer::getScreenWidth() - mMenu.getSize().x()) / 2, (Renderer::getScreenHeight() - mMenu.getSize().y()) / 2);
	else
		mMenu.setPosition((Renderer::getScreenWidth() - mMenu.getSize().x()) / 2, Renderer::getScreenHeight() * 0.15f);
}

void GuiBezelUninstallStart::start()
{
  if(strcmp(mSelectedBezel,"")) {
    mWindow->pushGui(new GuiBezelUninstall(mWindow, mSelectedBezel));
    delete this;
  }
}

bool GuiBezelUninstallStart::input(InputConfig* config, Input input)
{
	bool consumed = GuiComponent::input(config, input);
	if(consumed)
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

std::vector<HelpPrompt> GuiBezelUninstallStart::getHelpPrompts()
{
        std::vector<HelpPrompt> prompts = mMenu.getHelpPrompts();
        prompts.push_back(HelpPrompt(BUTTON_BACK, _("BACK")));
        prompts.push_back(HelpPrompt("start", _("CLOSE")));
	return prompts;
}

