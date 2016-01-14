#pragma once

/****************************************************************************
功 能：布局控制类
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-03-07
最后修改者：董伟
最后修改日期：2015-03-07
****************************************************************************/

#include "include/tinaMacros.h"
#include "widget/base/Widget.h"
#include "cocos2d.h"

TINA_NS_BEGIN
class LayoutUtil
{
public:
	static cocos2d::Size layout(const LayoutParam& param,
		const cocos2d::Size& givenSize,
		const cocos2d::Vector<Widget*>& widgetList);

	static cocos2d::Size layoutTest(const LayoutParam& param,
		const cocos2d::Size& givenSize,
		const cocos2d::Vector<Widget*>& widgetList);

	static cocos2d::Size layout(const LayoutParam& param,
		const cocos2d::Size& givenSize,
		const cocos2d::Vector<cocos2d::Node*>& nodeList);

private:
	static cocos2d::Size layoutVertical(
		const LayoutParam& param, 
		const cocos2d::Size& givenSize, 
		const cocos2d::Vector<Widget*>& widgetList);

	static cocos2d::Size layoutVertical(
		const LayoutParam& param,
		const cocos2d::Size& givenSize,
		const cocos2d::Vector<cocos2d::Node*>& nodeList);

	static cocos2d::Size layoutHorizontal(
		const LayoutParam& param,
		const cocos2d::Size& givenSize,
		const cocos2d::Vector<Widget*>& widgetList);

	static cocos2d::Size layoutHorizontal(
		const LayoutParam& param,
		const cocos2d::Size& givenSize,
		const cocos2d::Vector<cocos2d::Node*>& nodeList);

	static cocos2d::Size layoutAbsolute(
		const LayoutParam& param,
		const cocos2d::Size& givenSize,
		const cocos2d::Vector<Widget*>& widgetList);

	static cocos2d::Size layoutTile(
		const LayoutParam& param,
		const cocos2d::Size& givenSize,
		const cocos2d::Vector<Widget*>& widgetList);

	static cocos2d::Vec2 getRealAnchor(cocos2d::Node*);
};
TINA_NS_END