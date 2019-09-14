#include "LocaleES.h"
#include "guis/GuiInputConfig.h"

#include "components/ButtonComponent.h"
#include "components/MenuComponent.h"
#include "guis/GuiMsgBox.h"
#include "InputManager.h"
#include "Log.h"
#include "Window.h"

struct InputConfigStructure
{
	const char* name;
	const bool  skippable;
	const char* dispName;
	const char* icon;
};

static const int inputCount = 21; // batocera
static const InputConfigStructure GUI_INPUT_CONFIG_LIST[inputCount] =
{
  // batocera
  { "up",               false, "UP",           ":/help/dpad_up.svg" },
  { "down",             false, "DOWN",         ":/help/dpad_down.svg" },
  { "left",             false, "LEFT",         ":/help/dpad_left.svg" },
  { "right",            false, "RIGHT",        ":/help/dpad_right.svg" },
  { "start",            true,  "START",              ":/help/button_start.svg" },
  { "select",           true,  "SELECT",             ":/help/button_select.svg" },
  { "a",                false, "A",    ":/help/buttons_east.svg" },
  { "b",                true,  "B",   ":/help/buttons_south.svg" },
  { "x",                true,  "X",   ":/help/buttons_north.svg" },
  { "y",                true,  "Y",    ":/help/buttons_west.svg" },
  //{ "LeftShoulder",     true,  "LEFT SHOULDER",      ":/help/button_l.svg" },
  //{ "RightShoulder",    true,  "RIGHT SHOULDER",     ":/help/button_r.svg" },
  //{ "LeftTrigger",      true,  "LEFT TRIGGER",       ":/help/button_lt.svg" },
  //{ "RightTrigger",     true,  "RIGHT TRIGGER",      ":/help/button_rt.svg" },
  //{ "LeftThumb",        true,  "LEFT THUMB",         ":/help/analog_thumb.svg" },
  //{ "RightThumb",       true,  "RIGHT THUMB",        ":/help/analog_thumb.svg" },
  //{ "LeftAnalogUp",     true,  "LEFT ANALOG UP",     ":/help/analog_up.svg" },
  //{ "LeftAnalogDown",   true,  "LEFT ANALOG DOWN",   ":/help/analog_down.svg" },
  //{ "LeftAnalogLeft",   true,  "LEFT ANALOG LEFT",   ":/help/analog_left.svg" },
  //{ "LeftAnalogRight",  true,  "LEFT ANALOG RIGHT",  ":/help/analog_right.svg" },
  //{ "RightAnalogUp",    true,  "RIGHT ANALOG UP",    ":/help/analog_up.svg" },
  //{ "RightAnalogDown",  true,  "RIGHT ANALOG DOWN",  ":/help/analog_down.svg" },
  //{ "RightAnalogLeft",  true,  "RIGHT ANALOG LEFT",  ":/help/analog_left.svg" },
  //{ "RightAnalogRight", true,  "RIGHT ANALOG RIGHT", ":/help/analog_right.svg" },
  //{ "HotKeyEnable",     true,  "HOTKEY ENABLE",      ":/help/button_hotkey.svg" }
  
  { "joystick1up",     true,  "LEFT ANALOG UP",     ":/help/analog_up.svg" },
  { "joystick1left",   true,  "LEFT ANALOG LEFT",   ":/help/analog_left.svg" },
  { "joystick2up",     true,  "RIGHT ANALOG UP",     ":/help/analog_up.svg" },
  { "joystick2left",   true,  "RIGHT ANALOG LEFT",   ":/help/analog_left.svg" },
  { "pageup",          true,  "L1",      ":/help/button_l.svg" },
  { "pagedown",        true,  "R1",     ":/help/button_r.svg" },
  { "l2",              true,  "L2",       ":/help/button_lt.svg" },
  { "r2",              true,  "R2",      ":/help/button_rt.svg" },
  { "l3",              true,  "L3",       ":/help/button_lt.svg" },
  { "r3",              true,  "R3",      ":/help/button_rt.svg" },
  { "hotkey",          true,  "HOTKEY",      ":/help/button_hotkey.svg" } // batocera
};

#define fake_gettext_up _("UP")
#define fake_gettext_down _("DOWN")
#define fake_gettext_left _("LEFT")
#define fake_gettext_right _("RIGHT")
#define fake_gettext_start _("START")
#define fake_gettext_select _("SELECT")
#define fake_gettext_left_a_up _("LEFT ANALOG UP")
#define fake_gettext_left_a_left _("LEFT ANALOG LEFT")
#define fake_gettext_right_a_up _("RIGHT ANALOG UP")
#define fake_gettext_right_a_left _("RIGHT ANALOG LEFT")
#define fake_gettext_hotkey _("HOTKEY")

//MasterVolUp and MasterVolDown are also hooked up, but do not appear on this screen.
//If you want, you can manually add them to es_input.cfg.

#define HOLD_TO_SKIP_MS 1000

GuiInputConfig::GuiInputConfig(Window* window, InputConfig* target, bool reconfigureAll, const std::function<void()>& okCallback) : GuiComponent(window), 
	mBackground(window, ":/frame.png"), mGrid(window, Vector2i(1, 7)), 
	mTargetConfig(target), mHoldingInput(false), mBusyAnim(window)
{
	auto theme = ThemeData::getMenuTheme();
	mBackground.setImagePath(theme->Background.path);
	mBackground.setCenterColor(theme->Background.color);
	mBackground.setEdgeColor(theme->Background.color);
	mGrid.setSeparatorColor(theme->Text.separatorColor);

	LOG(LogInfo) << "Configuring device " << target->getDeviceId() << " (" << target->getDeviceName() << ").";

	if(reconfigureAll)
		target->clear();

	mConfiguringAll = reconfigureAll;
	mConfiguringRow = mConfiguringAll;

	addChild(&mBackground);
	addChild(&mGrid);

	// 0 is a spacer row
	mGrid.setEntry(std::make_shared<GuiComponent>(mWindow), Vector2i(0, 0), false);

	mTitle = std::make_shared<TextComponent>(mWindow, _("CONFIGURING"), theme->Title.font, theme->Title.color, ALIGN_CENTER); // batocera
	mGrid.setEntry(mTitle, Vector2i(0, 1), false, true);

	char strbuf[256];
	if(target->getDeviceId() == DEVICE_KEYBOARD)
	  strncpy(strbuf, _("KEYBOARD").c_str(), 256); // batocera
	else if(target->getDeviceId() == DEVICE_CEC)
	  strncpy(strbuf, _("CEC").c_str(), 256); // batocera
	else {
	  snprintf(strbuf, 256, _("GAMEPAD %i").c_str(), target->getDeviceId() + 1); // batocera
	}
	mSubtitle1 = std::make_shared<TextComponent>(mWindow, Utils::String::toUpper(strbuf), theme->Text.font, theme->Title.color, ALIGN_CENTER); // batocera
	mGrid.setEntry(mSubtitle1, Vector2i(0, 2), false, true);

	mSubtitle2 = std::make_shared<TextComponent>(mWindow, _("HOLD ANY BUTTON TO SKIP"), theme->TextSmall.font, theme->TextSmall.color, ALIGN_CENTER); // batocera
	mGrid.setEntry(mSubtitle2, Vector2i(0, 3), false, true);

	// 4 is a spacer row

	mList = std::make_shared<ComponentList>(mWindow);
	mGrid.setEntry(mList, Vector2i(0, 5), true, true);
	for(int i = 0; i < inputCount; i++)
	{
		ComponentListRow row;
		
		// icon
		auto icon = std::make_shared<ImageComponent>(mWindow);
		icon->setImage(GUI_INPUT_CONFIG_LIST[i].icon);
		icon->setColorShift(theme->Text.color);
		icon->setResize(0, Font::get(FONT_SIZE_MEDIUM)->getLetterHeight() * 1.25f);
		row.addElement(icon, false);

		// spacer between icon and text
		auto spacer = std::make_shared<GuiComponent>(mWindow);
		spacer->setSize(16, 0);
		row.addElement(spacer, false);

		auto text = std::make_shared<TextComponent>(mWindow, _(GUI_INPUT_CONFIG_LIST[i].dispName), theme->Text.font, theme->Text.color);
		row.addElement(text, true);

		auto mapping = std::make_shared<TextComponent>(mWindow, _("-NOT DEFINED-"), theme->Text.font, theme->TextSmall.color, ALIGN_RIGHT); // batocera
		setNotDefined(mapping); // overrides text and color set above
		row.addElement(mapping, true);
		mMappings.push_back(mapping);

		row.input_handler = [this, i, mapping](InputConfig* config, Input input) -> bool
		{
			// ignore input not from our target device
			if(config != mTargetConfig)
				return false;

			// if we're not configuring, start configuring when A is pressed
			if(!mConfiguringRow)
			{
				if(config->isMappedTo(BUTTON_OK, input) && input.value)
				{
					mList->stopScrolling();
					mConfiguringRow = true;
					setPress(mapping);
					return true;
				}
				
				// we're not configuring and they didn't press A to start, so ignore this
				return false;
			}


			// filter for input quirks specific to Sony DualShock 3
			if(filterTrigger(input, config))
				return false;

			// we are configuring, the button is unpressed or the axis is relaxed
			if(input.value != 0)
			{
				// input down
				// if we're already holding something, ignore this, otherwise plan to map this input
				if(mHoldingInput)
					return true;

				mHoldingInput = true;
				mHeldInput = input;
				mHeldTime = 0;
				mHeldInputId = i;

				return true;
			}else{
				// input up
				// make sure we were holding something and we let go of what we were previously holding
				if(!mHoldingInput || mHeldInput.device != input.device || mHeldInput.id != input.id || mHeldInput.type != input.type)
					return true;

				mHoldingInput = false;

				if(assign(mHeldInput, i))
					rowDone(); // if successful, move cursor/stop configuring - if not, we'll just try again

				return true;
			}
		};

		mList->addRow(row);
	}

	// only show "HOLD TO SKIP" if this input is skippable
	mList->setCursorChangedCallback([this](CursorState /*state*/) {
		bool skippable = GUI_INPUT_CONFIG_LIST[mList->getCursorId()].skippable;
		mSubtitle2->setOpacity(skippable * 255);
	});

	// make the first one say "PRESS ANYTHING" if we're re-configuring everything
	if(mConfiguringAll)
		setPress(mMappings.front());

	// buttons
	std::vector< std::shared_ptr<ButtonComponent> > buttons;
	std::function<void()> okFunction = [this, okCallback] {
		InputManager::getInstance()->writeDeviceConfig(mTargetConfig); // save
		if(okCallback)
			okCallback();
		delete this; 
	};
	buttons.push_back(std::make_shared<ButtonComponent>(mWindow, _("OK"), "ok", [this, okFunction] { // batocera
		// check if the hotkey enable button is set. if not prompt the user to use select or nothing.
		Input input;
		if (!mTargetConfig->getInputByName("hotkey", &input)) { // batocera
			mWindow->pushGui(new GuiMsgBox(mWindow,
				_("YOU DIDN'T CHOOSE A HOTKEY ENABLE BUTTON. THIS IS REQUIRED FOR EXITING GAMES WITH A CONTROLLER. DO YOU WANT TO USE THE SELECT BUTTON DEFAULT ? PLEASE ANSWER YES TO USE SELECT OR NO TO NOT SET A HOTKEY ENABLE BUTTON."),  // batocera
				_("YES"), [this, okFunction] { // batocera
					Input input;
					mTargetConfig->getInputByName("Select", &input);
					mTargetConfig->mapInput("hotkey", input); // batocera
					okFunction();
					},
				_("NO"), [this, okFunction] { // batocera
					// for a disabled hotkey enable button, set to a key with id 0,
					// so the input configuration script can be backwards compatible.
					mTargetConfig->mapInput("hotkey", Input(DEVICE_KEYBOARD, TYPE_KEY, 0, 1, true)); // batocera
					okFunction();
				}
			));
		} else {
			okFunction();
		}
	}));
	mButtonGrid = makeButtonGrid(mWindow, buttons);
	mGrid.setEntry(mButtonGrid, Vector2i(0, 6), true, false);

	setSize(Renderer::getScreenWidth() * 0.6f, Renderer::getScreenHeight() * 0.75f);
	setPosition((Renderer::getScreenWidth() - mSize.x()) / 2, (Renderer::getScreenHeight() - mSize.y()) / 2);
}

void GuiInputConfig::onSizeChanged()
{
	mBackground.fitTo(mSize, Vector3f::Zero(), Vector2f(-32, -32));

	// update grid
	mGrid.setSize(mSize);
	
	float h = (mTitle->getFont()->getHeight() + // *0.75f
		mSubtitle1->getFont()->getHeight() +
		mSubtitle2->getFont()->getHeight() +
		0.03f +
		mButtonGrid->getSize().y()) / mSize.y();

	int cnt = (1.0 - h) / (mList->getRowHeight(0) / mSize.y());

	mGrid.setRowHeightPerc(1, mTitle->getFont()->getHeight() / mSize.y()); // *0.75f
	mGrid.setRowHeightPerc(2, mSubtitle1->getFont()->getHeight() / mSize.y());
	mGrid.setRowHeightPerc(3, mSubtitle2->getFont()->getHeight() / mSize.y());
	//mGrid.setRowHeightPerc(4, 0.03f);
	mGrid.setRowHeightPerc(5, (mList->getRowHeight(0) * cnt + 2) / mSize.y());
	mGrid.setRowHeightPerc(6, mButtonGrid->getSize().y() / mSize.y());

	mBusyAnim.setSize(mSize);
}

void GuiInputConfig::update(int deltaTime)
{
	if(mConfiguringRow && mHoldingInput && GUI_INPUT_CONFIG_LIST[mHeldInputId].skippable)
	{
		int prevSec = mHeldTime / 1000;
		mHeldTime += deltaTime;
		int curSec = mHeldTime / 1000;

		if(mHeldTime >= HOLD_TO_SKIP_MS)
		{
			setNotDefined(mMappings.at(mHeldInputId));
			clearAssignment(mHeldInputId);
			mHoldingInput = false;
			rowDone();
		}else{
			if(prevSec != curSec)
			{
				// crossed the second boundary, update text
				const auto& text = mMappings.at(mHeldInputId);
				char strbuf[256];
				snprintf(strbuf, 256, ngettext("HOLD FOR %iS TO SKIP", "HOLD FOR %iS TO SKIP", HOLD_TO_SKIP_MS/1000 - curSec).c_str(), HOLD_TO_SKIP_MS/1000 - curSec); // batocera
				text->setText(strbuf);
				text->setColor(ThemeData::getMenuTheme()->Text.color);
			}
		}
	}
}

// move cursor to the next thing if we're configuring all, 
// or come out of "configure mode" if we were only configuring one row
void GuiInputConfig::rowDone()
{
	if(mConfiguringAll)
	{
		if(!mList->moveCursor(1)) // try to move to the next one
		{
			// at bottom of list, done
			mConfiguringAll = false;
			mConfiguringRow = false;
			mGrid.moveCursor(Vector2i(0, 1));
		}else{
			// on another one
			setPress(mMappings.at(mList->getCursorId()));
		}
	}else{
		// only configuring one row, so stop
		mConfiguringRow = false;
	}
}

void GuiInputConfig::setPress(const std::shared_ptr<TextComponent>& text)
{
  text->setText(_("PRESS ANYTHING")); // batocera
	text->setColor(0x656565FF);
}

void GuiInputConfig::setNotDefined(const std::shared_ptr<TextComponent>& text)
{
  text->setText(_("-NOT DEFINED-")); // batocera
	text->setColor(0x999999FF);
}

void GuiInputConfig::setAssignedTo(const std::shared_ptr<TextComponent>& text, Input input)
{
	text->setText(Utils::String::toUpper(input.string()));
	text->setColor(0x777777FF);
}

void GuiInputConfig::error(const std::shared_ptr<TextComponent>& text, const std::string& /*msg*/)
{
  text->setText(_("ALREADY TAKEN")); // batocera
	text->setColor(0x656565FF);
}

bool GuiInputConfig::assign(Input input, int inputId)
{
	// input is from InputConfig* mTargetConfig

	// if this input is mapped to something other than "nothing" or the current row, error
	// (if it's the same as what it was before, allow it)
	if(mTargetConfig->getMappedTo(input).size() > 0 && !mTargetConfig->isMappedTo(GUI_INPUT_CONFIG_LIST[inputId].name, input) && strcmp(GUI_INPUT_CONFIG_LIST[inputId].name, "hotkey") != 0) // batocera
	{
		error(mMappings.at(inputId), "Already mapped!");
		return false;
	}

	setAssignedTo(mMappings.at(inputId), input);
	
	input.configured = true;
	mTargetConfig->mapInput(GUI_INPUT_CONFIG_LIST[inputId].name, input);

	LOG(LogInfo) << "  Mapping [" << input.string() << "] -> " << GUI_INPUT_CONFIG_LIST[inputId].name;

	return true;
}

void GuiInputConfig::clearAssignment(int inputId)
{
	mTargetConfig->unmapInput(GUI_INPUT_CONFIG_LIST[inputId].name);
}

bool GuiInputConfig::filterTrigger(Input input, InputConfig* config)
{
  // batocera
  return false;

#if defined(__linux__)
	// on Linux, some gamepads return both an analog axis and a digital button for the trigger;
	// we want the analog axis only, so this function removes the button press event

	if((
	  // match PlayStation joystick with 6 axes only
	  strstr(config->getDeviceName().c_str(), "PLAYSTATION") != NULL
	  || strstr(config->getDeviceName().c_str(), "PS3 Ga") != NULL
	  || strstr(config->getDeviceName().c_str(), "PS(R) Ga") != NULL
	  // BigBen kid's PS3 gamepad 146b:0902, matched on SDL GUID because its name "Bigben Interactive Bigben Game Pad" may be too generic
	  || strcmp(config->getDeviceGUIDString().c_str(), "030000006b1400000209000011010000") == 0
	  ) && InputManager::getInstance()->getAxisCountByDevice(config->getDeviceId()) == 6)
	{
		// digital triggers are unwanted
		if (input.type == TYPE_BUTTON && (input.id == 6 || input.id == 7))
			return true;
		// ignore analog values < 0
		if (input.type == TYPE_AXIS && (input.id == 2 || input.id == 5) && input.value < 0)
			return true;
	}
#endif

	return false;
}
