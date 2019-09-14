#include "ThemeData.h"

#include "components/ImageComponent.h"
#include "components/TextComponent.h"
#include "components/NinePatchComponent.h"
#include "components/VideoVlcComponent.h"
#include "utils/FileSystemUtil.h"
#include "utils/StringUtil.h"
#include "Log.h"
#include "platform.h"
#include "Settings.h"
#include <algorithm>

std::vector<std::string> ThemeData::sSupportedViews { { "system" }, { "basic" }, { "detailed" }, { "grid" }, { "video" }, { "menu" } };
std::vector<std::string> ThemeData::sSupportedFeatures { { "video" }, { "carousel" }, { "z-index" }, { "visible" } };

std::map<std::string, std::map<std::string, ThemeData::ElementPropertyType>> ThemeData::sElementMap {
	{ "image", {
		{ "pos", NORMALIZED_PAIR },
		{ "size", NORMALIZED_PAIR },
		{ "maxSize", NORMALIZED_PAIR },
		{ "minSize", NORMALIZED_PAIR },
		{ "origin", NORMALIZED_PAIR },
	 	{ "rotation", FLOAT },
		{ "rotationOrigin", NORMALIZED_PAIR },
		{ "path", PATH },
		{ "default", PATH },
		{ "tile", BOOLEAN },
		{ "color", COLOR },
		{ "colorEnd", COLOR },
		{ "gradientType", STRING },
		{ "visible", BOOLEAN },
		{ "reflexion", NORMALIZED_PAIR },
		{ "zIndex", FLOAT } } },
	{ "imagegrid", {
		{ "pos", NORMALIZED_PAIR },
		{ "size", NORMALIZED_PAIR },
		{ "margin", NORMALIZED_PAIR },
		{ "padding", NORMALIZED_RECT },
		{ "autoLayout", NORMALIZED_PAIR },
		{ "autoLayoutSelectedZoom", FLOAT },
		{ "animateSelection", BOOLEAN },
		{ "imageSource", STRING }, // image, thumbnail, marquee
		{ "zIndex", FLOAT },
		{ "gameImage", PATH },
		{ "folderImage", PATH },
		{ "showVideoAtDelay", FLOAT },
		{ "scrollDirection", STRING },
		{ "centerSelection", BOOLEAN },
		{ "scrollLoop", BOOLEAN } } },
	{ "gridtile", {
		{ "size", NORMALIZED_PAIR },
		{ "padding", NORMALIZED_PAIR },
		{ "imageColor", COLOR },
		{ "backgroundImage", PATH },
		{ "backgroundCornerSize", NORMALIZED_PAIR },
		{ "backgroundColor", COLOR },
		{ "backgroundCenterColor", COLOR },
		{ "backgroundEdgeColor", COLOR },
		{ "selectionMode", STRING },
		{ "reflexion", NORMALIZED_PAIR },
		{ "imageSizeMode", STRING } } },
	{ "text", {
		{ "pos", NORMALIZED_PAIR },
		{ "size", NORMALIZED_PAIR },
		{ "origin", NORMALIZED_PAIR },
		{ "rotation", FLOAT },
		{ "rotationOrigin", NORMALIZED_PAIR },
		{ "text", STRING },
		{ "backgroundColor", COLOR },
		{ "fontPath", PATH },
		{ "fontSize", FLOAT },
		{ "color", COLOR },
		{ "alignment", STRING },
		{ "forceUppercase", BOOLEAN },
		{ "lineSpacing", FLOAT },
		{ "value", STRING },
		{ "glowColor", COLOR },
		{ "glowSize", FLOAT },
		{ "glowOffset", NORMALIZED_PAIR },
		{ "padding", NORMALIZED_RECT },
		{ "visible", BOOLEAN },
		{ "zIndex", FLOAT } } },
	{ "textlist", {
		{ "pos", NORMALIZED_PAIR },
		{ "size", NORMALIZED_PAIR },
		{ "origin", NORMALIZED_PAIR },
		{ "selectorHeight", FLOAT },
		{ "selectorOffsetY", FLOAT },
		{ "selectorColor", COLOR },
		{ "selectorColorEnd", COLOR },
		{ "selectorGradientType", STRING },
		{ "selectorImagePath", PATH },
		{ "selectorImageTile", BOOLEAN },
		{ "selectedColor", COLOR },
		{ "primaryColor", COLOR },
		{ "secondaryColor", COLOR },
		{ "fontPath", PATH },
		{ "fontSize", FLOAT },
		{ "scrollSound", PATH },
		{ "alignment", STRING },
		{ "horizontalMargin", FLOAT },
		{ "forceUppercase", BOOLEAN },
		{ "lineSpacing", FLOAT },
		{ "zIndex", FLOAT } } },
	{ "container", {
		{ "pos", NORMALIZED_PAIR },
		{ "size", NORMALIZED_PAIR },
	 	{ "origin", NORMALIZED_PAIR },
	 	{ "visible", BOOLEAN },
	 	{ "zIndex", FLOAT } } },
	{ "ninepatch", {
		{ "pos", NORMALIZED_PAIR },
		{ "size", NORMALIZED_PAIR },
		{ "path", PATH },
	 	{ "visible", BOOLEAN },
		{ "color", COLOR },
		{ "cornerSize", NORMALIZED_PAIR },
		{ "centerColor", COLOR },
		{ "edgeColor", COLOR },
		{ "zIndex", FLOAT } } },
	{ "datetime", {
		{ "pos", NORMALIZED_PAIR },
		{ "size", NORMALIZED_PAIR },
		{ "origin", NORMALIZED_PAIR },
		{ "rotation", FLOAT },
		{ "rotationOrigin", NORMALIZED_PAIR },
		{ "backgroundColor", COLOR },
		{ "fontPath", PATH },
		{ "fontSize", FLOAT },
		{ "color", COLOR },
		{ "alignment", STRING },
		{ "forceUppercase", BOOLEAN },
		{ "lineSpacing", FLOAT },
		{ "value", STRING },
		{ "format", STRING },
		{ "displayRelative", BOOLEAN },
	 	{ "visible", BOOLEAN },
	 	{ "zIndex", FLOAT } } },
	{ "rating", {
		{ "pos", NORMALIZED_PAIR },
		{ "size", NORMALIZED_PAIR },
		{ "origin", NORMALIZED_PAIR },
		{ "rotation", FLOAT },
		{ "rotationOrigin", NORMALIZED_PAIR },
		{ "color", COLOR },
		{ "filledPath", PATH },
		{ "unfilledPath", PATH },
		{ "visible", BOOLEAN },
		{ "zIndex", FLOAT } } },
	{ "sound", {
		{ "path", PATH } } },
	{ "clock", {
		{ "pos", NORMALIZED_PAIR },
		{ "fontPath", PATH },
		{ "fontSize", FLOAT },
		{ "textColor", COLOR } } },
	{ "helpsystem", {
		{ "pos", NORMALIZED_PAIR },
		{ "origin", NORMALIZED_PAIR },
		{ "textColor", COLOR },
		{ "iconColor", COLOR },
		{ "fontPath", PATH },
		{ "fontSize", FLOAT },
		{ "iconUpDown", PATH },
		{ "iconLeftRight", PATH },
		{ "iconUpDownLeftRight", PATH },
		{ "iconA", PATH },
		{ "iconB", PATH },
		{ "iconX", PATH },
		{ "iconY", PATH },
		{ "iconL", PATH },
		{ "iconR", PATH },
		{ "iconStart", PATH },
		{ "iconSelect", PATH } } },
	{ "video", {
		{ "pos", NORMALIZED_PAIR },
		{ "size", NORMALIZED_PAIR },
		{ "maxSize", NORMALIZED_PAIR },
		{ "minSize", NORMALIZED_PAIR },
		{ "origin", NORMALIZED_PAIR },
		{ "rotation", FLOAT },
		{ "rotationOrigin", NORMALIZED_PAIR },
		{ "default", PATH },
		{ "path", PATH },
		{ "delay", FLOAT },
	 	{ "visible", BOOLEAN },
	 	{ "zIndex", FLOAT },
		{ "showSnapshotNoVideo", BOOLEAN },
		{ "showSnapshotDelay", BOOLEAN } } },
	{ "carousel", {
		{ "type", STRING },
		{ "size", NORMALIZED_PAIR },
		{ "pos", NORMALIZED_PAIR },
		{ "origin", NORMALIZED_PAIR },
		{ "color", COLOR },
		{ "colorEnd", COLOR },
		{ "gradientType", STRING },
		{ "logoScale", FLOAT },
		{ "logoRotation", FLOAT },
		{ "logoRotationOrigin", NORMALIZED_PAIR },
		{ "logoSize", NORMALIZED_PAIR },
		{ "logoPos", NORMALIZED_PAIR },
		{ "logoAlignment", STRING },
		{ "maxLogoCount", FLOAT },
		{ "zIndex", FLOAT } } },
	{ "menuText", {
		{ "fontPath", PATH },
		{ "fontSize", FLOAT },
		{ "separatorColor", COLOR },
		{ "selectorColor", COLOR },
		{ "selectorColorEnd", COLOR },
		{ "selectorGradientType", STRING },
		{ "selectedColor", COLOR },
		{ "color", COLOR } } },
	{ "menuTextSmall", {
		{ "fontPath", PATH },
		{ "fontSize", FLOAT },
		{ "color", COLOR } } },
	{ "menuBackground", {
		{ "path", PATH },
		{ "fadePath", PATH },
		{ "color", COLOR } } },
	{ "menuIcons", { 		
		{ "iconSystem", PATH },
		{ "iconUpdates", PATH },
		{ "iconControllers", PATH },
		{ "iconGames", PATH },
		{ "iconUI", PATH },
		{ "iconSound", PATH },
		{ "iconNetwork", PATH },
		{ "iconScraper", PATH },
		{ "iconAdvanced", PATH },
		{ "iconQuit", PATH } } },
	{ "menuSwitch",{
		{ "pathOn", PATH },
		{ "pathOff", PATH } } },
	{ "menuSlider",{
		{ "path", PATH } } },
	{ "menuButton",{
		{ "path", PATH },
		{ "filledPath", PATH } } },
};

std::shared_ptr<ThemeData::ThemeMenu> ThemeData::mMenuTheme;
ThemeData* ThemeData::mDefaultTheme = nullptr;

#define MINIMUM_THEME_FORMAT_VERSION 3
#define CURRENT_THEME_FORMAT_VERSION 6

// helper
unsigned int getHexColor(const char* str)
{
	ThemeException error;
	if (!str)
	{
		//throw error << "Empty color";
		LOG(LogWarning) << "Empty color";
		return 0;
	}

	size_t len = strlen(str);
	if(len != 6 && len != 8)
	{
		//throw error << "Invalid color (bad length, \"" << str << "\" - must be 6 or 8)";
		LOG(LogWarning) << "Invalid color (bad length, \"" << str << "\" - must be 6 or 8)";
		return 0;
	}

	unsigned int val;
	std::stringstream ss;
	ss << str;
	ss >> std::hex >> val;

	if(len == 6)
		val = (val << 8) | 0xFF;

	return val;
}

std::string ThemeData::resolvePlaceholders(const char* in)
{
	std::string inStr(in);

	if(inStr.empty())
		return inStr;

	const size_t variableBegin = inStr.find("${");
	const size_t variableEnd   = inStr.find("}", variableBegin);

	if((variableBegin == std::string::npos) || (variableEnd == std::string::npos))
		return inStr;

	std::string prefix  = inStr.substr(0, variableBegin);
	std::string replace = inStr.substr(variableBegin + 2, variableEnd - (variableBegin + 2));
	std::string suffix  = resolvePlaceholders(inStr.substr(variableEnd + 1).c_str());

	return prefix + mVariables[replace] + suffix;
}

ThemeData::ThemeData()
{
	mHasSubsets = false;
	mColorset = Settings::getInstance()->getString("ThemeColorSet");
	mIconset = Settings::getInstance()->getString("ThemeIconSet");
	mMenu = Settings::getInstance()->getString("ThemeMenu");
	mSystemview = Settings::getInstance()->getString("ThemeSystemView");
	mGamelistview = Settings::getInstance()->getString("ThemeGamelistView");

	mVersion = 0;
}

void ThemeData::loadFile(const std::string system, std::map<std::string, std::string> sysDataMap, const std::string& path)
{
	mPaths.push_back(path);

	ThemeException error;
	error.setFiles(mPaths);

	if(!Utils::FileSystem::exists(path))
		throw error << "File does not exist!";

	mHasSubsets = false;
	mVersion = 0;
	mViews.clear();

	mSystemThemeFolder = system;

	mVariables.clear();
	mVariables.insert(sysDataMap.cbegin(), sysDataMap.cend());

	pugi::xml_document doc;
	pugi::xml_parse_result res = doc.load_file(path.c_str());
	if(!res)
		throw error << "XML parsing error: \n    " << res.description();

	pugi::xml_node root = doc.child("theme");
	if(!root)
		throw error << "Missing <theme> tag!";

	// parse version
	mVersion = root.child("formatVersion").text().as_float(-404);
	if(mVersion == -404)
		throw error << "<formatVersion> tag missing!\n   It's either out of date or you need to add <formatVersion>" << CURRENT_THEME_FORMAT_VERSION << "</formatVersion> inside your <theme> tag.";

	if(mVersion < MINIMUM_THEME_FORMAT_VERSION)
		throw error << "Theme uses format version " << mVersion << ". Minimum supported version is " << MINIMUM_THEME_FORMAT_VERSION << ".";

	parseVariables(root);
	parseTheme(root);
	
	mMenuTheme = nullptr;
	mDefaultTheme = this;
}

const std::shared_ptr<ThemeData::ThemeMenu>& ThemeData::getMenuTheme()
{
	if (mMenuTheme == nullptr && mDefaultTheme != nullptr)
		mMenuTheme = std::shared_ptr<ThemeData::ThemeMenu>(new ThemeMenu(mDefaultTheme));
	else if (mMenuTheme == nullptr)
	{
		auto emptyData = ThemeData();
		return std::shared_ptr<ThemeData::ThemeMenu>(new ThemeMenu(&emptyData));
	}

	return mMenuTheme;
}

std::string ThemeData::resolveSystemVariable(const std::string& systemThemeFolder, const std::string& path)
{
	std::string result = path;

	size_t start_pos = result.find("$system");
	if (start_pos == std::string::npos)
		return path;

	result.replace(start_pos, 7, systemThemeFolder);
	return result;
}

bool ThemeData::isFirstSubset(const pugi::xml_node& node)
{
	const std::string subsetToFind = node.attribute("subset").as_string();
	const std::string name = node.attribute("name").as_string();

	pugi::xml_node root = node.parent();

	for (pugi::xml_node node = root.child("include"); node; node = node.next_sibling("include"))
	{
		const std::string subsetAttr = node.attribute("subset").as_string();
		if (subsetAttr.empty() || subsetAttr != subsetToFind)
			continue;

		const std::string nameAttr = node.attribute("name").as_string();
		return (nameAttr == name);
	}

	return false;
}

bool ThemeData::parseSubset(const pugi::xml_node& node)
{
	if (!node.attribute("subset"))
		return true;

	mHasSubsets = true;

	const std::string subsetAttr = node.attribute("subset").as_string();
	const std::string nameAttr = node.attribute("name").as_string();

	if (subsetAttr == "colorset" && (nameAttr == mColorset || (mColorset.empty() && isFirstSubset(node))))
		return true;

	if (subsetAttr == "iconset" && (nameAttr == mIconset || (mIconset.empty() && isFirstSubset(node))))
		return true;

	if (subsetAttr == "menu" && (nameAttr == mMenu || (mMenu.empty() && isFirstSubset(node))))
		return true;

	if (subsetAttr == "systemview" && (nameAttr == mSystemview || (mSystemview.empty() && isFirstSubset(node))))
		return true;

	if (subsetAttr == "gamelistview" && (nameAttr == mGamelistview || (mGamelistview.empty() && isFirstSubset(node))))
		return true;

	return false;
}

void ThemeData::parseInclude(const pugi::xml_node& node)
{
	if (!parseSubset(node))
		return;

	if (node.attribute("tinyScreen"))
	{
		const std::string tinyScreenAttr = node.attribute("tinyScreen").as_string();

		if (!Renderer::isSmallScreen() && tinyScreenAttr == "true")
			return;

		if (Renderer::isSmallScreen() && tinyScreenAttr == "false")
			return;
	}

	std::string relPath = resolvePlaceholders(node.text().as_string());
	std::string path = Utils::FileSystem::resolveRelativePath(relPath, mPaths.back(), true);
	path = resolveSystemVariable(mSystemThemeFolder, path);

	if (!ResourceManager::getInstance()->fileExists(path))
	{
		LOG(LogWarning) << "Included file \"" << relPath << "\" not found! (resolved to \"" << path << "\")";
		return;
	}

	mPaths.push_back(path);

	pugi::xml_document includeDoc;
	pugi::xml_parse_result result = includeDoc.load_file(path.c_str());
	if (!result)
	{
		LOG(LogWarning) << "Error parsing file: \n    " << result.description() << "    from included file \"" << relPath << "\":\n    ";
		return;
	}

	pugi::xml_node theme = includeDoc.child("theme");
	if (!theme)
	{
		LOG(LogWarning) << "Missing <theme> tag!" << "    from included file \"" << relPath << "\":\n    ";
		return;
	}

	parseVariables(theme);
	parseTheme(theme);
	
	mPaths.pop_back();
}

void ThemeData::parseFeature(const pugi::xml_node& node)
{
	if (!node.attribute("supported"))
	{
		LOG(LogWarning) << "Feature missing \"supported\" attribute!";
		return;
	}

	const std::string supportedAttr = node.attribute("supported").as_string();

	if (std::find(sSupportedFeatures.cbegin(), sSupportedFeatures.cend(), supportedAttr) != sSupportedFeatures.cend())
		parseViews(node);
}

void ThemeData::parseVariable(const pugi::xml_node& node)
{
	std::string key = node.name();
	if (key.empty())
		return;

	std::string val = node.text().as_string();
	if (val.empty())
		return;
	
	mVariables.erase(key);
	mVariables.insert(std::pair<std::string, std::string>(key, val));	
}

void ThemeData::parseVariables(const pugi::xml_node& root)
{
	ThemeException error;
	error.setFiles(mPaths);
    
	pugi::xml_node variables = root.child("variables");

	if(!variables)
		return;
    
	for(pugi::xml_node_iterator it = variables.begin(); it != variables.end(); ++it)
		parseVariable(*it);
}

void ThemeData::parseViewElement(const pugi::xml_node& node)
{
	if (!node.attribute("name"))
	{
		LOG(LogWarning) << "View missing \"name\" attribute!";
		return;
	}

	if (node.attribute("tinyScreen"))
	{
		const std::string tinyScreenAttr = node.attribute("tinyScreen").as_string();

		if (!Renderer::isSmallScreen() && tinyScreenAttr == "true")
			return;

		if (Renderer::isSmallScreen() && tinyScreenAttr == "false")
			return;
	}

	const char* delim = " \t\r\n,";
	const std::string nameAttr = node.attribute("name").as_string();
	size_t prevOff = nameAttr.find_first_not_of(delim, 0);
	size_t off = nameAttr.find_first_of(delim, prevOff);
	std::string viewKey;
	while (off != std::string::npos || prevOff != std::string::npos)
	{
		viewKey = nameAttr.substr(prevOff, off - prevOff);
		prevOff = nameAttr.find_first_not_of(delim, off);
		off = nameAttr.find_first_of(delim, prevOff);

		if (std::find(sSupportedViews.cbegin(), sSupportedViews.cend(), viewKey) != sSupportedViews.cend())
		{
			ThemeView& view = mViews.insert(std::pair<std::string, ThemeView>(viewKey, ThemeView())).first->second;
			parseView(node, view);

			for (auto it = mViews.cbegin(); it != mViews.cend(); ++it)
			{
				if (it->second.isCustomView && it->second.baseType == viewKey)
				{
					ThemeView& customView = (ThemeView&)it->second;
					parseView(node, customView);
				}
			}
		}
	}
}

void ThemeData::parseTheme(const pugi::xml_node& root)
{
	if (root.attribute("defaultView"))
		mDefaultView = root.attribute("defaultView").as_string();

	for (pugi::xml_node node = root.first_child(); node; node = node.next_sibling())
	{
		std::string name = node.name();	

		if (name == "include")
			parseInclude(node);
		else if (name == "view")
			parseViewElement(node);
		else if (name == "customView")
			parseCustomView(node, root);	
	}

	// Unfortunately, recalbox does not do things in order, features have to be loaded after
	for (pugi::xml_node node = root.child("feature"); node; node = node.next_sibling("feature"))
		parseFeature(node);
}

void ThemeData::parseViews(const pugi::xml_node& root)
{
	ThemeException error;
	error.setFiles(mPaths);

	// parse views
	for (pugi::xml_node node = root.child("view"); node; node = node.next_sibling("view"))
		parseViewElement(node);	
}

void ThemeData::parseCustomViewBaseClass(const pugi::xml_node& root, ThemeView& view, std::string baseClass)
{
	bool found = false;

	// Import original view properties
	for (pugi::xml_node nodec = root.child("view"); nodec; nodec = nodec.next_sibling("view"))
	{
		if (!nodec.attribute("name"))
			continue;

		const char* delim = " \t\r\n,";
		const std::string nameAttr = nodec.attribute("name").as_string();

		size_t prevOff = nameAttr.find_first_not_of(delim, 0);
		size_t off = nameAttr.find_first_of(delim, prevOff);
		std::string viewKey;
		while (off != std::string::npos || prevOff != std::string::npos)
		{
			viewKey = nameAttr.substr(prevOff, off - prevOff);
			prevOff = nameAttr.find_first_not_of(delim, off);
			off = nameAttr.find_first_of(delim, prevOff);

			if (viewKey == baseClass)
			{
				found = true;
				parseView(nodec, view);
			}
		}
	}

	if (found)
		return;

	// base class is a customview ?
	for (pugi::xml_node nodec = root.child("customView"); nodec; nodec = nodec.next_sibling("customView"))
	{
		const std::string nameAttr = nodec.attribute("name").as_string();

		if (!nameAttr.empty() && nameAttr == baseClass)
		{
			std::string inherits = nodec.attribute("inherits").as_string();
			if (!inherits.empty() && inherits != baseClass)
			{
				view.baseType = inherits;
				parseCustomViewBaseClass(root, view, inherits);
			}

			parseView(nodec, view);
		}
	}
}

void ThemeData::parseCustomView(const pugi::xml_node& node, const pugi::xml_node& root)
{
	if (!node.attribute("name"))
		return;

	if (node.attribute("tinyScreen"))
	{
		const std::string tinyScreenAttr = node.attribute("tinyScreen").as_string();

		if (!Renderer::isSmallScreen() && tinyScreenAttr == "true")
			return;

		if (Renderer::isSmallScreen() && tinyScreenAttr == "false")
			return;
	}

	std::string viewKey = node.attribute("name").as_string();

	ThemeView& view = mViews.insert(std::pair<std::string, ThemeView>(viewKey, ThemeView())).first->second;
	view.isCustomView = true;

	std::string inherits = node.attribute("inherits").as_string();
	if (!inherits.empty())
	{
		view.baseType = inherits;
		parseCustomViewBaseClass(root, view, inherits);
	}

	parseView(node, view);
}

void ThemeData::parseView(const pugi::xml_node& root, ThemeView& view, bool overwriteElements)
{
	ThemeException error;
	error.setFiles(mPaths);

	for(pugi::xml_node node = root.first_child(); node; node = node.next_sibling())
	{
		if(!node.attribute("name"))
		{		
			LOG(LogWarning) << "Element of type \"" << node.name() << "\" missing \"name\" attribute!";
			continue;
		}		

		auto elemTypeIt = sElementMap.find(node.name());
		if(elemTypeIt == sElementMap.cend())
		{		
			LOG(LogWarning) << "Unknown element of type \"" << node.name() << "\"!";
			continue;
		}		

		if (parseRegion(node))
		{
			const char* delim = " \t\r\n,";
			const std::string nameAttr = node.attribute("name").as_string();
			size_t prevOff = nameAttr.find_first_not_of(delim, 0);
			size_t off = nameAttr.find_first_of(delim, prevOff);
			while (off != std::string::npos || prevOff != std::string::npos)
			{
				std::string elemKey = nameAttr.substr(prevOff, off - prevOff);
				prevOff = nameAttr.find_first_not_of(delim, off);
				off = nameAttr.find_first_of(delim, prevOff);

				parseElement(node, elemTypeIt->second,
					view.elements.insert(std::pair<std::string, ThemeElement>(elemKey, ThemeElement())).first->second, overwriteElements);

				if (std::find(view.orderedKeys.cbegin(), view.orderedKeys.cend(), elemKey) == view.orderedKeys.cend())
					view.orderedKeys.push_back(elemKey);
			}
		}
	}
}

bool ThemeData::parseRegion(const pugi::xml_node& node)
{
	if (!node.attribute("region"))
		return true;

	const std::string nameAttr = node.attribute("region").as_string();

	std::string regionsetting = Settings::getInstance()->getString("ThemeRegionName");
	if (regionsetting.empty())
		regionsetting = "eu";

	const char* delim = " \t\r\n,";
	
	size_t prevOff = nameAttr.find_first_not_of(delim, 0);
	size_t off = nameAttr.find_first_of(delim, prevOff);
	while (off != std::string::npos || prevOff != std::string::npos)
	{
		std::string elemKey = nameAttr.substr(prevOff, off - prevOff);
		prevOff = nameAttr.find_first_not_of(delim, off);
		off = nameAttr.find_first_of(delim, prevOff);
		if (elemKey == regionsetting)
			return true;
	}

	return false;
}

void ThemeData::parseElement(const pugi::xml_node& root, const std::map<std::string, ElementPropertyType>& typeMap, ThemeElement& element, bool overwrite)
{
	ThemeException error;
	error.setFiles(mPaths);

	element.type = root.name();
	element.extra = root.attribute("extra").as_bool(false);
	
	for(pugi::xml_node node = root.first_child(); node; node = node.next_sibling())
	{
		ElementPropertyType type = STRING;

		auto typeIt = typeMap.find(node.name());
		if(typeIt == typeMap.cend())
		{
			// Exception for menuIcons that can be extended
			if (element.type == "menuIcons")
				type = PATH;
			else
			{
				LOG(LogWarning) << "Unknown property type \"" << node.name() << "\" (for element of type " << root.name() << ").";
				continue;
			}
		}
		else
			type = typeIt->second;
		
		if (!overwrite && element.properties.find(node.name()) != element.properties.cend())
			continue;

		std::string str = resolveSystemVariable(mSystemThemeFolder, resolvePlaceholders(node.text().as_string()));

		switch(type)
		{
		case NORMALIZED_RECT:
		{
			Vector4f val;

			auto splits = Utils::String::split(str, ' ');
			if (splits.size() == 2)
			{
				val = Vector4f((float)atof(splits.at(0).c_str()), (float)atof(splits.at(1).c_str()),
					(float)atof(splits.at(0).c_str()), (float)atof(splits.at(1).c_str()));
			}
			else if (splits.size() == 4)
			{
				val = Vector4f((float)atof(splits.at(0).c_str()), (float)atof(splits.at(1).c_str()),
					(float)atof(splits.at(2).c_str()), (float)atof(splits.at(3).c_str()));
			}

			element.properties[node.name()] = val;
			break;
		}
		case NORMALIZED_PAIR:
		{
			size_t divider = str.find(' ');
			if(divider == std::string::npos) 
			{			
				LOG(LogWarning) << "invalid normalized pair (property \"" << node.name() << "\", value \"" << str.c_str() << "\")";
				break;
			}			

			std::string first = str.substr(0, divider);
			std::string second = str.substr(divider, std::string::npos);

			Vector2f val((float)atof(first.c_str()), (float)atof(second.c_str()));

			element.properties[node.name()] = val;
			break;
		}
		case STRING:
			element.properties[node.name()] = str;
			break;
		case PATH:
		{
			std::string path = Utils::FileSystem::resolveRelativePath(str, mPaths.back(), true);

			if (!ResourceManager::getInstance()->fileExists(path))
			{
				std::string rootPath = Utils::FileSystem::resolveRelativePath(str, mPaths.front(), true);
				if (ResourceManager::getInstance()->fileExists(rootPath))
					path = rootPath;
			}

			if(!ResourceManager::getInstance()->fileExists(path))
			{
				std::stringstream ss;
				ss << "  Warning " << error.msg; // "from theme yadda yadda, included file yadda yadda
				ss << "could not find file \"" << node.text().get() << "\" ";
				if(node.text().get() != path)
					ss << "(which resolved to \"" << path << "\") ";
				LOG(LogWarning) << ss.str();
			}
			else
				element.properties[node.name()] = path;

			break;
		}
		case COLOR:
			element.properties[node.name()] = getHexColor(str.c_str());
			break;
		case FLOAT:
		{
			float floatVal = static_cast<float>(strtod(str.c_str(), 0));
			element.properties[node.name()] = floatVal;
			break;
		}

		case BOOLEAN:
		{
			// only look at first char
			char first = str[0];
			// 1*, t* (true), T* (True), y* (yes), Y* (YES)
			bool boolVal = (first == '1' || first == 't' || first == 'T' || first == 'y' || first == 'Y');

			element.properties[node.name()] = boolVal;
			break;
		}
		default:
			LOG(LogWarning) << "Unknown ElementPropertyType for \"" << root.attribute("name").as_string() << "\", property " << node.name();
			break;
		}
	}
}

std::string ThemeData::getCustomViewBaseType(const std::string& view)
{
	auto viewIt = mViews.find(view);
	if (viewIt != mViews.cend())
		return viewIt->second.baseType;

	return "";
}

bool ThemeData::isCustomView(const std::string& view)
{
	auto viewIt = mViews.find(view);
	if (viewIt != mViews.cend())
		return viewIt->second.isCustomView;

	return false;
}

bool ThemeData::hasView(const std::string& view)
{
	auto viewIt = mViews.find(view);
	return (viewIt != mViews.cend());
}

const ThemeData::ThemeElement* ThemeData::getElement(const std::string& view, const std::string& element, const std::string& expectedType) const
{
	auto viewIt = mViews.find(view);
	if(viewIt == mViews.cend())
		return NULL; // not found

	auto elemIt = viewIt->second.elements.find(element);
	if(elemIt == viewIt->second.elements.cend()) return NULL;

	if(elemIt->second.type != expectedType && !expectedType.empty())
	{
		LOG(LogWarning) << " requested mismatched theme type for [" << view << "." << element << "] - expected \"" 
			<< expectedType << "\", got \"" << elemIt->second.type << "\"";
		return NULL;
	}

	return &elemIt->second;
}

const std::shared_ptr<ThemeData>& ThemeData::getDefault()
{
	static std::shared_ptr<ThemeData> theme = nullptr;
	if(theme == nullptr)
	{
		theme = std::shared_ptr<ThemeData>(new ThemeData());

		const std::string path = Utils::FileSystem::getEsConfigPath() + "/es_theme_default.xml";
		if(Utils::FileSystem::exists(path))
		{
			try
			{
				std::map<std::string, std::string> emptyMap;
				theme->loadFile("", emptyMap, path);
			} catch(ThemeException& e)
			{
				LOG(LogError) << e.what();
				theme = std::shared_ptr<ThemeData>(new ThemeData()); //reset to empty
			}
		}
	}

	return theme;
}

std::vector<GuiComponent*> ThemeData::makeExtras(const std::shared_ptr<ThemeData>& theme, const std::string& view, Window* window)
{
	std::vector<GuiComponent*> comps;

	auto viewIt = theme->mViews.find(view);
	if(viewIt == theme->mViews.cend())
		return comps;
	
	for(auto it = viewIt->second.orderedKeys.cbegin(); it != viewIt->second.orderedKeys.cend(); it++)
	{
		ThemeElement& elem = viewIt->second.elements.at(*it);
		if(elem.extra)
		{
			GuiComponent* comp = nullptr;

			const std::string& t = elem.type;
			if(t == "image")
				comp = new ImageComponent(window);
			else if(t == "text")
				comp = new TextComponent(window);
			else if (t == "ninepatch")
				comp = new NinePatchComponent(window);
			else if (t == "video")
				comp = new VideoVlcComponent(window);

			if (comp == nullptr)
				continue;

			comp->setDefaultZIndex(10);
			comp->applyTheme(theme, view, *it, ThemeFlags::ALL);
			comps.push_back(comp);
		}
	}

	return comps;
}

std::map<std::string, ThemeSet> ThemeData::getThemeSets()
{
	std::map<std::string, ThemeSet> sets;

	static const size_t pathCount = 3;
	std::string paths[pathCount] =
	{ 
		"/etc/emulationstation/themes",
		Utils::FileSystem::getEsConfigPath() + "/themes",
		"/storage/.config/emuelec/themes" // batocera
	};

	for(size_t i = 0; i < pathCount; i++)
	{
		if(!Utils::FileSystem::isDirectory(paths[i]))
			continue;

		Utils::FileSystem::stringList dirContent = Utils::FileSystem::getDirContent(paths[i]);

		for(Utils::FileSystem::stringList::const_iterator it = dirContent.cbegin(); it != dirContent.cend(); ++it)
		{
			if(Utils::FileSystem::isDirectory(*it))
			{
				ThemeSet set = {*it};
				sets[set.getName()] = set;
			}
		}
	}

	return sets;
}

std::string ThemeData::getThemeFromCurrentSet(const std::string& system)
{
	std::map<std::string, ThemeSet> themeSets = ThemeData::getThemeSets();
	if(themeSets.empty())
	{
		// no theme sets available
		return "";
	}

	std::map<std::string, ThemeSet>::const_iterator set = themeSets.find(Settings::getInstance()->getString("ThemeSet"));
	if(set == themeSets.cend())
	{
		// currently selected theme set is missing, so just pick the first available set
		set = themeSets.cbegin();
		Settings::getInstance()->setString("ThemeSet", set->first);
	}

	return set->second.getThemePath(system);
}

ThemeData::ThemeMenu::ThemeMenu(ThemeData* theme)
{
	Title.font = Font::get(FONT_SIZE_LARGE);
	Footer.font = Font::get(FONT_SIZE_SMALL);
	Text.font = Font::get(FONT_SIZE_MEDIUM);
	TextSmall.font = Font::get(FONT_SIZE_SMALL);

	auto elem = theme->getElement("menu", "menubg", "menuBackground");
	if (elem)
	{
		if (elem->has("path") && ResourceManager::getInstance()->fileExists(elem->get<std::string>("path")))
			Background.path = elem->get<std::string>("path");

		if (elem->has("fadePath") && ResourceManager::getInstance()->fileExists(elem->get<std::string>("fadePath")))
			Background.fadePath = elem->get<std::string>("fadePath");

		if (elem->has("color"))
			Background.color = elem->get<unsigned int>("color");
	}

	elem = theme->getElement("menu", "menutitle", "menuText");
	if (elem)
	{
		if (elem->has("fontPath") || elem->has("fontSize"))
			Title.font = Font::getFromTheme(elem, ThemeFlags::ALL, Font::get(FONT_SIZE_LARGE));
		if (elem->has("color"))
			Title.color = elem->get<unsigned int>("color");
	}

	elem = theme->getElement("menu", "menufooter", "menuText");
	if (elem)
	{
		if (elem->has("fontPath") || elem->has("fontSize"))
			Footer.font = Font::getFromTheme(elem, ThemeFlags::ALL, Font::get(FONT_SIZE_SMALL));
		if (elem->has("color"))
			Footer.color = elem->get<unsigned int>("color");
	}

	elem = theme->getElement("menu", "menutextsmall", "menuTextSmall");
	if (elem)
	{
		if (elem->has("fontPath") || elem->has("fontSize"))
			TextSmall.font = Font::getFromTheme(elem, ThemeFlags::ALL, Font::get(FONT_SIZE_SMALL));

		if (elem->has("color"))
			TextSmall.color = elem->get<unsigned int>("color");
		if (elem->has("selectedColor"))
			Text.selectedColor = elem->get<unsigned int>("selectedColor");
		if (elem->has("selectorColor"))
			Text.selectedColor = elem->get<unsigned int>("selectorColor");
	}

	elem = theme->getElement("menu", "menutext", "menuText");
	if (elem)
	{
		if (elem->has("fontPath") || elem->has("fontSize"))
			Text.font = Font::getFromTheme(elem, ThemeFlags::ALL, Font::get(FONT_SIZE_MEDIUM));

		if (elem->has("color"))
			Text.color = elem->get<unsigned int>("color");
		if (elem->has("separatorColor"))
			Text.separatorColor = elem->get<unsigned int>("separatorColor");
		if (elem->has("selectedColor"))
			Text.selectedColor = elem->get<unsigned int>("selectedColor");
		if (elem->has("selectorColor"))
		{
			Text.selectorColor = elem->get<unsigned int>("selectorColor");
			Text.selectorGradientColor = Text.selectorColor;
		}
		if (elem->has("selectorColorEnd"))
			Text.selectorGradientColor = elem->get<unsigned int>("selectorColorEnd");
		if (elem->has("selectorGradientType"))
			Text.selectorGradientType = !(elem->get<std::string>("selectorGradientType").compare("horizontal"));
	}

	elem = theme->getElement("menu", "menubutton", "menuButton");
	if (elem)
	{
		if (elem->has("path"))
			Icons.button = elem->get<std::string>("path");
		if (elem->has("filledPath"))
			Icons.button_filled = elem->get<std::string>("filledPath");
	}

	elem = theme->getElement("menu", "menuswitch", "menuSwitch");
	if (elem)
	{
		if (elem->has("pathOn") && ResourceManager::getInstance()->fileExists(elem->get<std::string>("pathOn")))
			Icons.on = elem->get<std::string>("pathOn");
		if (elem->has("pathOff") && ResourceManager::getInstance()->fileExists(elem->get<std::string>("pathOff")))
			Icons.off = elem->get<std::string>("pathOff");
	}

	elem = theme->getElement("menu", "menuslider", "menuSlider");
	if (elem && elem->has("path") && ResourceManager::getInstance()->fileExists(elem->get<std::string>("path")))
		Icons.knob = elem->get<std::string>("path");

	elem = theme->getElement("menu", "menuicons", "menuIcons");
	if (elem)
	{
		for (auto prop : elem->properties)
		{
			std::string path = prop.second.s;
			if (!path.empty() && ResourceManager::getInstance()->fileExists(path))
				mMenuIcons[prop.first] = path;
		}
	}
}

void ThemeData::setDefaultTheme(ThemeData* theme) 
{ 
	mDefaultTheme = theme; 
	mMenuTheme = nullptr;
};

std::vector<std::string> ThemeData::getViewsOfTheme()
{
	std::vector<std::string> ret;
	for (auto it = mViews.cbegin(); it != mViews.cend(); ++it)
	{
		if (it->first == "menu" || it->first == "system")
			continue;

		ret.push_back(it->first);
	}

	return ret;
}

std::vector<Subset> ThemeData::getThemeSubSets(const std::string& theme)
{
	std::vector<Subset> sets;

	std::deque<std::string> dequepath;

	static const size_t pathCount = 3;
	std::string paths[pathCount] =
	{
		"/etc/emulationstation/themes",		
		Utils::FileSystem::getEsConfigPath() + "/themes",
		Utils::FileSystem::getSharedConfigPath() + "/themes"
	};

	for (size_t i = 0; i < pathCount; i++)
	{
		if (!Utils::FileSystem::isDirectory(paths[i]))
			continue;

		auto dirs = Utils::FileSystem::getDirectoryFiles(paths[i] + "/" + theme);
		for (auto it = dirs.cbegin(); it != dirs.cend(); ++it)
		{
			if (!it->directory || it->hidden)
				continue;

			std::string path = it->path + "/theme.xml";
			if (!Utils::FileSystem::exists(path))
				continue;

			dequepath.push_back(path);
			pugi::xml_document doc;
			doc.load_file(path.c_str());

			pugi::xml_node root = doc.child("theme");
			crawlIncludes(root, sets, dequepath);
			findRegion(doc, sets);
			dequepath.pop_back();
		}

		std::string path = paths[i] + "/" + theme + "/theme.xml";
		if (!Utils::FileSystem::exists(path))
			continue;

		dequepath.push_back(path);
		pugi::xml_document doc;
		doc.load_file(path.c_str());

		pugi::xml_node root = doc.child("theme");
		crawlIncludes(root, sets, dequepath);
		findRegion(doc, sets);
		dequepath.pop_back();
	}

	return sets;
}

std::vector<std::string> ThemeData::getSubSet(const std::vector<Subset>& subsets, const std::string& subset)
{
	std::vector<std::string> ret;

	for (const auto& it : subsets)
	{
		if (it.subset == subset)
			ret.push_back(it.name);
	}

	return ret;
}


void ThemeData::crawlIncludes(const pugi::xml_node& root, std::vector<Subset>& sets, std::deque<std::string>& dequepath)
{
	for (pugi::xml_node node = root.child("include"); node; node = node.next_sibling("include"))
	{
		std::string name = node.attribute("name").as_string();
		std::string subset = node.attribute("subset").as_string();
		if (!subset.empty())
		{
			bool add = true;

			for (auto sb : sets) {
				if (sb.subset == subset && sb.name == name) {
					add = false; break;
				}
			}

			if (add)
				sets.push_back(Subset(subset, name));
		}
		//	sets.insert(std::pair<std::string, std::string>(name, subset));

		const char* relPath = node.text().get();
		std::string path = Utils::FileSystem::resolveRelativePath(relPath, dequepath.back(), true);

		dequepath.push_back(path);
		pugi::xml_document includeDoc;
		includeDoc.load_file(path.c_str());

		pugi::xml_node root = includeDoc.child("theme");
		crawlIncludes(root, sets, dequepath);
		findRegion(includeDoc, sets);
		dequepath.pop_back();
	}
}

void ThemeData::findRegion(const pugi::xml_document& doc, std::vector<Subset>& sets)
{
	pugi::xpath_node_set regionattr = doc.select_nodes("//@region");
	for (auto xpath_node : regionattr)
	{
		if (xpath_node.attribute() == nullptr)
			continue;
		
		std::string elemKey = xpath_node.attribute().value();
		if (elemKey.empty())
			continue;

		for (auto sb : sets)
			if (sb.subset == "region" && sb.name == elemKey)
				return;

		sets.push_back(Subset("region", elemKey));
	}
}
