#pragma once

/****************************************************************************
功 能：文本类
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-02-21
最后修改者：董伟
最后修改日期：2015-02-27
****************************************************************************/

#include "base/Widget.h"

TINA_NS_BEGIN
class Label : public Widget
{
public:
	static Label* create();
	static Label* create(const std::string& textContent, int fontSize);
	static Label* create(int fontSize);
	static Label* create(const std::string& textContent, const std::string& fontName, int fontSize);
	virtual void parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values) override;

public:
	void setString(const std::string& text);
	void setText(const std::string& text);
	const std::string& getString() const;
	const std::string& getText() const;
	ssize_t getStringLength() const;
	void setTextColor(cocos2d::Color4B color);
	void setTextColor(const std::string& colorStr);
	cocos2d::Color4B getTextColor();
	void setFontSize(int size);
	int getFontSize() const;
	void setFontName(const std::string& fontName);
	void setLineHeight(float height);
	void setDimensions(float width, float height);
	void setHorizontalAlignment(cocos2d::TextHAlignment alignment);
	cocos2d::TextHAlignment getHorizontalAlignment() const{ return m_renderer->getHorizontalAlignment(); };
	void setVerticalAlignment(cocos2d::TextVAlignment alignment);
	cocos2d::TextVAlignment getVerticalAlignment()const{ return m_renderer->getVerticalAlignment(); };
	virtual void enableShadow(const cocos2d::Color4B& shadowColor = cocos2d::Color4B::BLACK, const cocos2d::Size &offset = cocos2d::Size(2, -2), int blurRadius = 0);
	virtual void enableOutline(const cocos2d::Color4B& outlineColor, int outlineSize = 1);
	virtual void enableGlow(const cocos2d::Color4B& glowColor);
	virtual void disableEffect();
	cocos2d::Label* getRenderer(){ return m_renderer; };
	void setLineBreakWithoutSpace(bool breakWithoutSpace);

protected:
	virtual bool init(const std::string& textContent, const std::string& fontName, int fontSize);
	virtual bool init();
	void createLabel(const std::string& fontName);
	LabelType getLabelType(const std::string& fontName);
	virtual void drawRenderer(bool updateSize = true) override;

protected:
	cocos2d::Label*				m_renderer = nullptr;
	std::string					m_text;
	LabelType					m_labelType;
	bool						m_needUpdateRendererSize = false;
	cocos2d::Color4B			m_shadowColor;
	cocos2d::Size				m_shadowDistance = cocos2d::Size::ZERO;
	int							m_shadowRadius = 0;
	bool						m_shadowEnabled = false;
	cocos2d::Color4B			m_glowColor;
	bool						m_glowEnabled = false;
	cocos2d::Color4B			m_outlineColor;
	float						m_outlineSize;
	bool						m_outlineEnabled = false;

};
TINA_NS_END