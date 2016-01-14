#pragma once

/****************************************************************************
功 能：垂直或横向滚动组件
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-03-07
最后修改者：董伟
最后修改日期：2015-03-07
****************************************************************************/

#include "ScrollView.h"

TINA_NS_BEGIN
class BaseBox;
class ListView : public ScrollView
{
public:
	static ListView* create();
	virtual void parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values) override;

public:
	virtual void setLayoutType(LayoutType type);
	virtual LayoutType getLayoutType();
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

protected:
	virtual bool init(const cocos2d::Size& viewSize);
	virtual void updateSizeFromParent(const cocos2d::Size& virtualSize) override;
	virtual void updateSize(const cocos2d::Size& contentSize) override;

protected:
	LayoutType	m_layoutType;
};
TINA_NS_END