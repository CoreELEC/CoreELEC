#pragma once
#ifndef ES_CORE_GUIS_GUI_MSG_BOX_H
#define ES_CORE_GUIS_GUI_MSG_BOX_H

#include "components/ComponentGrid.h"
#include "components/NinePatchComponent.h"
#include "GuiComponent.h"

class ButtonComponent;
class TextComponent;
class ImageComponent;

enum GuiMsgBoxIcon
{
	ICON_AUTOMATIC,
	ICON_INFORMATION,
	ICON_QUESTION,
	ICON_WARNING,
	ICON_ERROR
};


class GuiMsgBox : public GuiComponent
{
public:
	GuiMsgBox(Window* window, const std::string& text, 
		const std::string& name1, const std::function<void()>& func1,
		const std::string& name2, const std::function<void()>& func2, 
		const std::string& name3, const std::function<void()>& func3, 
		GuiMsgBoxIcon icon = ICON_AUTOMATIC);


	GuiMsgBox(Window* window, const std::string& text,
		const std::string& name1, const std::function<void()>& func1,
		const std::string& name2, const std::function<void()>& func2,
		GuiMsgBoxIcon icon = ICON_AUTOMATIC);

	GuiMsgBox(Window* window, const std::string& text,
		const std::string& name1 = "OK", const std::function<void()>& func1 = nullptr,
		GuiMsgBoxIcon icon = ICON_AUTOMATIC);

	bool input(InputConfig* config, Input input) override;
	void onSizeChanged() override;
	std::vector<HelpPrompt> getHelpPrompts() override;

	std::string getValue() const override { return "GuiMsgBox"; }

private:
	void deleteMeAndCall(const std::function<void()>& func);

	NinePatchComponent mBackground;
	ComponentGrid mGrid;	

	std::shared_ptr<ImageComponent> mImage;
	std::shared_ptr<TextComponent> mMsg;
	std::vector< std::shared_ptr<ButtonComponent> > mButtons;
	std::shared_ptr<ComponentGrid> mButtonGrid;
	std::function<void()> mAcceleratorFunc;
};

#endif // ES_CORE_GUIS_GUI_MSG_BOX_H
