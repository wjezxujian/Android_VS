#include "TinaConfig.h"
#include "widget/util/TXMLMgr.h"

TINA_NS_BEGIN
TinaConfig::TinaConfig()
:_widgetCreator(nullptr)
,_layoutSwitch(true)
{
}

TinaConfig::~TinaConfig()
{
	if (_widgetCreator != nullptr)
	{
		CC_SAFE_DELETE(_widgetCreator);
	}
}

void TinaConfig::setCustomWidgetCreator(CustomWidgetCreator* creator)
{
	_widgetCreator = creator;
}

CustomWidgetCreator* TinaConfig::getCustomWidgetCreator()
{
	return _widgetCreator;
}

bool TinaConfig::pushFontDef(const FontDef& def)
{
	auto _it = _fontDefMap.find(def.fontName);
	if (_it != _fontDefMap.end())
		return false;

	_fontDefMap.insert({def.fontName, def});
	return true;
}

FontDef* TinaConfig::getFontDefWithName(const std::string& fontName)
{
	auto _it = _fontDefMap.find(fontName);
	if (_it == _fontDefMap.end())
		return nullptr;

	return &_it->second;
}

FontDef* TinaConfig::getDefaultFontDef()
{
	if (_fontDefMap.empty())
		return nullptr;

	auto _it = std::find_if(_fontDefMap.begin(), _fontDefMap.end(), [](const std::pair<std::string, FontDef>& it)->bool{
		return it.second.isdefault;
	});

	if (_it == _fontDefMap.end())
		return nullptr;

	return &_it->second;
}
TINA_NS_END