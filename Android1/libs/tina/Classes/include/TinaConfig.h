#pragma once

/****************************************************************************
功 能：Tina全局配置对象
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-09-11
最后修改者：董伟
最后修改日期：2015-09-11
****************************************************************************/

#include "include/tinaMacros.h"
#include "platform/CCPlatformMacros.h"
#include <string>
#include "2d/CCLabel.h"

TINA_NS_BEGIN
struct FontDef
{
	FontDef(const std::string& _fontName, LabelType _type, const std::string& _file, bool _default = false)
	:fontName(_fontName)
	, type(_type)
	, file(_file)
	, isdefault(_default)
	{}

	std::string		fontName;
	LabelType		type;
	std::string		file;
	bool			isdefault;
};
class CustomWidgetCreator;
class TinaConfig
{
public:
    TinaConfig();
	~TinaConfig();
	void setCustomWidgetCreator(CustomWidgetCreator* creator);
	CustomWidgetCreator* getCustomWidgetCreator();
	bool pushFontDef(const FontDef& def);
	FontDef* getFontDefWithName(const std::string& fontName);
	FontDef* getDefaultFontDef();

private:
	CustomWidgetCreator*	_widgetCreator;
	std::map<std::string, FontDef>	_fontDefMap;
	CC_SYNTHESIZE_PASS_BY_REF(std::string, _imageDir, ImageDir);
	CC_SYNTHESIZE_PASS_BY_REF(std::string, _txmlDir, TxmlDir);
	CC_SYNTHESIZE_PASS_BY_REF(std::string, _animDir, AnimDir);
	CC_SYNTHESIZE_PASS_BY_REF(std::string, _fontDir, FontDir);
	CC_SYNTHESIZE_PASS_BY_REF(std::string, _xmlDir, XmlDir);
	CC_SYNTHESIZE_PASS_BY_REF(std::string, _langDir, LangDir);
	CC_SYNTHESIZE_PASS_BY_REF(std::string, _soundDir, SoundDir);
	CC_SYNTHESIZE_PASS_BY_REF(std::string, _shaderDir, ShaderDir);
	CC_SYNTHESIZE_PASS_BY_REF(std::string, _particleDir, ParticleDir);
	CC_SYNTHESIZE_PASS_BY_REF(std::string, _resDir, ResDir);
	CC_SYNTHESIZE_PASS_BY_REF(bool, _layoutSwitch, LayoutSwitch);
};
TINA_NS_END