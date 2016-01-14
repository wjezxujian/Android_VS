#pragma once

/****************************************************************************
�� �ܣ��������������ľŹ���ͼƬ
============================================================================
Copyright (c) 2015 Tungway
�����ߣ���ΰ
�������ڣ�2015-03-04
����޸��ߣ���ΰ
����޸����ڣ�2015-03-04
****************************************************************************/

#include "base/Widget.h"
#include "cocos-ext.h"

TINA_NS_BEGIN
class ProgressImage : public Widget
{
public:
	static ProgressImage* create();
	static ProgressImage* create(const std::string& file, const cocos2d::Size& size, const cocos2d::Rect& capInsets);
	static ProgressImage* create(const std::string& file, const cocos2d::Size& size);
	static ProgressImage* create(const std::string& file, const cocos2d::Rect& rect, const cocos2d::Rect& capInsets);
	static ProgressImage* create(const cocos2d::Rect& capInsets, const std::string& file);
	static ProgressImage* create(const std::string& file, const cocos2d::Rect& rect);
	virtual void setSource(const std::string& src, const cocos2d::Rect& capInset);
	virtual void setPercent(float percent);
	float getPercent(){ return m_percent; };
	virtual void parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values) override;

protected:
	virtual bool init(const std::string& file, const cocos2d::Rect& rect, const cocos2d::Rect& capInsets);
	virtual void drawRenderer(bool updateSize = true) override;

protected:
	cocos2d::ClippingNode*		m_renderer;
	cocos2d::ui::Scale9Sprite*	m_img;
	cocos2d::DrawNode*			m_stencil;
	std::string					m_source;
	float						m_percent;
	bool						m_useAbsPath = false;
};
TINA_NS_END