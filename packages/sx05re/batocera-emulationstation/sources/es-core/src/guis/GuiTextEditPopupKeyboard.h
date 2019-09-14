#include "GuiComponent.h"

#include "components/NinePatchComponent.h"
#include "components/ButtonComponent.h"
#include "components/ComponentGrid.h"
#include "components/TextEditComponent.h"
#include "components/TextComponent.h"

class GuiTextEditPopupKeyboard : public GuiComponent
{
public:
	GuiTextEditPopupKeyboard(Window* window, const std::string& title, const std::string& initValue,
		const std::function<void(const std::string&)>& okCallback, bool multiLine, const std::string acceptBtnText = "OK");

	bool input(InputConfig* config, Input input);
	void update(int deltatime) override;
	void onSizeChanged();
	std::vector<HelpPrompt> getHelpPrompts() override;

private:
	class KeyboardButton
	{
	public:
		std::shared_ptr<ButtonComponent> button;
		const std::string key;
		const std::string shiftedKey;
		KeyboardButton(const std::shared_ptr<ButtonComponent> b, const std::string& k, const std::string& sk) : button(b), key(k), shiftedKey(sk) {};
	};
	
	std::shared_ptr<ButtonComponent> makeButton(const std::string& key, const std::string& shiftedKey);
	std::vector<KeyboardButton> keyboardButtons;
	std::shared_ptr<ButtonComponent> mShiftButton;
	const Vector2f getButtonSize();

	void shiftKeys();

	NinePatchComponent mBackground;
	ComponentGrid mGrid;

	std::shared_ptr<TextComponent> mTitle;
	std::shared_ptr<TextEditComponent> mText;
	std::shared_ptr<ComponentGrid> mKeyboardGrid;
	std::shared_ptr<ComponentGrid> mButtons;
	
	bool mMultiLine;
	bool mShift = false;	
};

