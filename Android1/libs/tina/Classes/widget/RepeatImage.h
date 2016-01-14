#pragma once

/****************************************************************************
�� �ܣ��ظ�ͼƬ��
============================================================================
Copyright (c) 2015 Tungway
�����ߣ���ΰ
�������ڣ�2015-03-13
����޸��ߣ���ΰ
����޸����ڣ�2015-03-13
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