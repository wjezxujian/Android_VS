#pragma once

/****************************************************************************
功 能：Node
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-02-26
最后修改者：董伟
最后修改日期：2015-09-11
****************************************************************************/

#include "include/tinaMacros.h"
#include "widget/base/Widget.h"
#include "2d/CCNode.h"

TINA_NS_BEGIN
class WidgetReader;
class TiNode : public cocos2d::Node
{
public:
	static TiNode* create(const std::string& file);
	static TiNode* create();
	TiNode();
	~TiNode();
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