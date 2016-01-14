#pragma once

/****************************************************************************
功 能：图片类
============================================================================
Copyright (c) 2015 Gintama@DeNA
创建者：徐剑
创建日期：2015-05-13
最后修改者：徐剑
最后修改日期：2015-05-13
****************************************************************************/

#include "base/Widget.h"

TINA_NS_BEGIN
class ProgressTimer : public Widget
{
public:
	enum class Type
	{
		RADIAL,
		BAR,
	};
public:
	ProgressTimer();
	static ProgressTimer* create();
	static ProgressTimer* create(const std::string& file);
	virtual void parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values) override;

public:
	virtual void setSource(const std::string& file);
	virtual void setProcessType(ProgressTimer::Type);
	
	//动态调用的接口
	virtual void setPercentage(float percent, float duration = 0.0f);
	virtual void setPercentFromTo(float fromPercent, float toPercent, float duration = 0.0f);

	void setReverseDirection(bool value);
	void setMidpoint(const cocos2d::Vec2& point);
	void setBarChangeRate(const cocos2d::Vec2& barChangeRate);
	void setColor(const cocos2d::Color3B &color);
	void setOpacity(GLubyte opacity);

	inline const cocos2d::ProgressTimer* getRenderer(){ return m_renderer; };
	
protected:
	virtual bool init(const std::string& file);

	//给Parse解析调用
	virtual void setFromPercentage(float percent);
	virtual void setToPercentage(float percent);
	virtual void setPercentDuration(float duration);

	virtual void updatePercentage();
	virtual void updateSpriteAttr();
	virtual void drawRenderer(bool updateSize = true) override;

protected:
	cocos2d::ProgressTimer*		m_renderer = nullptr;
	ProgressTimer::Type			m_type;
	std::string					m_source;

	float						m_fromPercent;
	float						m_toPercent;
	float						m_duration;

	bool						m_isFromPercent;
	bool						m_isToPercent;

	cocos2d::Color3B			m_color;
	GLubyte						m_opacity;

	bool						m_isColor;
	bool						m_isOpacity;


};
TINA_NS_END