#pragma once

#include "GuiComponent.h"
#include "components/MenuComponent.h"

template<typename T>
class OptionListComponent;

class GuiInstallStart : public GuiComponent
{
public:
	GuiInstallStart(Window* window);
	bool input(InputConfig* config, Input input) override;

	virtual std::vector<HelpPrompt> getHelpPrompts() override;

private:
	void start();

	MenuComponent mMenu;
	std::shared_ptr< OptionListComponent<std::string> >moptionsStorage;
	std::shared_ptr< OptionListComponent<std::string> >moptionsArchitecture;
	std::shared_ptr< OptionListComponent<std::string> >moptionsValidation;
};
