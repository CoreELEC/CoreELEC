#pragma once
#ifndef ES_CORE_HELP_STYLE_H
#define ES_CORE_HELP_STYLE_H

#include "math/Vector2f.h"
#include <memory>
#include <string>
#include <map>

class Font;
class ThemeData;

struct HelpStyle
{
	Vector2f position;
	Vector2f origin;
	unsigned int iconColor;
	unsigned int textColor;
	std::shared_ptr<Font> font;
	std::map<std::string, std::string> iconMap;

	Vector2f clockPosition;
	std::shared_ptr<Font> clockFont;
	unsigned int clockColor;

	HelpStyle(); // default values
	void applyTheme(const std::shared_ptr<ThemeData>& theme, const std::string& view);
};

#endif // ES_CORE_HELP_STYLE_H
