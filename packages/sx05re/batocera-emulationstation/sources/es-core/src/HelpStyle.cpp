#include "HelpStyle.h"

#include "resources/Font.h"

HelpStyle::HelpStyle()
{
	position = Vector2f(Renderer::getScreenWidth() * 0.012f, Renderer::getScreenHeight() * 0.9515f);
	origin = Vector2f(0.0f, 0.0f);
	iconColor = 0x777777FF;
	textColor = 0x777777FF;

	clockPosition = Vector2f(-1, -1);
	clockColor = 0x777777FF;

	if (FONT_SIZE_SMALL != 0)
	{
		font = Font::get(FONT_SIZE_SMALL);
		clockFont = Font::get(FONT_SIZE_SMALL);
	}
	else
	{
		font = nullptr;
		clockFont = nullptr;
	}
}

void HelpStyle::applyTheme(const std::shared_ptr<ThemeData>& theme, const std::string& view)
{
	auto elem = theme->getElement(view, "help", "helpsystem");
	if (elem)
	{
		if (elem->has("pos"))
			position = elem->get<Vector2f>("pos") * Vector2f((float)Renderer::getScreenWidth(), (float)Renderer::getScreenHeight());

		if (elem->has("origin"))
			origin = elem->get<Vector2f>("origin");

		if (elem->has("textColor"))
		{
			textColor = elem->get<unsigned int>("textColor");
			clockColor = textColor;
		}

		if (elem->has("iconColor"))
			iconColor = elem->get<unsigned int>("iconColor");

		if (elem->has("fontPath") || elem->has("fontSize"))
		{
			font = Font::getFromTheme(elem, ThemeFlags::ALL, font);
			clockFont = font;
		}

		if (elem->has("iconUpDown"))
			iconMap["up/down"] = elem->get<std::string>("iconUpDown");

		if (elem->has("iconLeftRight"))
			iconMap["left/right"] = elem->get<std::string>("iconLeftRight");

		if (elem->has("iconUpDownLeftRight"))
			iconMap["up/down/left/right"] = elem->get<std::string>("iconUpDownLeftRight");

		if (elem->has("iconA"))
			iconMap["a"] = elem->get<std::string>("iconA");

		if (elem->has("iconB"))
			iconMap["b"] = elem->get<std::string>("iconB");

		if (elem->has("iconX"))
			iconMap["x"] = elem->get<std::string>("iconX");

		if (elem->has("iconY"))
			iconMap["y"] = elem->get<std::string>("iconY");

		if (elem->has("iconL"))
			iconMap["l"] = elem->get<std::string>("iconL");

		if (elem->has("iconR"))
			iconMap["r"] = elem->get<std::string>("iconR");

		if (elem->has("iconStart"))
			iconMap["start"] = elem->get<std::string>("iconStart");

		if (elem->has("iconSelect"))
			iconMap["select"] = elem->get<std::string>("iconSelect");
	}

	elem = theme->getElement(view, "clock", "clock");
	if (elem)
	{
		if (elem->has("pos"))
			clockPosition = elem->get<Vector2f>("pos") * Vector2f((float)Renderer::getScreenWidth(), (float)Renderer::getScreenHeight());
		else
			clockPosition = Vector2f(-1, -1);

		if (elem->has("fontPath") || elem->has("fontSize"))
			clockFont = Font::getFromTheme(elem, ThemeFlags::ALL, font);

		if (elem->has("textColor"))
			clockColor = elem->get<unsigned int>("textColor");
	}
	else
		clockPosition = Vector2f(-1, -1);
}
