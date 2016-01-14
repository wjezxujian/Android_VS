#pragma once

/****************************************************************************
功 能：裁剪
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-03-04
最后修改者：董伟
最后修改日期：2015-03-04
****************************************************************************/

#include "base/Widget.h"

TINA_NS_BEGIN
class ClipView : public Widget
{
public:
	static ClipView* create();
	virtual void parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values) override;
	virtual void parseAttr(const std::string& key, const std::vector<Widget*>& nodeParams) override;

public:
	virtual void setStencil(Widget* stencilNode);
	cocos2d::Node* getStencil(){ return m_renderer->getStencil(); };
	void setAlphaThreshold(float alpha);
	void setInverted(bool inverted);
	virtual void addWidget(Widget* child) override;
	virtual void addWidget(Widget* child, const std::string &id) override;
	virtual void addWidget(Widget* child, int localZOrder) override;
	virtual void addWidget(Widget* child, int localZOrder, const std::string &id) override;
	virtual void removeWidget(Widget* child, bool cleanup = true) override;
	virtual void removeAllWidget() override;

protected:
	virtual bool init();

protected:
	cocos2d::ClippingNode*		m_renderer = nullptr;
};
TINA_NS_END