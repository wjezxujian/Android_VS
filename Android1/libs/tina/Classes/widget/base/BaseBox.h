#pragma once

/****************************************************************************
功 能：布局基础类
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-02-26
最后修改者：董伟
最后修改日期：2015-03-30
****************************************************************************/

#include "Widget.h"
#include <functional>

TINA_NS_BEGIN
class Layout;
class ScrollView;
class ListView;
class BaseBox : public Widget
{
public:
	typedef std::function<void(BaseBox* box)> layoutFunc;

public:
	static BaseBox* create();
	void parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values);

public:
	virtual void setVAlign(VAlign align);
	VAlign getVAlign();
	virtual void setHAlign(HAlign align);
	HAlign getHAlign();
	virtual void setGap(uint16_t gap);
	uint16_t getGap();
	virtual void setHGap(uint16_t gap);
	uint16_t getHGap();
	virtual void setVGap(uint16_t gap);
	uint16_t getVGap();
	virtual void setPaddingLeft(uint16_t paddingLeft);
	uint16_t getPaddingLeft();
	virtual void setPaddingRight(uint16_t paddingRight);
	uint16_t getPaddingRight();
	virtual void setPaddingTop(uint16_t paddingTop);
	uint16_t getPaddingTop();
	virtual void setPaddingBottom(uint16_t paddingBottom);
	uint16_t getPaddingBottom();
	void setLayoutCallfunc(const layoutFunc& func){ m_callfunc = func; };

protected:
	~BaseBox();
	virtual bool init(LayoutType type);
	friend class ScrollView;
	friend class ListView;
	virtual void setLayoutType(LayoutType type) final;
	virtual void relayoutByChildrenChanges() override;
	virtual void drawRenderer(bool updateSize = true) override;

protected:
	Layout*		m_layout = nullptr;
	layoutFunc	m_callfunc;
};
TINA_NS_END