#pragma once

/****************************************************************************
功 能：矩形
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-03-04
最后修改者：董伟
最后修改日期：2015-03-04
****************************************************************************/

#include "base/Widget.h"

TINA_NS_BEGIN
class ShapeRect : public Widget
{
public:
	static ShapeRect* create();
	virtual void parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values) override;

public:
	void setBorderWidth(float width);
	float getBorderWidth(){ return m_borderWidth; };
	void setBorderColor(const cocos2d::Color4B& color);
	void setBorderColor(const std::string& colorStr);
	const cocos2d::Color4B& getBorderColor(){ return m_borderColor; };
	void setBgColor(const cocos2d::Color4B& color);
	void setBgColor(const std::string& colorStr);
	const cocos2d::Color4B& getBgColor(){ return m_bgColor; };

protected:
	virtual bool init();
	virtual void drawRenderer(bool updateSize = true) override;
	void setOpacity(GLubyte opacity);

protected:
	cocos2d::DrawNode*		m_renderer;
	cocos2d::Color4B		m_bgColor = cocos2d::Color4B::BLACK;
	cocos2d::Color4B		m_borderColor = cocos2d::Color4B::WHITE;
	float					m_borderWidth = 0;
	GLubyte					m_opacity = 0;
	bool					m_isOpacity = false;

};
TINA_NS_END