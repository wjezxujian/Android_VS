#pragma once

/****************************************************************************
功 能：Layer
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-02-26
最后修改者：董伟
最后修改日期：2015-09-11
****************************************************************************/

#include "include/tinaMacros.h"
#include "widget/base/Widget.h"
#include "2d/CCLayer.h"

TINA_NS_BEGIN
class WidgetReader;
class TiLayer : public cocos2d::Layer
{
public:
	static TiLayer* create(const std::string& file);
	static TiLayer* create();
	TiLayer();
	~TiLayer();
	WidgetReader* getUIReader();

protected:
	virtual bool initWithTxml(const std::string& file);
	virtual bool init();
	virtual void handlerButton(Widget*, tina::TouchType){};
	void foreachWidgetChildren(Widget* parent);

protected:
	WidgetReader*	m_uiReader;
};
TINA_NS_END