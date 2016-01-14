#pragma once

/****************************************************************************
�� �ܣ�Layer
============================================================================
Copyright (c) 2015 Tungway
�����ߣ���ΰ
�������ڣ�2015-02-26
����޸��ߣ���ΰ
����޸����ڣ�2015-09-11
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