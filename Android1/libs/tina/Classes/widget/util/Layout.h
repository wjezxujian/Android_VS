#pragma once

/****************************************************************************
功 能：布局控制类
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-03-22
最后修改者：董伟
最后修改日期：2015-03-22
****************************************************************************/

#include "include/tinaMacros.h"
#include "widget/base/Widget.h"
#include "cocos2d.h"

TINA_NS_BEGIN
class Layout
{
public:
	Layout()
		:m_type(LayoutType::ABSOLUTE)
		, m_paddingLeft(0)
		, m_paddingRight(0)
		, m_paddingTop(0)
		, m_paddingBottom(0)
		, m_hGap(0)
		, m_vGap(0)
		, m_align(HAlign::LEFT)
		, m_valign(VAlign::BOTTOM)
		, m_virtualSize(cocos2d::Size(-1, -1))
		, m_percentSize(cocos2d::Size(-1, -1))
	{};
	virtual const cocos2d::Size& calcVirtualSize(const cocos2d::Vector<Widget*>& children, const cocos2d::Size& givenSize);
	virtual cocos2d::Size doLayout(const cocos2d::Vector<Widget*>& children, const cocos2d::Size& givenSize);

protected:
	cocos2d::Size absoluteLayout(const cocos2d::Vector<Widget*>& children, const cocos2d::Size& givenSize);
	cocos2d::Size horizontalLayout(const cocos2d::Vector<Widget*>& children, const cocos2d::Size& givenSize);
	cocos2d::Size verticalLayout(const cocos2d::Vector<Widget*>& children, const cocos2d::Size& givenSize);
	cocos2d::Size tileLayout(const cocos2d::Vector<Widget*>& children, const cocos2d::Size& givenSize);

protected:
	CC_SYNTHESIZE(LayoutType, m_type, Type);
	CC_SYNTHESIZE(uint16_t, m_paddingLeft, PaddingLeft);
	CC_SYNTHESIZE(uint16_t, m_paddingRight, PaddingRight);
	CC_SYNTHESIZE(uint16_t, m_paddingTop, PaddingTop);
	CC_SYNTHESIZE(uint16_t, m_paddingBottom, PaddingBottom);
	CC_SYNTHESIZE(uint16_t, m_hGap, HGap);
	CC_SYNTHESIZE(uint16_t, m_vGap, VGap);
	CC_SYNTHESIZE(HAlign, m_align, HAlign);
	CC_SYNTHESIZE(VAlign, m_valign, VAlign);
	CC_SYNTHESIZE_PASS_BY_REF(cocos2d::Size, m_virtualSize, VirtualSize);
	CC_SYNTHESIZE_PASS_BY_REF(cocos2d::Size, m_percentSize, PercentSize);
};
TINA_NS_END