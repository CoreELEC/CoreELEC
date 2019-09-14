#pragma once

#include "GuiComponent.h"
#include "components/MenuComponent.h"

template<typename T>
class OptionListComponent;

// Batocera
class GuiBezelInstallStart : public GuiComponent
{
public:
	GuiBezelInstallStart(Window* window);
	bool input(InputConfig* config, Input input) override;
	virtual std::vector<HelpPrompt> getHelpPrompts() override;

private:
	void start();
	MenuComponent mMenu;
	char *mSelectedBezel;
};

class GuiBezelUninstallStart : public GuiComponent
{
public:
	GuiBezelUninstallStart(Window* window);
	bool input(InputConfig* config, Input input) override;
	virtual std::vector<HelpPrompt> getHelpPrompts() override;

private:
	void start();
	MenuComponent mMenu;
	char *mSelectedBezel;
};

class GuiBezelInstallMenu : public GuiComponent
{
public:
	GuiBezelInstallMenu(Window* window);
	bool input(InputConfig* config, Input input) override;
	virtual std::vector<HelpPrompt> getHelpPrompts() override;

private:
	MenuComponent mMenu;
};

