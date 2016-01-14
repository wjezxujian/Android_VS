#pragma once

/****************************************************************************
功 能：圆形
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-03-04
最后修改者：董伟
最后修改日期：2015-03-04
****************************************************************************/

#include "base/Widget.h"

TINA_NS_BEGIN
class ShapeCircle : public Widget
{
public:
	static ShapeCircle* create();
	virtual void parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values) override;

public:
	void setBgColor(const cocos2d::Color4B& color);
	void setBgColor(const std::string& colorStr);
	const cocos2d::Color4B& getBgColor(){ return m_bgColor; };

protected:
	virtual bool init();
	virtual void drawRenderer(bool updateSize = true) override;

protected:
	cocos2d::DrawNode*		m_renderer;
	cocos2d::Color4B		m_bgColor = cocos2d::Color4B::BLACK;
};
TINA_NS_END