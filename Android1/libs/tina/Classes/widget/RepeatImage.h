#pragma once

/****************************************************************************
功 能：重复图片类
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-03-13
最后修改者：董伟
最后修改日期：2015-03-13
****************************************************************************/

#include "base/Widget.h"

TINA_NS_BEGIN
class RepeatImage : public Widget
{
public:
	static RepeatImage* create();
	static RepeatImage* create(const std::string& file);
	virtual void parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values) override;

public:
	virtual void setSource(const std::string& file);
	virtual void setTextureRect(const cocos2d::Rect& rect);
	virtual void setRepeatCnt(uint16_t cnt);
	uint16_t getRepeatCnt(){ return m_repeatCnt; };
	virtual void setDirection(ScrollDirection direction);
	ScrollDirection getDirection();
	virtual void setGap(int gap);

protected:
	virtual bool init(const std::string& file);
	virtual void drawRenderer(bool updateSize = true) override;

protected:
	cocos2d::SpriteBatchNode*	m_renderer = nullptr;
	uint16_t					m_repeatCnt = 1;
	LayoutParam					m_layoutParam;
	cocos2d::Rect				m_textureRect;
	bool						m_useAbsPath = false;
};
TINA_NS_END