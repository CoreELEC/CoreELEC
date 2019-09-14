#include "guis/GuiSystemsHide.h"

#include "components/OptionListComponent.h"
#include "components/SwitchComponent.h"
#include "guis/GuiMsgBox.h"
#include "views/ViewController.h"
#include "FileData.h"
#include "SystemData.h"
#include "LocaleES.h"
#include "SystemConf.h"

// Batocera: new option in batocera.conf to hide specific systems
// Example: put "nes.hide=1" in batocera.conf to *not* display NES category in ES 
GuiSystemsHide::GuiSystemsHide(Window* window) : GuiComponent(window),
  mMenu(window, _("SELECT SYSTEMS TO DISPLAY").c_str()) 
{  
	addChild(&mMenu);

	//list all systems available
	mSystems = std::make_shared< OptionListComponent<SystemData*> >(mWindow, _("SYSTEMS DISPLAYED"), true);
	for (auto it = SystemData::sSystemVector.cbegin(); it != SystemData::sSystemVector.cend(); it++)
	{
		mSystems->add((*it)->getFullName(), *it, (SystemConf::getInstance()->get((*it)->getName() + ".hide") != "1"));
	}
	mMenu.addWithLabel(_("SELECT SYSTEMS TO DISPLAY"), mSystems);

	mMenu.addButton(_("APPLY"), "apply", std::bind(&GuiSystemsHide::Apply, this));
	mMenu.addButton(_("REVERT"), "back", [&] { delete this; });

	if (Renderer::isSmallScreen())
		mMenu.setPosition((Renderer::getScreenWidth() - mMenu.getSize().x()) / 2, (Renderer::getScreenHeight() - mMenu.getSize().y()) / 2);
	else
		mMenu.setPosition((Renderer::getScreenWidth() - mMenu.getSize().x()) / 2, Renderer::getScreenHeight() * 0.15f);
}

void GuiSystemsHide::Apply()
{	
	std::string value_cfg_hidden;
	std::vector<SystemData*> sys = mSystems->getSelectedObjects();
	for (auto it = SystemData::sSystemVector.cbegin(); it != SystemData::sSystemVector.cend(); it++)
	{
		value_cfg_hidden="1";
		for(auto selected = sys.cbegin(); selected != sys.cend(); selected++) {
			if ((*it)->getName() == (*selected)->getName())
				value_cfg_hidden="0";
		}
		SystemConf::getInstance()->set((*it)->getName()+".hide", value_cfg_hidden);
		Settings::getInstance()->setBool((*it)->getName()+".hide", (value_cfg_hidden == "1"));
	}
	SystemConf::getInstance()->saveSystemConf();
	delete this;
	// refresh GUI
	ViewController::get()->reloadAll();
}

bool GuiSystemsHide::input(InputConfig* config, Input input)
{
	if (GuiComponent::input(config, input))
		return true;

	if ((config->isMappedTo(BUTTON_BACK, input) || config->isMappedTo("start", input)) && input.value != 0)
	{
		delete this;
		return true;
	}

	return false;
}

std::vector<HelpPrompt> GuiSystemsHide::getHelpPrompts()
{
	std::vector<HelpPrompt> prompts = mMenu.getHelpPrompts();
	prompts.push_back(HelpPrompt(BUTTON_BACK, _("BACK")));
	prompts.push_back(HelpPrompt(BUTTON_OK, _("VALIDATE")));
	prompts.push_back(HelpPrompt("start", _("CLOSE"))); 
	return prompts;
}

