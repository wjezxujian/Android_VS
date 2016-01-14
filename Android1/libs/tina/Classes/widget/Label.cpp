#include "Label.h"
#include "util/ParamUtil.h"
#include "util/StringUtil.h"
#include "include/TinaConfig.h"
#include "util/TinaMgr.h"

TINA_NS_BEGIN
Label* Label::create()
{
	auto _ref = new Label();
	if (_ref && _ref->init())
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

Label* Label::create(const std::string& textContent, int fontSize)
{
	return create(textContent, "", fontSize);
}

Label* Label::create(int fontSize)
{
	return create("", "", fontSize);
}

Label* Label::create(const std::string& textContent, const std::string& fontName, int fontSize)
{
	auto _ref = new Label();
	if (_ref && _ref->init(textContent, fontName, fontSize))
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

void Label::parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values)
{
	Widget::parseAttr(keys, values);
	for (auto i = 0; i < keys->size(); ++i)
	{
		const auto& _key = keys->at(i);
		const auto& _value = values->at(i);
		if (_key == "string" || _key == "text")
		{
			setString(_value);
		}
		else if (_key == "fontsize")
		{
			setFontSize(ParamUtil::readInt(_value));
		}
		else if (_key == "font" || _key == "fontname")
		{
			setFontName(_value);
		}
		else if (_key == "lineheight")
		{
			setLineHeight(ParamUtil::readFloat(_value));
		}
		else if (_key == "textcolor" || _key == "fontcolor")
		{
			setTextColor(ParamUtil::readColor4B(_value));
		}
		else if (_key == "align" || _key == "halign")
		{
			setHorizontalAlignment(ParamUtil::readTextHalign(_value));
		}
		else if (_key == "valign")
		{
			setVerticalAlignment(ParamUtil::readTextValign(_value));
		}
		else if (_key == "border" || _key == "borderwidth")
		{
			int _borderWidth = ParamUtil::readInt(_value);
			cocos2d::Color4B _borderColor = cocos2d::Color4B::BLACK;
			auto _it = std::find(keys->begin(), keys->end(), "bordercolor");
			if (_it != keys->end())
			{
				_borderColor = ParamUtil::readColor4B(*(values->begin() +(_it - keys->begin())));
			}
			enableOutline(_borderColor, _borderWidth);
		}
		else if (_key == "shadow" || _key == "shadowwidth")
		{
			int _shadowWidth = ParamUtil::readInt(_value);
			cocos2d::Color4B _shadowColor = cocos2d::Color4B::BLACK;
			cocos2d::Size _shadowDistance = cocos2d::Size(2, 2);
			auto _it = std::find(keys->begin(), keys->end(), "shadowcolor");
			if (_it != keys->end())
			{
				_shadowColor = ParamUtil::readColor4B(*(values->begin() + (_it - keys->begin())));
			}
			_it = std::find(keys->begin(), keys->end(), "shadowdistance");
			if (_it != keys->end())
			{
				_shadowDistance = ParamUtil::readSize(*(values->begin() + (_it - keys->begin())));
			}
			enableShadow(_shadowColor, _shadowDistance, _shadowWidth);
		}
		else if (_key == "glow")
		{
			enableGlow(ParamUtil::readColor4B(_value));
		}
	}
}

void Label::setString(const std::string& text)
{
	if (m_text == text)return;
	m_text = text;
	std::string _text = m_text;
	StringUtil::replace(&_text, "\\n", "\n");
	StringUtil::replace(&_text, "<br/>", "\n");
	m_renderer->setString(_text);
	DRAW_RENDERER;
}

void Label::setText(const std::string& text)
{
	setString(text);
}

const std::string& Label::getString() const
{
	return m_text;
}

ssize_t Label::getStringLength() const
{
	return m_renderer->getStringLength();
}

void Label::setTextColor(cocos2d::Color4B color)
{
	if (m_labelType == LabelType::TTF || m_labelType == LabelType::SYSTEM)
		m_renderer->setTextColor(color);
}

void Label::setTextColor(const std::string& colorStr)
{
	std::string _colorStr = colorStr;
	if (!_colorStr.empty() && _colorStr.front() != '#')
	{
		_colorStr.insert(_colorStr.begin(), '#');
	}
	setTextColor(ParamUtil::readColor4B(_colorStr));
}

cocos2d::Color4B Label::getTextColor()
{
	return m_renderer->getTextColor();
}

void Label::setFontSize(int size)
{
	switch (m_labelType)
	{
	case LabelType::TTF:
		do
		{
			auto _ttf = m_renderer->getTTFConfig();
			_ttf.fontSize = size;
			m_renderer->setTTFConfig(_ttf);
		} while (0);
		break;
	case LabelType::SYSTEM:
		m_renderer->setSystemFontSize(size);
		break;
	default:
		break;
	}
	DRAW_RENDERER;
}

int Label::getFontSize() const
{
	if (m_labelType == LabelType::TTF)
	{
		return m_renderer->getTTFConfig().fontSize;
	}
	else
	{
		return m_renderer->getSystemFontSize();
	}
}

void Label::setFontName(const std::string& fontName)
{
	LabelType _labelType = getLabelType(fontName);
	if (_labelType != m_labelType)
	{
		m_labelType = _labelType;
		auto _color = getTextColor();
		auto _size = getFontSize();
		auto _text = getString();
		auto _align = getHorizontalAlignment();
		auto _valign = getVerticalAlignment();
		m_renderer->removeFromParent();
		createLabel(fontName);
		setString(_text);
		setTextColor(_color);
		setFontSize(_size);
		setHorizontalAlignment(_align);
		setVerticalAlignment(_valign);
		if (m_glowEnabled)
		{
			enableGlow(m_glowColor);
		}
		if (m_outlineEnabled)
		{
			enableOutline(m_outlineColor, m_outlineSize);
		}
		if (m_shadowEnabled)
		{
			enableShadow(m_shadowColor, m_shadowDistance, m_shadowRadius);
		}
	}
	else
	{
		switch (m_labelType)
		{
		case LabelType::TTF:
			do
			{
				auto _ttf = m_renderer->getTTFConfig();
				auto _fontDef = TINA_MGR->getConfig()->getFontDefWithName(fontName);
				if (_fontDef != nullptr)
					_ttf.fontFilePath = TINA_MGR->getFontFullPath(_fontDef->file);
				m_renderer->setTTFConfig(_ttf);
			} while (0);
			break;
		case LabelType::SYSTEM:
			m_renderer->setSystemFontName(fontName);
			break;
		default:
			break;
		}
	}

	DRAW_RENDERER;
}

void Label::setLineHeight(float height)
{
	m_renderer->setLineHeight(height);
	DRAW_RENDERER;
}

void Label::setDimensions(float width, float height)
{
	setContentSize(cocos2d::Size(width, height));
}

void Label::setHorizontalAlignment(cocos2d::TextHAlignment alignment)
{
	m_renderer->setHorizontalAlignment(alignment);
}

void Label::setVerticalAlignment(cocos2d::TextVAlignment alignment)
{
	m_renderer->setVerticalAlignment(alignment);
}

void Label::enableShadow(const cocos2d::Color4B& shadowColor, const cocos2d::Size &offset, int blurRadius)
{
	m_renderer->enableShadow(shadowColor, offset, blurRadius);
	m_shadowColor = shadowColor;
	m_shadowDistance = offset;
	m_shadowRadius = blurRadius;
	m_shadowEnabled = true;
	DRAW_RENDERER;
}

void Label::enableOutline(const cocos2d::Color4B& outlineColor, int outlineSize)
{
	m_renderer->enableOutline(outlineColor, outlineSize);
	m_outlineColor = outlineColor;
	m_outlineSize = outlineSize;
	m_outlineEnabled = true;
	DRAW_RENDERER;
}

void Label::enableGlow(const cocos2d::Color4B& glowColor)
{
	m_renderer->enableGlow(glowColor);
	m_glowColor = glowColor;
	m_glowEnabled = true;
	DRAW_RENDERER;
}

void Label::disableEffect()
{
	m_renderer->disableEffect();
	m_outlineEnabled = false;
	m_glowEnabled = false;
	m_shadowEnabled = false;
	DRAW_RENDERER;
}

void Label::setLineBreakWithoutSpace(bool breakWithoutSpace)
{
	m_renderer->setLineBreakWithoutSpace(true);
}

bool Label::init(const std::string& textContent, const std::string& fontName, int fontSize)
{
	if (!Widget::init())
	{
		return false;
	}
	setType(WidgetType::LABEL);
	std::string _fontName = fontName;
	if (_fontName.empty())
	{
		auto _defaultFontDef = TINA_MGR->getConfig()->getDefaultFontDef();
		_fontName = (_defaultFontDef != nullptr) ? _defaultFontDef->fontName : "arial";
	}

	m_labelType = getLabelType(_fontName);
	createLabel(_fontName);
	setString(textContent);
	setFontSize(fontSize);
	setTextColor(cocos2d::Color4B::BLACK);
	return true;
}

bool Label::init()
{
	auto _defaultFontDef = TINA_MGR->getConfig()->getDefaultFontDef();
	auto _defaltFontName = (_defaultFontDef != nullptr) ? _defaultFontDef->fontName : "arial";
	return init("", _defaltFontName, 14);
}

void Label::createLabel(const std::string& fontName)
{
	if (m_labelType == LabelType::TTF)
	{
		m_renderer = cocos2d::Label::createWithTTF(TINA_MGR->getTTFConfigWithFontName(fontName), 
			m_text);
	}
	else if (m_labelType == LabelType::BMFONT)
	{
		auto _labelDef = TINA_MGR->getConfig()->getFontDefWithName(fontName);
		m_renderer = cocos2d::Label::createWithBMFont(TINA_MGR->getFontFullPath(_labelDef->file),
			m_text);
	}
	else
	{
		m_renderer = cocos2d::Label::createWithSystemFont(m_text, fontName, 0);
	}
	m_renderer->ignoreAnchorPointForPosition(true);
	Node::addChild(m_renderer);
}

LabelType Label::getLabelType(const std::string& fontName)
{
	auto _labelDef = TINA_MGR->getConfig()->getFontDefWithName(fontName);

	if (!_labelDef)
		return LabelType::SYSTEM;

	return _labelDef->type;
}

void Label::drawRenderer(bool updateSize)
{
	if (m_parsing)return;
	if (m_givenSize.width >= 0)
		m_renderer->setWidth(m_givenSize.width);
	if (m_givenSize.height >= 0)
		m_renderer->setHeight(m_givenSize.height);
	if (updateSize)
		updateSizeFromContent(m_renderer->getContentSize());
}

TINA_NS_END