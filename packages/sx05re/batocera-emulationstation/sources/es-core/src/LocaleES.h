#ifndef _LOCALE_H_
#define _LOCALE_H_

#if !defined(WIN32)

#include <boost/locale.hpp>

#define _(A) boost::locale::gettext(A)
#define ngettext(A, B, C) boost::locale::ngettext(A, B, C)

#define _U(x) x
#define UNICODE_CHARTYPE char*
#define _L(x) x

#else // WIN32

#include <string>
#include <map>
#include <functional>
#include "utils/StringUtil.h"

#define _U(x) Utils::String::convertFromWideString(L ## x)

struct PluralRule
{
	std::string key;
	std::string rule;
	std::function<int(int n)> evaluate;
};

class EsLocale
{
public:
	static const std::string getText(const std::string text);
	static const std::string nGetText(const std::string msgid, const std::string msgid_plural, int n);

	static const std::string getLanguage() { return mCurrentLanguage; }

private:
	static void checkLocalisationLoaded();
	static std::map<std::string, std::string> mItems;
	static std::string mCurrentLanguage;
	static bool mCurrentLanguageLoaded;

	static PluralRule mPluralRule;
};


#define UNICODE_CHARTYPE wchar_t*
#define _L(x) L ## x
#define _U(x) Utils::String::convertFromWideString(L ## x)

#define _(x) EsLocale::getText(x)
#define ngettext(A, B, C) EsLocale::nGetText(A, B, C)

#endif // WIN32

#endif
