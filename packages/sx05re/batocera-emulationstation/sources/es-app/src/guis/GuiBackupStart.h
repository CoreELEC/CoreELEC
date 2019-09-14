#pragma once

#include "GuiComponent.h"
#include "components/MenuComponent.h"

template<typename T>
class OptionListComponent;

class GuiBackupStart : public GuiComponent
{
public:
	GuiBackupStart(Window* window);
	bool input(InputConfig* config, Input input) override;

	virtual std::vector<HelpPrompt> getHelpPrompts() override;

private:
	void start();

	MenuComponent mMenu;
	std::shared_ptr< OptionListComponent<std::string> >moptionsStorage;
};
