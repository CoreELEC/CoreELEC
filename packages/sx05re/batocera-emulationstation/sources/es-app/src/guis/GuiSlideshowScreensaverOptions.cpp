#include "guis/GuiSlideshowScreensaverOptions.h"

#include "components/SliderComponent.h"
#include "components/SwitchComponent.h"
#include "guis/GuiTextEditPopup.h"
#include "guis/GuiTextEditPopupKeyboard.h"
#include "utils/StringUtil.h"
#include "Settings.h"
#include "Window.h"
#include "LocaleES.h"

GuiSlideshowScreensaverOptions::GuiSlideshowScreensaverOptions(Window* window, const char* title) : GuiScreensaverOptions(window, title)
{
	auto theme = ThemeData::getMenuTheme();

	ComponentListRow row;

	// image duration (seconds)
	auto sss_image_sec = std::make_shared<SliderComponent>(mWindow, 1.f, 60.f, 1.f, "s");
	sss_image_sec->setValue((float)(Settings::getInstance()->getInt("ScreenSaverSwapImageTimeout") / (1000)));
	addWithLabel(row, _("SWAP IMAGE AFTER (SECS)"), sss_image_sec);
	addSaveFunc([sss_image_sec] {
		int playNextTimeout = (int)Math::round(sss_image_sec->getValue()) * (1000);
		Settings::getInstance()->setInt("ScreenSaverSwapImageTimeout", playNextTimeout);
		PowerSaver::updateTimeouts();
	});

	// stretch
	auto sss_stretch = std::make_shared<SwitchComponent>(mWindow);
	sss_stretch->setState(Settings::getInstance()->getBool("SlideshowScreenSaverStretch"));
	addWithLabel(row, _("STRETCH IMAGES"), sss_stretch);
	addSaveFunc([sss_stretch] {
		Settings::getInstance()->setBool("SlideshowScreenSaverStretch", sss_stretch->getState());
	});

	// background audio file
	auto sss_bg_audio_file = std::make_shared<TextComponent>(mWindow, "", theme->TextSmall.font, theme->Text.color);
	addEditableTextComponent(row, _("BACKGROUND AUDIO"), sss_bg_audio_file, Settings::getInstance()->getString("SlideshowScreenSaverBackgroundAudioFile"));
	addSaveFunc([sss_bg_audio_file] {
		Settings::getInstance()->setString("SlideshowScreenSaverBackgroundAudioFile", sss_bg_audio_file->getValue());
	});

	// image source
	auto sss_custom_source = std::make_shared<SwitchComponent>(mWindow);
	sss_custom_source->setState(Settings::getInstance()->getBool("SlideshowScreenSaverCustomImageSource"));
	addWithLabel(row, _("USE CUSTOM IMAGES"), sss_custom_source);
	addSaveFunc([sss_custom_source] { Settings::getInstance()->setBool("SlideshowScreenSaverCustomImageSource", sss_custom_source->getState()); });

	// custom image directory
	auto sss_image_dir = std::make_shared<TextComponent>(mWindow, "", theme->TextSmall.font, theme->Text.color);
	addEditableTextComponent(row, _("CUSTOM IMAGE DIR"), sss_image_dir, Settings::getInstance()->getString("SlideshowScreenSaverImageDir"));
	addSaveFunc([sss_image_dir] {
		Settings::getInstance()->setString("SlideshowScreenSaverImageDir", sss_image_dir->getValue());
	});

	// recurse custom image directory
	auto sss_recurse = std::make_shared<SwitchComponent>(mWindow);
	sss_recurse->setState(Settings::getInstance()->getBool("SlideshowScreenSaverRecurse"));
	addWithLabel(row, _("CUSTOM IMAGE DIR RECURSIVE"), sss_recurse);
	addSaveFunc([sss_recurse] {
		Settings::getInstance()->setBool("SlideshowScreenSaverRecurse", sss_recurse->getState());
	});

	// custom image filter
	auto sss_image_filter = std::make_shared<TextComponent>(mWindow, "", theme->TextSmall.font, theme->Text.color);
	addEditableTextComponent(row, _("CUSTOM IMAGE FILTER"), sss_image_filter, Settings::getInstance()->getString("SlideshowScreenSaverImageFilter"));
	addSaveFunc([sss_image_filter] {
		Settings::getInstance()->setString("SlideshowScreenSaverImageFilter", sss_image_filter->getValue());
	});
}

GuiSlideshowScreensaverOptions::~GuiSlideshowScreensaverOptions()
{
}

void GuiSlideshowScreensaverOptions::addWithLabel(ComponentListRow row, const std::string label, std::shared_ptr<GuiComponent> component)
{
	auto theme = ThemeData::getMenuTheme();

	row.elements.clear();

	auto lbl = std::make_shared<TextComponent>(mWindow, Utils::String::toUpper(label), theme->Text.font, theme->Text.color);
	row.addElement(lbl, true); // label

	row.addElement(component, false, true);

	addRow(row);
}

void GuiSlideshowScreensaverOptions::addEditableTextComponent(ComponentListRow row, const std::string label, std::shared_ptr<GuiComponent> ed, std::string value)
{
	auto theme = ThemeData::getMenuTheme();
	row.elements.clear();

	auto lbl = std::make_shared<TextComponent>(mWindow, Utils::String::toUpper(label), theme->Text.font, theme->Text.color);
	row.addElement(lbl, true); // label

	row.addElement(ed, true);

	auto spacer = std::make_shared<GuiComponent>(mWindow);
	spacer->setSize(Renderer::getScreenWidth() * 0.005f, 0);
	row.addElement(spacer, false);

	auto bracket = std::make_shared<ImageComponent>(mWindow);
	bracket->setImage(ThemeData::getMenuTheme()->Icons.arrow);
	bracket->setResize(Vector2f(0, lbl->getFont()->getLetterHeight()));
	row.addElement(bracket, false);

	auto updateVal = [ed](const std::string& newVal) { ed->setValue(newVal); }; // ok callback (apply new value to ed)
	row.makeAcceptInputHandler([this, label, ed, updateVal] {
	    if (Settings::getInstance()->getBool("UseOSK")) {
	      mWindow->pushGui(new GuiTextEditPopupKeyboard(mWindow, label, ed->getValue(), updateVal, false));
	    } else {
	      mWindow->pushGui(new GuiTextEditPopup(mWindow, label, ed->getValue(), updateVal, false));
	    }
	});

	assert(ed);
	addRow(row);
	ed->setValue(value);
}
