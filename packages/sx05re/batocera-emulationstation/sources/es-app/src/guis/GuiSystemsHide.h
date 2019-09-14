#pragma once
#ifndef ES_APP_GUIS_GUI_SYSTEMS_HIDE_H
#define ES_APP_GUIS_GUI_SYSTEMS_HIDE_H

#include "components/MenuComponent.h"

// class FileData;
template<typename T>
class OptionListComponent;
class SwitchComponent;
class SystemData;

class GuiSystemsHide : public GuiComponent
{
public:
	GuiSystemsHide(Window* window);
	virtual std::vector<HelpPrompt> getHelpPrompts() override;
	bool input(InputConfig* config, Input input);

private:
	void Apply();

	MenuComponent mMenu;
	std::shared_ptr< OptionListComponent<SystemData*> > mSystems;
};

#endif // ES_APP_GUIS_GUI_SYSTEMS_HIDE_H
